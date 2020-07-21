#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "test_pal_mem.h"
#include "test_pal_mem_s.h"
#include "test_pal_log.h"

#define MPS2_PLUS_MEM_S_DMA_BASE_ADDR		(0x38200000 + 0x00100000)
#define MPS2_PLUS_MEM_S_DMA_BASE_ADDR_LEN	(0x00100000) /* 1MB */
#define MPS2_PLUS_MEM_NS_DMA_BASE_ADDR		(0x28000000)
#define MPS2_PLUS_MEM_NS_DMA_BASE_ADDR_LEN	(0x00200000) /* 2MB */

#define MPS2_PLUS_MEM_SYSTEM_RAM			0x20000000
#define MPS2_PLUS_MEM_SYSTEM_RAM_LEN		0x00800000 /* 8MB */
#define MPS2_PLUS_MEM_DMA_BASE_ADDR			0x21200000
#define MPS2_PLUS_MEM_DMA_AREA_LEN			0x00E00000 /* 14MB */

/*******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/

#if defined(Zynq)
static int isInRange(void *addr)
{
	if (((unsigned long)(addr) >= Test_PalGetDMABaseAddr()
		&& (unsigned long)(addr) <= Test_PalGetDMABaseAddr() + 0x0F000000)) {
		return 0;
	}
	TEST_PRINTF("NOT in DMA range \r\n");
	return 1;
}
#elif defined(MPS2) && defined(ARMCM3)
static int isInRange(void *addr)
{
	if ((((unsigned long)(addr) >= MPS2_PLUS_MEM_SYSTEM_RAM) &&
			((int)(addr) <= MPS2_PLUS_MEM_SYSTEM_RAM + MPS2_PLUS_MEM_SYSTEM_RAM_LEN))
			|| (((unsigned long)(addr) >= MPS2_PLUS_MEM_DMA_BASE_ADDR) &&
			((int)(addr) <= MPS2_PLUS_MEM_DMA_BASE_ADDR + MPS2_PLUS_MEM_DMA_AREA_LEN))) {
		return 0;
	}
	TEST_PRINTF("NOT in DMA range \r\n");
	return 1;
}
#elif defined(MPS2) && defined(SSE_200)
#if defined TZM
/*
 * This function checks if an address is in the Secure region.
 */
static int isInRange_s(void *addr)
{
	if (((unsigned long)(addr) >=  MPS2_PLUS_MEM_S_DMA_BASE_ADDR
		&& (unsigned long)(addr) <= MPS2_PLUS_MEM_S_DMA_BASE_ADDR
		+ MPS2_PLUS_MEM_S_DMA_BASE_ADDR_LEN)) {
		return 0;
	}
	TEST_PRINTF("NOT in Secure DMA range \r\n");
	return 1;
}

/*
 * This function checks if an address is in the Non-Secure region.
 */
static int isInRange(void *addr)
{
	if (((unsigned long)(addr) >= MPS2_PLUS_MEM_NS_DMA_BASE_ADDR
		&& (unsigned long)addr <= MPS2_PLUS_MEM_NS_DMA_BASE_ADDR
		+ MPS2_PLUS_MEM_NS_DMA_BASE_ADDR_LEN)) {
		return 0;
	}
	TEST_PRINTF("NOT in Non-Secure DMA range \r\n");
	return 1;
}
#else
static int isInRange(void *addr)
{
	if (((unsigned long)(addr) >=  MPS2_PLUS_MEM_S_DMA_BASE_ADDR
		&& (unsigned long)(addr) <= MPS2_PLUS_MEM_S_DMA_BASE_ADDR
		+ MPS2_PLUS_MEM_S_DMA_BASE_ADDR_LEN)) {
		return 0;
	}
	TEST_PRINTF("NOT in DMA range \r\n");
	return 1;
}
#endif
#endif

/*******************************************************************************
 *
 * Tests
 *
 ******************************************************************************/

