/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from ARM Limited or its affiliates.      *
*   (C) COPYRIGHT [2001-2017] ARM Limited or its affiliates.                   *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from ARM Limited or its affiliates.                                          *
*******************************************************************************/

#ifndef TEST_PAL_THREAD_H_
#define TEST_PAL_THREAD_H_

/*!
 @file
 @brief This file contains the PAL thread integration tests.
 */

/*!
 @addtogroup pal_thread_test
 @{
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *ThreadHandle; /*!< Thread handle*/

/******************************************************************************/
/*!
 * @brief This function returns the minimal stack size in bytes.
 *
 *
 * @return Minimal stack size in bytes.
 */
size_t Test_PalGetMinimalStackSize(void);

/******************************************************************************/
/*!
 * @brief This function returns the highest thread priority.
 *
 *
 * @return Highest thread priority.
 */
uint32_t Test_PalGetHighestPriority(void);

/******************************************************************************/
/*!
 * @brief This function returns the lowest thread priority.
 *
 *
 * @return Lowest thread priority.
 */
uint32_t Test_PalGetLowestPriority(void);

/******************************************************************************/
/*!
 * @brief This function returns the default thread priority.
 *
 *
 * @return Default thread priority.
 */
uint32_t Test_PalGetDefaultPriority(void);

/******************************************************************************/
/*!
 * @brief This function creates a thread. The user should call
 * Test_PalThreadJoin() in order to wait until the thread ends and then to
 * Test_PalThreadDestroy() in order to free resources.
 * In case of a thread without an end, the user should not call
 * Test_PalThreadJoin() which will not return. Instead, the user should call
 * Test_PalThreadDestroy() which will cancel the thread and free
 * its resources.
 *
 *
 * @return Thread handle address on success
 * @return NULL on failure.
 */
ThreadHandle Test_PalThreadCreate(
 size_t stackSize, /*!< Thread stack size in bytes. The allocated stack size
 will be greater from stackSize and the minimal stack size.*/
 void *(*threadFunc)(void *), /*!< Thread function. The function shall return
 a pointer to the returned value or NULL. In case TZM is supported,
 this function must have the same security attribute as TestAL's (either secure
 or non-secure).*/
 int priority, /*!< Thread priority. Highest and lowest priorities can be
 received by calling Test_PalGetLowestPriority() and
 Test_PalGetHighestPriority() accordingly.*/
 void *args, /*!< Function input arguments.*/
 const char *threadName, /*!< Thread name. Not in use for Linux.*/
 uint8_t nameLen,/*!< Thread name length. Not in use for Linux.*/
 uint8_t dmaAble /*!< Determines whether the stack should be DMA-able (true).*/
);

/******************************************************************************/
/*!
 * @brief This function waits for a thread to terminate (BLOCKING).
 * If that thread has already terminated it returns immediately.
 *
 *
 * Note that threadRet is not changed, yet \c threadRet is changed and
 * can be NULL. Therefore, do not try to access \c threadRet without
 * checking that \c threadRet is not NULL.
 *
 *
 * @return 0 on success
 * @return 1 on failure.
 */
uint32_t Test_PalThreadJoin(
 ThreadHandle threadHandle, /*!< Thread structure.*/
 void **threadRet /*!< A pointer to the returned value of the target thread.*/
);

/******************************************************************************/
/*!
 * @brief This function destroys a thread (if it's still running) and frees
 * its resources.
 * In order to free thread resources only after thread's end this function
 * should be called after Test_PalThreadJoin().
 * In order to cancel the thread immediately and free its resources, this
 * function should be called alone (without Test_PalThreadJoin()), which
 *  must eventually be called in any case.
 * Note that this function does not deallocate the memory that the
 * thread itself allocates. This needs to be done by the thread itself.
 *
 *
 * @return 0 on success
 * @return 1 on failure.
 */
uint32_t Test_PalThreadDestroy(
 ThreadHandle threadHandle /*!< Thread structure.*/
);

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TEST_PAL_THREAD_H_ */
