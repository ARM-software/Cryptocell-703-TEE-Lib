/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#define CC_PAL_LOG_CUR_COMPONENT CC_LOG_MASK_CCLIB

#include "cc_regs.h"
#include "cc_pal_memmap.h"
#include "cc_hal.h"
#include "cc_registers.h"
#include "cc_pal_abort.h"
#include "cc_error.h"
#include "cc_pal_types_plat.h"
#include "cc_pal_interrupt_ctrl_plat.h"
/******************************************************************************
*				DEFINITIONS
******************************************************************************/
#define DX_CC_REG_AREA_LEN 0x100000

/******************************************************************************
*				GLOBALS
******************************************************************************/
unsigned long gCcRegBase = 0;

/* used to map the mask of relevant interrupts */
uint32_t irqCachedMask[CC_HAL_IRQ_MAX] = { 0 };

/******************************************************************************
*				PRIVATE FUNCTIONS
******************************************************************************/

/******************************************************************************
*				FUNCTIONS
******************************************************************************/
CCError_t CC_HalInit(void)
{
#ifndef CMPU_UTIL
    unsigned long *pVirtBuffAddr = NULL;
    CC_PalMemMap(CC_BASE_CC, DX_CC_REG_AREA_LEN, (uint32_t**) &pVirtBuffAddr);
    gCcRegBase = (unsigned long) pVirtBuffAddr;
#endif

    irqCachedMask[CC_HAL_IRQ_AXIM_COMPLETE] = CC_REG_BIT_MASK(HOST_RGF_IMR, AXIM_COMP_INT_MASK);
    irqCachedMask[CC_HAL_IRQ_RNG] = CC_REG_BIT_MASK(HOST_RGF_IMR, RNG_INT_MASK);

    return CC_OK;
}

CCError_t CC_HalTerminate(void)
{
#ifndef CMPU_UTIL
    CC_PalMemUnMap((uint32_t *) gCcRegBase, DX_CC_REG_AREA_LEN);
    gCcRegBase = 0;
#endif

    return CC_OK;
}

void CC_HalClearInterrupt(uint32_t data)
{
	if (0 == data) {
		CC_PalAbort("CC_HalClearInterrupt illegal input\n");
	}

	/* clear interrupt */
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), data);

	return;
}

void CC_HalMaskInterrupt(uint32_t data)
{
    CC_HAL_WRITE_REGISTER( CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR), data);

    return;
}


CCError_t CC_HalWaitInterrupt(CCHalIrq_t irqType, uint32_t *irqData)
{
    CCError_t error = CC_OK;
    CCHalMsg_t msg = { 0 };

    switch (irqType) {
        case CC_HAL_IRQ_AXIM_COMPLETE:

            error = CC_PalInterruptRead(CC_HAL_IRQ_AXIM_COMPLETE, &msg);
            if (error != CC_OK) {
                return CC_FAIL;
            }

            break;
        case CC_HAL_IRQ_RNG:

            if (irqData == NULL) {
                CC_PAL_LOG_ERR("irqData is NULL\n");
                return CC_FAIL;
            }

            error = CC_PalInterruptRead(CC_HAL_IRQ_RNG, &msg);
            if (error != CC_OK) {
                return CC_FAIL;
            }

            /*
             * When using irq_handler to handle interrupts, then handler must be the one to reset the
             * RNG_ISR and RGF_IRR registers, so to avoid calling the handler again right after leaveing the initial call.
             * When using polling mode, the blocked context is in charge of the reading and clearing of the
             * interupt registers.
             */
#if defined(CC_CONFIG_INTERRUPT_IRQ) || defined(CC_CONFIG_INTERRUPT_POLLING_THREAD)
            *irqData = msg.param;
#elif defined(CC_CONFIG_INTERRUPT_POLLING)
            /* read RNG_ISR to pass it to blocked task */
            *irqData = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, RNG_ISR));

            /* clear the RNG_ISR value to be able to clear RNG_INT bit in RGF_IRR */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), CC_HAL_ALL_BITS);

            /* can only be cleared when RNG_ISR is 0 */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), irqCachedMask[CC_HAL_IRQ_RNG]);