int Test_Memory(void)
{
	int size = 10000000;
	void *addr = NULL;
	void *readdr = NULL;

#ifdef Zynq
	size = 20000000;
#elif defined(MPS2) && defined(SSE_200)
#ifdef TZM
	int s_size = 800000;
	size = s_size*2;
	void *s_addr = NULL;
	void *s_readdr = NULL;
#else
	size = 800000;
#endif
#endif

	/*
	alloc test: allocate DMA buffer and check if it's allocated in range

	NOTICE: the size variable in SSE_200 + trust zone is the size of the
	NON-SECURE buffer, while s_size is the size of the SECURE buffer.
	As well, s_addr and s_readdr are the start addresses of the secure
	buffers, while addr and readdr are non-secure.
	*/
	readdr = Test_PalDMAContigBufferAlloc(size);

	if ((readdr == NULL) || ((isInRange(readdr)) == 1)) {
		TEST_PRINTF_ERROR("Failed to allocate the buffer, readdr.\r\n");
		goto error;
	}
	TEST_PRINTF("Managed to allocate the buffer, readdr:"
			" 0x%lx-0x%lx new size: %d \r\n",
			readdr, (int)readdr+size, size);
#ifdef TZM
	s_readdr = Test_PalDMAContigBufferAlloc_s(s_size);

	if ((s_readdr == NULL) || ((isInRange_s(s_readdr)) == 1)) {
		TEST_PRINTF_ERROR("Failed to allocate secure buffer, s_readdr.\r\n");
		goto error;
	}
	TEST_PRINTF("Managed to allocate secure buffer, s_readdr:"
			" 0x%lx-0x%lx new size: %d \r\n",
			s_readdr, (int)s_readdr+s_size, s_size);
#endif

	/*
	free test: free the block allocated before, readdr
	The buffer readdr have to be freed before allocating the next
	buffer, addr. readdr is 10MB/150MB/1.6MB (MPS2+/Zynq/SSE200)
	and addr is 10MB/150MB/1.6MB (MPS2+/Zynq/SSE200) as well,
	max continuous DMA buffer is 14MB/240MB/2MB (MPS2+/Zynq/SSE200),
	So if addr allocation fails, it's an indication that there's no
	sufficient memory => readdr free failed
	*/

	Test_PalDMAContigBufferFree(readdr);

	addr = (char *) Test_PalDMAContigBufferAlloc(size);
	if (addr == NULL) {
		TEST_PRINTF_ERROR("Failed to free the buffer, readdr.\r\n");
		goto error;
	}
	TEST_PRINTF("Managed to allocate the buffer, addr:"
			" 0x%lx-0x%lx new size: %d \r\n",
			addr, (int)addr+size, size);

	TEST_PRINTF("Managed to free the buffer, readdr.\r\n");

#ifdef TZM

	/*
		free test: free the block allocated before, s_readdr (secure)
		The buffer s_readdr have to be freed before allocating the next
		buffer, s_addr. s_readdr is 0.8MB (SSE200, in secure region)
		and s_addr is 0.8M as well,
		max continuous DMA buffer is 1MB (SSE200 secure region size),
		So if s_addr allocation fails, it's an indication that there's no
		sufficient memory => s_readdr free failed
		*/

		Test_PalDMAContigBufferFree_s(s_readdr);

		s_addr = (char *) Test_PalDMAContigBufferAlloc_s(s_size);
		if (s_addr == NULL) {
			TEST_PRINTF_ERROR("Failed to free secure buffer, s_readdr.\r\n");
			goto error;
		}
		TEST_PRINTF("Managed to allocate secure buffer, s_addr:"
				" 0x%lx-0x%lx new size: %d \r\n",
				s_addr, (int)s_addr+s_size, s_size);

		TEST_PRINTF("Managed to free secure buffer, s_readdr.\r\n");


#endif

	/*
	realloc test - size down: reallocate previous buffer (addr) to new
	smaller buffer size and check if the value written in the buffer addr
	is copied to the reallocated buffer, readdr
	*/

	strcpy(addr, "This is a test!");
	TEST_PRINTF("This is what's written in the allocated buffer,"
			" addr, before reallocating it: %s \r\n", addr);
	size = ((size)/6);
	readdr = Test_PalDMAContigBufferRealloc(addr, size);

	if ((readdr == NULL) || ((isInRange(readdr)) == 1)) {
		TEST_PRINTF_ERROR("Failed to reallocate-down the buffer,"
				" addr: 0x%lx with size: %d \r\n", addr, size);
		goto error;
	}
	if (strcmp(readdr, "This is a test!") != 0) {
		TEST_PRINTF_ERROR("Failed to reallocate-down the buffer,"
				" readdr: 0x%lx because the value in the previous"
				" address hasn't been copied correctly to the new"
				" reallocated buffer \r\n", readdr);
		goto error;
	}
	TEST_PRINTF("Managed to reallocate-down the buffer, addr, new "
			"buffer - readdr: 0x%lx-0x%lx new size: %d \r\n"
			, readdr, (int)readdr+size, size);

#ifdef TZM
		strcpy(s_addr, "This is a test!");
		TEST_PRINTF("This is what's written in the allocated buffer,"
				" s_addr, before reallocating it: %s \r\n", s_addr);
		s_size = ((s_size)/6);
		s_readdr = Test_PalDMAContigBufferRealloc_s(s_addr, s_size);

		if ((s_readdr == NULL) || ((isInRange_s(s_readdr)) == 1)) {
			TEST_PRINTF_ERROR("Failed to reallocate-down secure buffer,"
					" s_addr: 0x%lx with size: %d \r\n", s_addr, s_size);
			goto error;
		}
		if (strcmp(s_readdr, "This is a test!") != 0) {
			TEST_PRINTF_ERROR("Failed to reallocate-down secure buffer,"
					" s_readdr: 0x%lx because the value in the previous"
					" address hasn't been copied correctly to the new"
					" reallocated buffer \r\n", s_readdr);
			goto error;
		}
		TEST_PRINTF("Managed to reallocate-down secure buffer, s_addr,"
				" new buffer - s_readdr: 0x%lx-0x%lx new size: %d \r\n"
				, s_readdr, (int)s_readdr+s_size, s_size);
#endif

	/*
	realloc tests - size UP: reallocate previous buffer (readdr) to
	new bigger buffer size.
	*/

	size = size*2;
	addr = Test_PalDMAContigBufferRealloc(readdr, size);

	if ((addr == NULL) || ((isInRange(addr)) == 1)) {
		TEST_PRINTF_ERROR("Failed to reallocate-up the buffer, readdr:"
				" 0x%lx with size: %d \r\n", readdr, (size));
		goto error;
	}
	if (strcmp(addr, "This is a test!") != 0) {
		TEST_PRINTF_ERROR("Failed to reallocate-up the buffer, addr:"
				" 0x%lx because the value in the previous address "
				"hasn't been copied correctly to the new reallocated"
				" buffer \r\n", addr);
		goto error;
	}
	TEST_PRINTF("Managed to reallocate-up buffer, readdr,"
			" new buffer - addr: 0x%lx-0x%lx new size: %d \r\n",
			addr, (int)addr+size, size);

	Test_PalDMAContigBufferFree(addr);

#ifdef TZM
	s_size = s_size*2;
	s_addr = Test_PalDMAContigBufferRealloc_s(s_readdr, s_size);

	if ((s_addr == NULL) || ((isInRange_s(s_addr)) == 1)) {
		TEST_PRINTF_ERROR("Failed to reallocate-up secure buffer,"
				" s_ readdr: 0x%lx with size: %d \r\n",
				s_readdr, (s_size));
		goto error;
	}
	if (strcmp(s_addr, "This is a test!") != 0) {
		TEST_PRINTF_ERROR("Failed to reallocate-up secure buffer,"
				" s_addr: 0x%lx because the value in the previous"
				" address hasn't been copied correctly to the new"
				" reallocated buffer \r\n", s_addr);
		goto error;
	}
	TEST_PRINTF("Managed to reallocate-up secure buffer, s_readdr,"
			" new buffer - s_addr: 0x%lx-0x%lx new size: %d \r\n",
			s_addr, (int)s_addr+s_size, s_size);

	Test_PalDMAContigBufferFree_s(s_addr);
#endif

	TEST_PRINTF_MESSAGE("Memory test result: SUCCESS \r\n");
	return 0;
error:
	TEST_PRINTF_MESSAGE("Memory test result: FAILED \r\n");
	return 1;
}
