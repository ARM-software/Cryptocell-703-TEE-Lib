/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#ifndef __CC_HAL_PLAT_H__
#define __CC_HAL_PLAT_H__

#include "cc_registers.h"
#include "cc_bitops.h"

/******************************************************************************
*				DEFINITIONS
******************************************************************************/
#define CC_LARGE_SECRET_KEY_NUM_OF_BYTES 32
#define CC_SMALL_SECRET_KEY_NUM_OF_BYTES 16

/******************************************************************************
*                               MACROS
******************************************************************************/
extern unsigned long gCcRegBase;

/******************************************************************************
*                               MACROS
******************************************************************************/

/*!
 * Read CryptoCell memory-mapped-IO register.
 *
 * \param regOffset The offset of the ARM CryptoCell register to read
 * \return uint32_t Return the value of the given register
 */
#define CC_HAL_READ_REGISTER(regOffset) 				\
		(*((volatile uint32_t *)(gCcRegBase + (regOffset))))

/*!
 * Write CryptoCell memory-mapped-IO register.
 * \note This macro must be modified to make the operation synchronous, i.e. the write operation must complete,
 *       and the new value must be written to the register before the macro returns. The mechanisms required to
 *       achieve this are architecture-dependent (e.g., the memory barrier in ARM architecture).
 *
 * \param regOffset The offset of the Arm CryptoCell register to write
 * \param val The value to write
 */
#define CC_HAL_WRITE_REGISTER(regOffset, val) 		\
		(*((volatile uint32_t *)(gCcRegBase + (regOffset))) = (uint32_t)(val))



#endif /*__CC_HAL_PLAT_H__*/

