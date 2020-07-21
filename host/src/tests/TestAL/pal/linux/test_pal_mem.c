/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "test_pal_log.h"
#include "test_pal_mem.h"
#include "test_pal_map_addrs.h"

/* malloc.c allocator - user definitions */
#define ONLY_MSPACES	1
#define USE_DL_PREFIX	1
#define HAVE_MORECORE	0
#define HAVE_MMAP	0
#define HAVE_MREMAP	0
#define USE_LOCKS	1

/* malloc.c is compiled only as part of test_pal_mem.c */
#include "malloc.c"

mspace msp;
static unsigned long DMABaseAddr = 0;
static unsigned long unmanagedBaseAddr = 0;
static uint8_t memInitialised = 0;

/******************************************************************************/
void *Test_PalMalloc(size_t size)
{
	if (!size)
		return NULL;

	return malloc(size);
}

/******************************************************************************/
void Test_PalFree(void *pvAddress)
{
	if (pvAddress == NULL)
		return;

	free(pvAddress);
}

/******************************************************************************/
void *Test_PalRealloc(void *pvAddress, size_t newSize)
{
	if (pvAddress == NULL)
		return NULL;

	return realloc(pvAddress, newSize);
}

/******************************************************************************/
void *Test_PalDMAContigBufferAlloc(size_t size)
{
	return mspace_malloc(msp, size);
}

/******************************************************************************/
void Test_PalDMAContigBufferFree(void *pvAddress)
{
	mspace_free(msp, pvAddress);
}

/******************************************************************************/
void *Test_PalDMAContigBufferRealloc(void *pvAddress, size_t newSize)
{
	return mspace_realloc(msp, pvAddress, newSize);
}

/******************************************************************************/
unsigned long Test_PalGetDMABaseAddr(void)
{
	return DMABaseAddr;
}

/******************************************************************************/
unsigned long Test_PalGetUnmanagedBaseAddr(void)
{
	return unmanagedBaseAddr;
}

/******************************************************************************/
uint32_t Test_PalMemInit(unsigned long newDMABaseAddr,
			 unsigned long newUnmanagedBaseAddr,
			 size_t DMAsize)
{
	if(memInitialised) {
		TEST_PRINTF_ERROR("Memory is already initialised");
		return 1;
	}

	msp = create_mspace_with_base((void *)newDMABaseAddr, DMAsize, 1);

	if (!msp) {
		TEST_PRINTF_MESSAGE("create_mspace_with_base failed");
		return 1;
	}

	TEST_PRINTF_MESSAGE("create_mspace_with_base success \n");
	DMABaseAddr = newDMABaseAddr;
	unmanagedBaseAddr = newUnmanagedBaseAddr;
	memInitialised = 1;

	return 0;
}

/******************************************************************************/
uint32_t Test_PalMemFin(void)
{
	destroy_mspace(msp);

	DMABaseAddr = 0;
	unmanagedBaseAddr = 0;
	memInitialised = 0;

	return 0;
}
