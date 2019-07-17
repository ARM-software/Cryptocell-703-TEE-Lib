/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

/*!
@file
@brief This file contains all the enums and definitions that are used for the
        CryptoCell Lib init and finish APIs, as well as the APIs themselves.
*/

#ifndef __CC_LIB_H__
#define __CC_LIB_H__

#include "cc_pal_types.h"
#include "cc_chinese_cert.h"
#include "cc_cert_ctx.h"
#include "cc_axi_ctrl.h"

typedef enum {
	CC_LIB_RET_OK = 0,
	SA_SILIB_RET_ENODEV,      /* Device not opened or does not exist */
	SA_SILIB_RET_EINTERNAL,   /* Internal driver error (check system log)*/
	SA_SILIB_RET_ENOTSUP,     /* Unsupported function/option */
	SA_SILIB_RET_ENOPERM,     /* Not enough permissions for request */
	SA_SILIB_RET_EINVAL,      /* Invalid parameters */
	SA_SILIB_RET_HW_Q_INIT,
	SA_SILIB_RET_COMPLETION,
	CC_LIB_RET_HAL,
	CC_LIB_RET_EINVAL_PIDR,
	CC_LIB_RET_EINVAL_CIDR,
	SA_SILIB_RET_ASYM_ERR,
	CC_LIB_RET_RND_INST_ERR,
	CC_LIB_RET_EINVAL_HW_VERSION,    /* Invalid HW version */
	CC_LIB_RET_EINVAL_HW_SIGNATURE,  /* Invalid HW signature */
	CC_LIB_RET_PAL,
	CC_LIB_INCORRECT_HW_VERSION_SLIM_VS_FULL,       /* Mismatched HW/SW flavor - SW flavor is SLIM, but HW isn't */
	CC_LIB_RET_CACHE_PARAMS_ERROR,
    SA_SILIB_RET_ECHCERT,		/* Chinese certification tests error */
    CC_LIB_RESERVE32B = 0x7FFFFFFFL
} CClibRetCode_t;


/*!
@brief This function Perform global initialization of the ARM TrustZone
CryptoCell TEE runtime library;
it must be called once per cold boot cycle. As part of the global initialization
the function verifies that
all the cryptographic engines are working as expected by running known answer
tests. If a test fails (the function
returns an error), it signifies that there is a fatal error, and it should be
handled accordingly.
\note Unlike the other APIs in the library, this API is not thread-safe. \par
@return CC_LIB_RET_OK on success.
@return A non-zero value in case of failure.
*/

CClibRetCode_t CC_LibInit(bool isChCertSupport, CCCertKatContext_t  *pCertCtx, CCAxiFields_t  *pAxiFields);

/*!
@brief This function finalizes the library operations. It calls hal and pal
terminate functions.
@return CC_LIB_RET_OK on success.
@return A non-zero value in case of failure.
*/
void CC_LibFini(void);

#endif /*__CC_LIB_H__*/

