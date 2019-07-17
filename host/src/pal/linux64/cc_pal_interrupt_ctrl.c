/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


/************* Include Files *************************************************/

#include "cc_pal_types.h"
#include "cc_pal_interrupt_ctrl_plat.h"
#include "cc_regs.h"
#include "cc_registers.h"
#include "cc_hal.h"
#include "dx_reg_base_host.h"

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/************************ Defines ********************************************/
#define SEM_MAX_DEPTH   1

/************************ Enums **********************************************/

/************************ Typedefs *******************************************/
typedef struct CC_Completion_t {
    sem_t empty;
    sem_t full;
    CCHalMsg_t msg;
} CC_Completion_t;

/************************ Extern *********************************************/

/************************ Global Data ****************************************/
pthread_t   palThreadId;
uint32_t    palThreadRc;
bool        palThreadExit = false;
bool        suspendIrqPolling = false;

CCPalISR    pIsr = NULL;

CC_Completion_t xCompletion[CC_HAL_IRQ_MAX];
/************************ Private Functions **********************************/
void *palIrqThread(void *params)
{
    CC_UNUSED_PARAM(params);

    while (!palThreadExit) {
        if (!suspendIrqPolling) {

            if (pIsr != NULL) {
                pIsr();
            }
        }
        usleep(100);    // wait 100 milisecond
    }
    palThreadRc = 0;
    pthread_exit(&palThreadRc);
}

/************************ Public Functions ***********************************/
/**
 * @brief
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return - CC_SUCCESS for success, CC_FAIL for failure.
 */
CCError_t CC_PalInitIrq(CCPalISR pIsrFunction)
{
    uint32_t rc;
    uint32_t i = 0;

    /* initialize the completion databases */
    for (i = 0; i < CC_HAL_IRQ_MAX; i++) {
        sem_init(&xCompletion[i].empty, 0, SEM_MAX_DEPTH);
        sem_init(&xCompletion[i].full, 0, 0);
        xCompletion[i].msg.param = 0;
        xCompletion[i].msg.error = CC_OK;
    }

    palThreadExit = false;

    if (pIsrFunction == NULL)
    {
        return CC_FAIL;
    }

    rc = pthread_create(&palThreadId, NULL, palIrqThread, NULL);
    if (rc != 0) {
        return rc;
    }

    pIsr = pIsrFunction;

    // join will be in the termination function
    return CC_SUCCESS;
}

/**
 * @brief This function removes the interrupt handler for
 * cryptocell interrupts.
 *
 */
void CC_PalFinishIrq(void)
{
    void **threadRet = NULL;
    uint32_t i = 0;

    for (i = 0; i < CC_HAL_IRQ_MAX; i++) {
        sem_destroy(&xCompletion[i].empty);
        sem_destroy(&xCompletion[i].full);
    }

    palThreadExit = true; // The fips thread checks this flag and act accordingly
    pthread_join(palThreadId, threadRet);
}

/*
 * When using Polling to test the interrupts.
 *
 */
#if defined(CC_CONFIG_INTERRUPT_POLLING)
CCError_t CC_PalInterruptRead(CCHalIrq_t irqType, CCHalMsg_t *pMsg)
{
    CCError_t error = CC_OK;
    uint32_t irrValue = 0;

    CC_UNUSED_PARAM(pMsg);

    do {
        error = CC_HalInterruptSample(irqType, CC_HAL_CLEAR, &irrValue);
    } while (error == CC_OK && irrValue == 0);

    pMsg->param = irrValue;

    if (error != CC_OK) {
        pMsg->error = CC_FAIL;
        return CC_FAIL;
    }

    return CC_OK;
}

CCError_t CC_PalInterruptNotify(CCHalIrq_t irqType, CCHalMsg_t *pMsg)
{
    CC_UNUSED_PARAM(irqType);
    CC_UNUSED_PARAM(pMsg);

    return CC_OK;
}

#elif defined(CC_CONFIG_INTERRUPT_POLLING_THREAD)
CCError_t CC_PalInterruptRead(CCHalIrq_t irqType, CCHalMsg_t *pMsg)
{
    if (irqType >= CC_HAL_IRQ_MAX) {
        return CC_FAIL;
    }

    sem_wait(&xCompletion[irqType].full);

    *pMsg = xCompletion[irqType].msg;

    sem_post(&xCompletion[irqType].empty);

    if (pMsg->error != CC_OK) {
        return CC_FAIL;
    }

    return CC_OK;
}

CCError_t CC_PalInterruptNotify(CCHalIrq_t irqType, CCHalMsg_t *pMsg)
{
    if (irqType >= CC_HAL_IRQ_MAX) {
        return CC_FAIL;
    }

    sem_wait(&xCompletion[irqType].empty);

    xCompletion[irqType].msg = *pMsg;

    sem_post(&xCompletion[irqType].full);

    return CC_OK;
}
#else
#error "Interrupt method is not supported for this OS and product"
#endif /* CC_CONFIG_INTERRUPT_POLLING */

