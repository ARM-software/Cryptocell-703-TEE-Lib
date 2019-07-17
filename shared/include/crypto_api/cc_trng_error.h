/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#ifndef _CC_TRNG_ERROR_H
#define _CC_TRNG_ERROR_H

#include "cc_error.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*!
@file
@brief This file contains the definitions of the CryptoCell TRNG errors.
*/


/*!
 @addtogroup cc_trng_error
 @{
	 */




/************************ Defines ******************************/
/*! TRNG module on the CryptoCell layer base address - 0x00F02F00 */
/*! Illegal input parameters.*/
#define CC_TRNG_INVALID_PARAMS_ERROR        	(CC_TRNG_MODULE_ERROR_BASE + 0x0UL)

#ifdef __cplusplus
}
#endif
/*!
@}
 */
#endif


