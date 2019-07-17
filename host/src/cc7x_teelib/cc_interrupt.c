/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */
#include "cc_regs.h"
#include "cc_hal_plat.h"
#include "cc_pal_types.h"
#include "cc_pal_log.h"
#include "cc_cpp.h"
#include "cc_pal_interrupt_ctrl_plat.h"

/******************************************************************************
*               DEFINITIONS
******************************************************************************/
#define CC_CPP_MASK         CC_REG_BIT_MASK(HOST_RGF, IRR_REE_KS_OPERATION_INDICATION)
#define CC_AXIM_MASK        CC_REG_BIT_MASK(HOST_RGF, IRR_AXIM_COMP_INT)
#define CC_RNG_MASK         CC_REG_BIT_MASK(HOST_RGF, IRR_RNG_INT)

/******************************************************************************
*               EXTERNS
******************************************************************************/
extern void CC_CppEventHandler(void);

/******************************************************************************
*               FUNCTIONS
******************************************************************************/

/*!
 * This function is part of the cc7x tee driver.
 * This function is meant to be executaed in a seperate thread in a loop.
 * When working with CC_CONFIG_INTERRUPT_POLLING_THREAD, AXIM_COMP and RNG_INT interrupt are handled by this thread.
 * When working with other interrrupt modes, the two interrupts mentioned above will not be handled by this function, but by the driver context
 * CPP interrupts SHOULD be handled this this function (in a seperate thread).
 */
void CC_InterruptHandler(void)
{
    uint32_t irrValue = 0;
    uint32_t imrValue = 0;
#ifndef CC_CONFIG_INTERRUPT_POLLING
    CCHalMsg_t msg = { 0 };
#endif /* CC_CONFIG_INTERRUPT_POLLING */

    irrValue = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IRR));
    imrValue = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR));

    if (irrValue & (~imrValue)) {
        if (irrValue & CC_CPP_MASK) {
            /* Mask interrupt - will be unmasked in Deferred service handler */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR), imrValue | CC_CPP_MASK);

            /* clear interrupt - must be before processing events */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), CC_CPP_MASK);

            CC_CppEventHandler();
        }

        /*
         * In this product we are not using other interrupt bits other than:
         *      REE_KS_OPERATION_INDICATION
         *      AXIM_COMP_INT
         *      AXI_ERR
         *      RNG_INT
         */
#ifndef CC_CONFIG_INTERRUPT_POLLING

        if (irrValue & CC_AXIM_MASK) {
            msg.param = irrValue;

            /* test for AXI error */
            if (CC_REG_FLD_GET(0, HOST_RGF_IRR, AXI_ERR_INT, irrValue) == CC_TRUE) {

                /* clearing bus error */
                CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR),
                                      CC_REG_BIT_MASK(HOST_RGF_ICR, AXI_ERR_CLEAR));

                msg.error = CC_FAIL;
            }

            /* clear interrupts */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), CC_AXIM_MASK);

            /* notify the blocked thread about the interrupt */
            /* axim does not need to read amy following data */
            CC_PalInterruptNotify(CC_HAL_IRQ_AXIM_COMPLETE, &msg);
        }

        if (irrValue & CC_RNG_MASK) {
            msg.error = 0;

            /* clear interrupt value */
            msg.param = CC_HAL_READ_REGISTER(CC_REG_OFFSET(RNG, RNG_ISR));
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(RNG, RNG_ICR), CC_HAL_ALL_BITS);

            /* clear interrupts */
            CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), CC_RNG_MASK);

            /* signal completion to blocked thread */
            CC_PalInterruptNotify(CC_HAL_IRQ_RNG, &msg);
        }
#endif /* CC_CONFIG_INTERRUPT_POLLING */
    }

}

