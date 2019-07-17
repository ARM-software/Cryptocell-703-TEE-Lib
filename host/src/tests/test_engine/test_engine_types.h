/****************************************************************************
* The confidential and proprietary information contained in this file may   *
* only be used by a person authorised under and to the extent permitted     *
* by a subsisting licensing agreement from Arm Limited (or its affiliates). *
*     (C) COPYRIGHT [2018] Arm Limited (or its affiliates).                 *
*         ALL RIGHTS RESERVED                                               *
* This entire notice must be reproduced on all copies of this file          *
* and copies of this file may only be made by a person if such person is    *
* permitted to do so under the terms of a subsisting license agreement      *
* from Arm Limited (or its affiliates).                                     *
*****************************************************************************/

#ifndef TEST_ENGINE_TYPES_H_
#define TEST_ENGINE_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

/****************************************************************************
 *
 * macros
 *
 ****************************************************************************/
/** macros to construct vectors */
#define TE_TEST_VEC(_a) \
                { .pData=_a, .count=TE_VEC_COUNT(_a) }

#define TE_VEC_SIZE(_a) \
                (sizeof(*(_a)))

#define TE_VEC_COUNT(_a) \
                (sizeof(_a)/TE_VEC_SIZE(_a))

/****************************************************************************
 *
 * Types
 *
 ****************************************************************************/

/**
 * Test engine return codes
 */
typedef enum TE_rc_t {
    TE_RC_SUCCESS,    //!< TE_RC_SUCCESS
    TE_RC_FAIL    //!< TE_RC_FAIL
} TE_rc_t;

/**
 * A single test vector that will be passed to the fAlgo function
 */
typedef struct TE_TestVec_t {

    /** name of the vector to identify it in case it fails */
    const char *name;

    /** generic data that the user provided. */
    void *pData;

} TE_TestVec_t;

typedef uint32_t TE_TestRc_t;

typedef struct TE_TestVecList_t {

    /** a list of TE_TestVec_t to iterate over */
    TE_TestVec_t *pData;

    /** amount of elements in the vector list */
    size_t count;
} TE_TestVecList_t;

/****************************************************************************
 *
 * the callback prototype that will be supplied by the user
 *
 ****************************************************************************/

/**
 * callback to the function that prepares the environment towards the test
 *
 * @param pContext      A pointer to the context that was provided during the registration.
 *
 * @return              TE_RC_SUCCESS or any other user specific error
 */
typedef TE_rc_t (*fPrepare)(void *pContext);

/**
 * callback to the function that performs the actual test
 *
 * @param pContext      A pointer to the context that was provided during the registration.
 *
 * @return              TE_RC_SUCCESS or any other user specific error
 */
typedef TE_rc_t (*fExecFlow)(void *pContext);

/**
 * callback to the function that performs the actual test.
 * The test will be performed for-each of the test vectors in pVecResult
 *
 * @param pContext      A pointer to the context that was provided during the registration.
 * @param pVecResult    A pointer to the vector list to test.
 *
 * @return              TE_RC_SUCCESS or any other user specific error
 */
typedef TE_rc_t (*fExecSuite)(TE_TestVec_t *pTestVec, TE_TestRc_t *pVecResult);

/**
 * callback to the function that performs the verification of the test result.
 *
 * @note                This test is run only when using Test of type Flow.
 *
 * @param pContext      A pointer to the context that was provided during the registration.
 *
 * @return              TE_RC_SUCCESS or any other user specific error
 */
typedef TE_rc_t (*fVerify)(void *pContext);

/**
 * callback to the function that performs the verification of the test result.
 *
 * @note                This test is run only when using Test of type Flow.
 *
 * @param pContext      A pointer to the context that was provided during the registration.
 *
 * @return              TE_RC_SUCCESS or any other user specific error
 */
typedef TE_rc_t (*fClean)(void *pContext);

#endif /* TEST_ENGINE_TYPES_H_ */
