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

#include <string.h>
#include <stdbool.h>

#include "test_pal_thread.h"
#include "test_proj_common.h"
#include "tests_log.h"

#define LINUX_MIN_THREAD_STACK_SIZE (1024 * 1024)
/*
 * Tests_Runthread
 * Run a function in a thread.
 * threadFunc [in]- function to run
 * args [in] - arguments to the function
 *
 * return value is -1 if thread creation, join, or destruction didn't work,
 * otherwise - value returned from running the function threadFunc.
 *
 * */
int Tests_Runthread(void *(*threadFunc)(void *), void *args)
{

    int threadRc;
    ThreadHandle threadHandle;
    uint32_t rc = 0;

    threadHandle = Test_PalThreadCreate(LINUX_MIN_THREAD_STACK_SIZE,
                                        threadFunc,
                                        Test_PalGetDefaultPriority(),
                                        args,
                                        NULL,
                                        0,
                                        true);
    if (threadHandle == NULL) {
        TEST_LOG_ERROR("Test_PalThreadCreate failed\n");
        return -1;
    }
    threadRc = Test_PalThreadJoin(threadHandle, (void *) &rc);
    if (threadRc != 0) {
        TEST_LOG_ERROR("Test_PalThreadJoin failed\n");
        return -1;
    }

    threadRc = Test_PalThreadDestroy(threadHandle);
    if (threadRc != 0) {
        TEST_LOG_ERROR("Test_PalThreadDestroy failed\n");
        rc = -1;
    }

    TEST_LOG_TRACE("Finished running thread. Returned rc = %d\n", rc);

    return rc;
}
