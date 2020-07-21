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

#define DELAY		10000
#define THREADS_NUMBER		5

/*******************************************************************************
 *
 * Global Variables
 *
 ******************************************************************************/

int mutexVar = 0;
int smphrVar = 0;
Test_PalMutex pMutexId;
Test_PalBinarySemaphore pSemphrId;

/*******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/

static void mutualFunc(void)
{
	int i;
	int j;

	TEST_PRINTF("I'm taking.... \r\n");
	Test_PalMutexLock(&pMutexId, INFINITE);
	TEST_PRINTF("Took! \r\n");
	Test_PalDelay(DELAY);
	for (i = 0; i < 20; i++) {
		j = mutexVar + 1;
		Test_PalDelay(DELAY);
		mutexVar = j;
		TEST_PRINTF("mutexVar is: %d\r\n", mutexVar);
	}
	TEST_PRINTF("mutexVar = %d\n", mutexVar);
	TEST_PRINTF("I'm giving.... \r\n");
	Test_PalMutexUnlock(&pMutexId);
	TEST_PRINTF("Gave! \r\n");
}

static void mutualFuncSemphr(void)
{
	int i;
	int j;

	TEST_PRINTF("I'm taking binary semphr.... \r\n");
	Test_PalBinarySemaphoreTake(&pSemphrId, INFINITE);
	TEST_PRINTF("Took Semphr! \r\n");
	Test_PalDelay(DELAY);
	for (i = 0; i < 20; i++) {
		j = smphrVar + 1;
		Test_PalDelay(DELAY);
		smphrVar = j;
		TEST_PRINTF("smphrVar is: %d\r\n", smphrVar);
	}
	TEST_PRINTF("smphrVar = %d\n", smphrVar);
}

static void *threadfunc(void *args)
{
	(void)args;
	mutualFunc();
	return 0;
}

static void *threadfuncSemphr(void *args)
{
	(void)args;
	mutualFuncSemphr();
	return 0;
}

/*******************************************************************************
 *
 * Tests
 *
 ******************************************************************************/

static int Test_Mutex(void)
{
	int args = 1;
	int i;
	void *arg;
	ThreadHandle threads[THREADS_NUMBER];

	if (Test_PalMutexCreate(&pMutexId) != 0) {
		TEST_PRINTF_ERROR("Failed to create a mutex\r\n");
		goto error;
	}

	TEST_PRINTF("Before Test_PalThreadCreate\r\n");

	for (i = 0; i < THREADS_NUMBER; i++) {
		threads[i] = Test_PalThreadCreate(
				Test_PalGetMinimalStackSize(),
				&threadfunc,
				3, &args,
				"old",
				3, 0);
		if (threads[i] == NULL) {
			TEST_PRINTF_ERROR("Failed to create a thread\r\n");
			goto error;
		}
	}

	for (i = 0; i < THREADS_NUMBER; i++) {
		TEST_PRINTF("Hello before thread join %d\r\n", i);
		if (Test_PalThreadJoin(threads[i], &arg) != 0) {
			TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin\r\n");
			goto error;
		}
	}

	for (i = 0; i < THREADS_NUMBER; i++) {
		TEST_PRINTF("Hello before thread destroy %d\r\n", i);
		if (Test_PalThreadDestroy(threads[i]) != 0) {
			TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin\r\n");
			goto error;
		}
	}

	if (Test_PalMutexDestroy(&pMutexId) != 0) {
		TEST_PRINTF_ERROR("Failed to destroy a mutex\r\n");
		goto error;
	}

	if (mutexVar != 100) {
		TEST_PRINTF_ERROR("Mutex doesn't work correctly. did not manage"
				" to protect critical section \r\n");
		goto error;
	}

	TEST_PRINTF_MESSAGE("Mutex test result: SUCCESS \r\n");
	return 0;
error:
	TEST_PRINTF_MESSAGE("Mutex test result: FAILED \r\n");
	return 1;
}

static int Test_Semaphore(void)
{
	int args = 1;
	int i, j;
	void *arg;
	ThreadHandle threadsSemphr[THREADS_NUMBER];

	if (Test_PalBinarySemaphoreCreate(&pSemphrId) != 0) {
		TEST_PRINTF_ERROR("Failed to create a Semaphore\r\n");
		goto error;
	}

	TEST_PRINTF("Before Test_PalThreadCreate\r\n");

	for (i = 0; i < THREADS_NUMBER; i++) {
		threadsSemphr[i] = Test_PalThreadCreate(
				Test_PalGetMinimalStackSize(),
				&threadfuncSemphr,
				3, &args,
				"old",
				3, 0);
		if (threadsSemphr[i] == NULL) {
			TEST_PRINTF_ERROR("Failed to create a thread\r\n");
			goto error;
		}
	}

	for (i = 0; i < THREADS_NUMBER; i++) {
		Test_PalDelay(DELAY*30);
		TEST_PRINTF("Giving Semaphore... %d \r\n", i);
		Test_PalBinarySemaphoreGive(&pSemphrId);
		TEST_PRINTF("Gave Semaphore! %d \r\n", i);
	}

	Test_PalDelay(DELAY*30);

	for (j = 0; j < THREADS_NUMBER; j++) {
		TEST_PRINTF("Hello before thread join %d\r\n", j);
		if (Test_PalThreadJoin(threadsSemphr[j], &arg) != 0) {
			TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin\r\n");
			goto error;
		}
	}

	for (j = 0; j < THREADS_NUMBER; j++) {
		TEST_PRINTF("Hello before thread destroy %d\r\n", j);
		if (Test_PalThreadDestroy(threadsSemphr[j]) != 0) {
			TEST_PRINTF_ERROR("Failed to Test_PalThreadJoin\r\n");
			goto error;
		}
		TEST_PRINTF("After Thread Destroy %d\n", j);
	}


	if (Test_PalBinarySemaphoreDestroy(&pSemphrId) != 0) {
		TEST_PRINTF_ERROR("Failed to destroy a Semaphore\r\n");
		goto error;
	}

	if (smphrVar != 100) {
		TEST_PRINTF_ERROR("Semaphore failed to work correctly. \r\n");
		goto error;
	}

	TEST_PRINTF_MESSAGE("Semaphore test result: SUCCESS \r\n");
	return 0;
error:
	TEST_PRINTF_MESSAGE("Semaphore test result: FAILED \r\n");
	return 1;
}

int Test_Semphr(void)
{
	if (Test_Mutex() == 1 || Test_Semaphore() == 1)
		return 1;
	return 0;
}
