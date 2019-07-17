/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#ifndef _CC_PAL_INTERRUPTCTRL_H
#define _CC_PAL_INTERRUPTCTRL_H

#include "cc_hal.h"

/**
 * @brief This function removes the interrupt handler for
 * cryptocell interrupts.
 *
 */
void CC_PalFinishIrq(void);

/*!
 * @brief       A function that is called to block on waiting interrupt. This is OS dependant.
 *
 * @param irqType       The irq to wait on
 * @param pMsg          Optional paramter to receive from the notifing context
 *
 * @return              CC_OK for success, CC_FAIL for failure.
 */
CCError_t CC_PalInterruptRead(CCHalIrq_t irqType, CCHalMsg_t *pMsg);

/*!
 * @brief       A function that notifies a blocking context that an interrupt was detected.
 *
 * @param irqType       The irq to notify about
 * @param pMsg          Optional paramter to pass to the waiting thread.
 *                      The msg should be passed by value, so it is op to pass messages from stack.
 *                      Must be NULL
 *                      pMsg should contain a field indicating whether an error was detected while expecting interrupt.
 *
 * @return              CC_OK for success, CC_FAIL for failure.
 */
CCError_t CC_PalInterruptNotify(CCHalIrq_t irqType, CCHalMsg_t *pMsg);

#endif /* _CC_PAL_INTERRUPTCTRL_H */