#endif /* CC_CONFIG_INTERRUPT_IRQ || CC_CONFIG_INTERRUPT_POLLING_THREAD */
            break;
        default:
            return CC_FAIL;
    }

    return CC_OK;
}

#if defined(CC_CONFIG_INTERRUPT_IRQ)
/*!
 * ISR handler for CryptoCell interrupts.
 * Clears the IRR
 * In case of RND interrupt, also send RNG_ISR data.
 * User should be aware to implement this handler with correct access to CC registers (IRR, RNG_ISR).
 **/
CCError_t CC_HalIsr(uint32_t index, void *args)
{
    CCError_t error = CC_OK;
    uint32_t irrValue = 0;
    uint32_t clearMask = 0;
    CCHalMsg_t msg = { 0 };

    CC_UNUSED_PARAM(index);
    CC_UNUSED_PARAM(args);

    /* this is a common handler, so we need to test for all relevant interrupts */
    irrValue = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IRR));

    /* AXIM_COMPLETE interrpt */
    if (irrValue & irqCachedMask[CC_HAL_IRQ_AXIM_COMPLETE]) {
        msg.error = 0;
        msg.param = irrValue;

        clearMask = irqCachedMask[CC_HAL_IRQ_AXIM_COMPLETE];

        if (CC_REG_FLD_GET(0, HOST_RGF_IRR, AXI_ERR_INT, irrValue) == CC_TRUE) {
            msg.error = error;

            /*set data for clearing bus error*/
            clearMask |= CC_REG_BIT_MASK(HOST_RGF_ICR, AXI_ERR_CLEAR);
        }

        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), clearMask);

        /* Unblock the task waiting to be notified that the CryptoCell operation has ended. */
        CC_PalInterruptNotify(CC_HAL_IRQ_AXIM_COMPLETE, &msg);
    }

    /* RNG cannot be reset until RNG_ISR is cleared */
    if (irrValue & irqCachedMask[CC_HAL_IRQ_RNG])
        msg.error = error;

        /* read RNG_ISR to pass it to blocked task */
        msg.param = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, RNG_ISR));

        /* clear the RNG_ISR value to be able to clear RNG_INT bit in RGF_IRR */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), CC_HAL_ALL_BITS);

        /* can only be cleared when RNG_ISR is 0 */
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), irqCachedMask[CC_HAL_IRQ_RNG]);

        /* notify blocked task */
        CC_PalInterruptNotify(CC_HAL_IRQ_RNG, &msg);
    }

    return CC_OK;
}
#endif /* CC_CONFIG_INTERRUPT_IRQ */

CCError_t CC_HalInterruptSample(CCHalIrq_t irqType, CCHalClear_t clear, uint32_t *pIrrValue)
{
    CCError_t error = CC_OK;
    volatile uint32_t irr = 0;
    volatile uint32_t clearMask = 0;
    uint32_t intMask = 0;

    if (irqType >= CC_HAL_IRQ_MAX) {
        return CC_FAIL;
    }

    intMask = irqCachedMask[irqType];

    if (pIrrValue == NULL) {
        return CC_FAIL;
    }

    *pIrrValue = 0;

    /* sample IRR register */
    irr = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IRR));

    /* test for AHB_ERR */
    if (irqType == CC_HAL_IRQ_AXIM_COMPLETE && CC_REG_FLD_GET(0, HOST_RGF_IRR, AXI_ERR_INT, irr) == CC_TRUE) {
        error = CC_FAIL;

        /*set data for clearing bus error*/
        CC_REG_FLD_SET(HOST_RGF, HOST_RGF_ICR, AXI_ERR_CLEAR, clearMask, 1);
    }

    /* clear interrupt bits*/
    if (clear == CC_HAL_CLEAR && (intMask & irr)) {
        clearMask |= intMask;
    }

    /* not an error */
    if (irr & intMask) {
        *pIrrValue = irr;
    }

    if (clearMask) {
        CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), clearMask);
    }

    return error;
}

