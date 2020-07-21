#ifndef SELFTEST_MEM_H_
#define SELFTEST_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief This function tests the APIs of test_pal_mem.h in TestAL
 * Allocation test - allocate buffer, check returned value (success/failure),
 * check if the returned address is in DMA range.
 * Free test - free a buffer after allocation.
 * Realloc test - reallocate a buffer to a bigger size and then to a smaller size.
 * @param[in]
 *
 * @param[out]
 *
 * @return - 0 for success, 1 for failure.
 */
int Test_Memory(void);

#endif /* SELFTEST_MEM_H_ */
