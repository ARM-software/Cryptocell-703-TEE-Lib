#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "test_pal_mem.h"
#include "test_pal_log.h"
#include "test_pal_semphr.h"
#include "test_pal_thread.h"
#include "test_pal_time.h"

/*******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/

#define DELAY		1000
#define ARRAY_SIZE		10

/*******************************************************************************
 *
 * Global Variables
 *
 ******************************************************************************/

int arrOne[ARRAY_SIZE];
int arrTwo[ARRAY_SIZE];
int threadTwoVar;
int wait;
ThreadHandle threadOne;
ThreadHandle threadTwo;

/*******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/

static void *joinFunc(void *args)
{
	int j;
	(void)args;

	for (j = 0; j < ARRAY_SIZE; j++) {
		arrOne[j] = j;
		TEST_PRINTF("ThreadOne write: %d to index %d"
				" in address: 0x%lx \r\n",
				arrOne[j], j, &arrOne[j]);
	}
	return 0;
}

static void *desrtoyFunc(void *args)
{
	(void)args;

	for (threadTwoVar = 0; threadTwoVar < ARRAY_SIZE; threadTwoVar++) {
		if (threadTwoVar == (ARRAY_SIZE/2)){
			while(wait == 1){
				Test_PalDelay(DELAY*300);
			}
		}
		arrTwo[threadTwoVar] = threadTwoVar;
		TEST_PRINTF("ThreadTwo write %d to index %d"
				" in address: 0x%lx \r\n",
				arrTwo[threadTwoVar], threadTwoVar, &arrTwo[threadTwoVar]);
	}
	return 0;
}

/*******************************************************************************
 *
 * Tests
 *
 ******************************************************************************/

int Test_Threads(void)
{
	int arg1 = 1;
	int arg2 = 1;
	void *arg;
	int j;

	/*
	make sure all values are set to 0 in order to make sure that
	threadTwo was destroyed when j=4 (the value in indices 5-9
	should be 0, meaning threadTwo was destroyed properly)
	*/

	memset(arrOne, 0, sizeof(int)*ARRAY_SIZE);
	memset(arrTwo, 0, sizeof(int)*ARRAY_SIZE);
	wait = 1;

	TEST_PRINTF("Before Test_PalThreadCreate\r\n");

	threadOne = Test_PalThreadCreate(
				Test_PalGetMinimalStackSize(),
				&joinFunc,
				3, &arg1,
				"One",
				3, 0);
	threadTwo = Test_PalThreadCreate(
				Test_PalGetMinimalStackSize(),
				&desrtoyFunc,
				3, &arg2,
				"Two",
				3, 0);

	if (threadOne == NULL || threadTwo == NULL) {
		TEST_PRINTF_ERROR("Failed to create a thread\r\n");
		goto error;
	}

	TEST_PRINTF("Hello before thread one join\r\n");
	/*
	test Test_PalThreadJoin (returned value is 0 on success)
	*/
	if (Test_PalThreadJoin(threadOne, &arg) != 0) {
		TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin()\r\n");
		goto error;
	}
	TEST_PRINTF("Hello before thread one destroy\r\n");
	/*
	destroy threadOne
	*/
	if (Test_PalThreadDestroy(threadOne) != 0) {
		TEST_PRINTF_ERROR("Failed to destroy thread one\r\n");
		goto error;
	}

	while (threadTwoVar < (ARRAY_SIZE/2)) {
		Test_PalDelay(DELAY);
	}

	TEST_PRINTF("Hello before thread two destroy\r\n");

	/*
	destroy threadTwo while it performs its function (k=ARRAY_SIZE/2)
	wait = 0, in order to release threadTwo from a blocking while loop
	*/
	wait = 0;
	if (Test_PalThreadDestroy(threadTwo) != 0) {
		TEST_PRINTF_ERROR("Failed to destroy thread two\r\n");
		goto error;
	}

	TEST_PRINTF("After thread two destroy\r\n");

	for (j = 0; j < ARRAY_SIZE; j++) {
		/*
		check if all the values 0-9 were written correctly to the
		array arrOne by threadOne (join waited for threadOne to
		finish writing, hence, join worked properly)
		*/
		if (arrOne[j] != j) {
			TEST_PRINTF_ERROR("Join failed to perform"
					" correctly\r\n");
			goto error;
		}
		/*
		Check if the value 0 is written in indices 5-9 in the
		array arrTwoay Meaning that threadTwo was destroyed properly
		(when j=4 in it's function)
		*/
		if ((j >= (ARRAY_SIZE/2)) && (arrTwo[j] != 0)) {
			TEST_PRINTF_ERROR("Failed to destroy a thread \r\n");
			goto error;
		}
	}

	TEST_PRINTF_MESSAGE("Threads test result: SUCCESS \r\n");
	return 0;
error:
	TEST_PRINTF_MESSAGE("Threads test result: FAILED \r\n");
	return 1;
}
