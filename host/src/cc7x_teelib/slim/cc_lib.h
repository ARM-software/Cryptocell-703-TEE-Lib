/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

/*!
@file
@brief This file contains all of the CryptoCell library initiation and finish
       APIs, enums, and definitions.
*/

#ifndef __CC_LIB_H__
#define __CC_LIB_H__

#include "cc_pal_types.h"
#include "cc_chinese_cert.h"
#include "cc_cert_ctx.h"
#include "cc_axi_ctrl.h"

/*! CryptoCell library return codes. */
typedef enum
{
    /*! A success indication. */
    CC_LIB_RET_OK = 0,
    /*! Invalid parameters. */
    CC_LIB_RET_EINVAL,
    /*! Error in adapter modules initialization. */
    CC_LIB_RET_COMPLETION,
    /*! Error in Hardware Adaption Layer initialization. */
    CC_LIB_RET_HAL,
    /*! Invalid peripheral ID. */
    CC_LIB_RET_EINVAL_PIDR,
    /*! Invalid component ID. */
    CC_LIB_RET_EINVAL_CIDR,
    /*! Reserved. */
    CC_LIB_RET_RND_INST_ERR,
    /*! Invalid HW version. */
    CC_LIB_RET_EINVAL_HW_VERSION,
    /*! Invalid HW signature. */
    CC_LIB_RET_EINVAL_HW_SIGNATURE,
    /*! Error in Platform Adaption Layer initialization. */
    CC_LIB_RET_PAL,
    /*! Mismatched HW and SW products - SW is CryptoCell-703, but HW is not. */
    CC_LIB_INCORRECT_HW_VERSION_SLIM_VS_FULL,
    /*! Error setting the cache parameters due to invalid input parameter. */
    CC_LIB_RET_CACHE_PARAMS_ERROR,
    /*! Chinese certification tests error. */
    CC_LIB_RET_ECHCERT,
    /*! Reserved. */
    CC_LIB_RESERVE32B = 0x7FFFFFFFL
}CClibRetCode_t  ;


/*!
@brief This function performs global initialization of the Arm
CryptoCell TEE runtime library;
it must be called once per cold boot cycle. As part of the global initialization
the function verifies that
all of the cryptographic engines are working as expected by running known answer
tests. If a test fails (the function
returns an error), it signifies that there is a fatal error, which must be
handled accordingly.
\note Unlike the other APIs in the library, this API is not thread-safe. \par
@return \c CC_LIB_RET_OK on success.
@return A non-zero value in case of failure.
*/

CClibRetCode_t CC_LibInit(bool isChCertSupport, CCCertKatContext_t  *pCertCtx, CCAxiFields_t  *pAxiFields);

/*!
@brief This function finalizes the library operations. It calls HAL and PAL
terminate functions.
@return \c CC_LIB_RET_OK on success.
@return A non-zero value in case of failure.
*/
void CC_LibFini(void);

#endif /*__CC_LIB_H__*/


