#ifndef SELFTEST_SEMPHR_H_
#define SELFTEST_SEMPHR_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
* @brief This function tests the APIs of test_pal_semphr.h in TestAL
*
* Binary semaphore test:
* create 5 threads, in their function each of them locks the mutex - pMutexId,
* and then, takes the binary semaphore - Test_PalBinarySemaphoreTake(pSemphrId),
* and waits for a notification.
* Main thread does Test_PalBinarySemaphoreGive(&pSemphrId), and releases the thread
* that waits for a notification on the semaphore in that time.
* This thread then continues with the code, increases smphrVar by 1 and release the mutex.
* if smphrVar is 5, because all 5 threads increased it, then the APIs worked properly.
*
* Mutex test:
* create 5 threads, in their function each of them locks the mutex - pMutexId,
* increases mutexVar by 1 twenty times (so by 20 overall) and unlocks the mutex.
* Main thread is waiting on Test_PalThreadJoin() for all threads, then destroys them.
* if mutexVar is 100, because all 5 threads increased it 20 times, then the APIs worked properly.
* else, the mutex failed because more than one thread were able to enter the critical
* section between Test_PalMutexLock and Test_PalMutexUnlock and change mutexVar.
*
* @param[in]
*
* @param[out]
*
* @return - 0 for success, 1 for failure.
*/
int Test_Semphr(void);

#endif /* SELFTEST_SEMPHR_H_ */
