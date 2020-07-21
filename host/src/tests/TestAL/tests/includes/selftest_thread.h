#ifndef SELFTEST_THREAD_H_
#define SELFTEST_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
* @brief This function tests the APIs of test_pal_thread.h in TestAL
* create 2 threads - threadOne, threadTwo.
* in its function, threadOne writes the values 0-9 to an array,
* and main thread is waiting on Test_PalThreadJoin(threadOne),
* if all values 0-9 are wrriten in resArr, it means that Test_PalThreadJoin worked properly.
* in threadTwo's functions, it writes the values 0-4 in the array desArr, then main thread
* calls Test_PalThreadDestroy(threadTwo), if array's indices 5-9 are 0, then destroy succeed,
* else, destroy failed becaused uhreadTwo continued writing to the array.
* @param[in]
*
* @param[out]
*
* @return - 0 for success, 1 for failure.
*/
int Test_Threads(void);

#endif /* SELFTEST_THREAD_H_ */
