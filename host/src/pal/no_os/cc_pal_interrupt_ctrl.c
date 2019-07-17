/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


/************* Include Files *************************************************/
#include "cc_pal_types.h"
#include "cc_pal_interrupt_ctrl_plat.h"
#include "cc_hal.h"

#if defined(CC_CONFIG_INTERRUPT_POLLING)
/************************ Defines ********************************************/

/************************ Enums **********************************************/

/************************ Typedefs *******************************************/

/************************ Global Data ****************************************/

/************************ Private Functions **********************************/

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
CCError_t CC_PalInitIrq(void)
{
    return CC_SUCCESS;
}

/**
 * @brief This function removes the interrupt handler for
 * cryptocell interrupts.
 *
 */
void CC_PalFinishIrq(void)
{
}

CCError_t CC_PalInterruptRead(CCHalIrq_t irqType, CCHalMsg_t *pMsg)
{
    CCError_t error = CC_OK;
    uint32_t irrValue = 0;

    do {
        error = CC_HalInterruptSample(irqType, CC_HAL_CLEAR, &irrValue);
    } while (error == CC_OK && irrValue == 0);

    pMsg->param = irrValue;
    pMsg->error = error;

    if (error != CC_OK) {
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
#else
#error "Interrupt method is not supported for this OS and product"
#endif /* CC_CONFIG_INTERRUPT_POLLING */
