/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#ifndef _CC_INT_GENERAL_DEFS_H
#define _CC_INT_GENERAL_DEFS_H

/*!
@file
@brief This file contains internal general definitions.
*/

#ifdef __cplusplus
extern "C"
{
#endif

/************************ Macros ******************************/

/* check if fatal error bit is set to ON - NA for CC7X */
#define CC_IS_FATAL_ERR_ON(rc)\
do {\
       rc = CC_FALSE;\
}while(0)

/* The DCU word that holds the virtual DCU bits used for reset over-ride and SDER */
    // IG - move to another place in bsv
#define CC_DCU_VIRTUAL_WORD_OFFSET                              3

/*
 * the optimization threshold. buffers smaller than this value
 * will be copied into a continuous temporary buffer and sent to the crypto engine using DLLI.
 * The allows to save time by not building the MLLI table.
 **/
#define DLLI_OPTIMIZED_BUFF_SIZE                                128

#ifdef __cplusplus
}
#endif

#endif



