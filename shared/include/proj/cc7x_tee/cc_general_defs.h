/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#ifndef _CC_GENERAL_DEFS_H
#define _CC_GENERAL_DEFS_H

/*!
@file
@brief This file contains general definitions.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include "cc_hash_defs.h"

/************************ Defines ******************************/
typedef struct {
    uint16_t hashResultSize;
    CCHashOperationMode_t hashMode;
}HmacHash_t;

#define CC_HASH_NAME_MAX_SIZE  10

extern const HmacHash_t HmacHashInfo_t[CC_HASH_NumOfModes];
extern const uint8_t HmacSupportedHashModes_t[CC_HASH_NumOfModes];

/* general definitions */
/*-------------------------*/
#define CC_AES_KDR_MAX_SIZE_BYTES	32
#define CC_AES_KDR_MAX_SIZE_WORDS	(CC_AES_KDR_MAX_SIZE_BYTES/sizeof(uint32_t))


/* Life cycle state definitions */
#define CC_LCS_CHIP_MANUFACTURE_LCS		0x0 /*!< Life cycle CM value. */
#define CC_LCS_DEVICE_MANUFACTURE_LCS		0x1 /*!< Life cycle DM value. */
#define CC_LCS_SECURITY_DISABLED_LCS		0x3 /*!< Life cycle security disabled value. */
#define CC_LCS_SECURE_LCS			0x5 /*!< Life cycle secure value. */
#define CC_LCS_RMA_LCS				0x7 /*!< Life cycle RMA value. */


#ifdef __cplusplus
}
#endif

#endif



