/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#ifndef __CC_HAL_H__
#define __CC_HAL_H__


/*!
@file
@brief This file contains HAL definitions and APIs.
*/

#include <stdint.h>
#include <stdio.h>

#include "cc_hal_plat.h"
#include "cc_hal_defs.h"
#include "cc_pal_types.h"
#include "cc_pal_types_plat.h"

#define CC_HAL_ALL_BITS     0xFFFFFFFFUL

typedef enum CCHalClear_t {
    CC_HAL_NO_CLEAR,
    CC_HAL_CLEAR,
    CC_HAL_CLEAR_RESERVE = 0x7fffffffUL
} CCHalClear_t;

typedef struct CCHalMsg_t {
    uint32_t param;
    uint32_t error;
} CCHalMsg_t;

/*!
 * @brief   This function is used to map ARM TrustZone CryptoCell TEE registers to Host virtual address space.
	        It is called by ::CC_LibInit, and returns a non-zero value in case of failure.
            The existing implementation supports Linux environment. In case virtual addressing is not used, the function can be minimized to contain only the
	        following line, and return OK:
            gCcRegBase = (uint32_t)CC_BASE_CC;

  @return   CC_OK on success.
  @return   CCError_t error code.
*/
CCError_t CC_HalInit(void);

/*!
 * @brief This function clears the DSCRPTR_COMPLETION bit in the ICR signal.
 */
void CC_HalClearInterrupt(uint32_t data);


/*!
 * @brief   This function is used to unmap ARM TrustZone CryptoCell TEE registers' virtual address.
 *	        It is called by CC_LibFini, and returns a non-zero value in case of failure.
 *	        In case virtual addressing is not used, the function can be minimized to be an empty function returning OK.
 *
 * @return  CCError_t error code.
 */
CCError_t CC_HalTerminate(void);

/*!
 * @brief   This function is used to clear the interrupt vector.

 * @return  void.
 */
void CC_HalClearInterrupt(uint32_t clearMask);

/*!
 * @brief This function is used to mask IRR interrupts.

 * @return void.
 */
void CC_HalMaskInterrupt(uint32_t irqMask);

/*!
 * @brief   This function notifies for any ARM CryptoCell interrupt
 *          A blocking function that waits for the completions of an interrupt of type irqType
 *          For certain values of irqType, irqData will be returned.
 *          Whether irqData is returned or not is implementations pecific.
 *
 * @param   irqType       the type of interrupt to wait for, as defined in CCHalIrq_t
 * @param   irqData       returned data (optionally) dependent on IRQ type
 *
 * @return                CCError_t error code.
 */
CCError_t CC_HalWaitInterrupt(CCHalIrq_t irqType, uint32_t *irqData);

/*!
 * @brief   A non blocking function that reads the IRR register once and optionally clears the relevant bit.
 *          pIsOn will hols whether the bit was asserted
 *
 * @param   irqType       the type of interrupt to wait for, as defined in CCHalIrq_t
 * @param   isClear       should attempt to clear interrupt in case it was detected.
 * @param   pIrrValue     the read value of the IRR register
 *
 * @return                CCError_t error code.
 */
CCError_t CC_HalInterruptSample(CCHalIrq_t irqType, CCHalClear_t clear, uint32_t *pIrrValue);

/*!
 * @brief   ISR handler for CryptoCell interrupts
 *
 * @param   index
 * @param   args
 *
 * @return              CCError_t error code.
 */
CCError_t CC_HalIsr(uint32_t index, void *args);

#endif

