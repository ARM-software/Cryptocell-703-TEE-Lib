#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <selftest_mem.h>
#include <selftest_thread.h>
#include <selftest_semphr.h>
#include <selftest_socket.h>

#include "board_configs.h"
#include "test_pal_log.h"
#include "test_pal_cli.h"

/*******************************************************************************
 *
 * Function prototypes
 *
 ******************************************************************************/

#ifdef freertos

int StartTestFromCLI(char *pcWriteBuffer, size_t xWriteBufferLen,
		     const char *pcCommandString);
static int Start_Test(void);

#endif

/*******************************************************************************
 *
 * Variables
 *
 ******************************************************************************/

#ifdef freertos

struct Test_PalCliCommand selftest_cli_registartion = {
	"run-selftest",
	"\r\nrun-selftest: \r\n Run selftest\r\n",
	&StartTestFromCLI,
	0
};

#endif

/*******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/

#ifdef freertos

int libselftest_initLib(void)
{
	int rc = 0;

	rc += Test_PalCLIRegisterCommand(&selftest_cli_registartion);
	return rc;
}

int StartTestFromCLI(char *pcWriteBuffer, size_t xWriteBufferLen,
		const char *pcCommandString)
{
	(void)pcWriteBuffer;
	(void)xWriteBufferLen;
	(void)pcCommandString;

	Start_Test();  /* test's function! */

	return 0;
}

#endif

/*******************************************************************************
 *
 * Tests
 *
 ******************************************************************************/

#ifdef linux

int main(void)
{
	TEST_PRINTF("Before Test_HalBoardInit\r\n");
	Test_HalBoardInit();
	int rc = 0;

	rc |= Test_Memory();
	rc |= Test_Threads();
	rc |= Test_Semphr();
	rc |= Test_Socket();

	if (rc == 1) {
		Test_HalBoardFree();
		return rc;
	}

	Test_HalBoardFree();
	return rc;
}

#elif defined(freertos)

static int Start_Test(void)
{
	int rc = 0;
	TEST_PRINTF("Before Test_HalBoardInit\r\n");
	Test_HalBoardInit();

	rc |= Test_Memory();
	rc |= Test_Threads();
	rc |= Test_Semphr();

	if (rc == 1) {
		Test_HalBoardFree();
		return rc;
	}

	Test_HalBoardFree();
	return rc;
}

#endif
