/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm's non-OSI source license
 *
 */


/************* Include Files *************************************************/
#include "cc_pal_types.h"
#include "cc_pal_interrupt_ctrl.h"
#include "cc_hal.h"

/************************ Defines ********************************************/

/************************ Enums **********************************************/

/************************ Typedefs *******************************************/

/************************ Global Data ****************************************/

/************************ Private Functions **********************************/

/************************ Public Functions ***********************************/
CCError_t CC_PalInitIrq(CCPalISR pIsrFunction)
{
    CC_UNUSED_PARAM(pIsrFunction);

    return CC_OK;
}

void CC_PalFinishIrq(void)
{
    return;
}

CCError_t CC_PalInitWaitInterruptComp(CCHalIrq_t irqType)
{
    CC_UNUSED_PARAM(irqType);

    return CC_OK;
}

CCError_t CC_PalFinishWaitInterruptComp(CCHalIrq_t irqType)
{
    CC_UNUSED_PARAM(irqType);

    return CC_OK;
}

CCError_t CC_PalWaitInterruptComp(CCHalIrq_t irqType, uint32_t *irqData)
{
    CC_UNUSED_PARAM(irqType);
    CC_UNUSED_PARAM(irqData);

    return CC_OK;
}

CCError_t CC_PalInterruptNotify(CCHalIrq_t irqType, uint32_t irrData)
{
    CC_UNUSED_PARAM(irqType);
    CC_UNUSED_PARAM(irrData);

    return CC_OK;
}
