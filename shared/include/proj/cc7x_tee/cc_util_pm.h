/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#ifndef  CC_UTIL_PM_H
#define  CC_UTIL_PM_H

/*!
@file
@brief This file contains power management definitions and APIs.
*/

#include "cc_util_error.h"


/*! Get ARM Cerberus status. */
#define CC_STATUS_GET 	0	/* no active registered CC operations */

/*! Notify ARM Cerberus is active. */
#define CC_IS_WAKE 	0 	/* Do Nothing, return without error */

/*! Notify ARM Cerberus is idle. */
#define CC_IS_IDLE 	0 	/* Do Nothing, return without error */


/************************************************************************************/
/****************        Power management API           *****************************/
/************************************************************************************/

/****************************************************************************************/
/**
 *
 * @brief This function should be called by user before the ARM TEE is being power down.
 *
 * @return CC_UTIL_OK on success.
 * @return A non-zero value on failure.
 */
CCUtilError_t CC_PmSuspend(void);


/****************************************************************************************/
/**
 *
 * @brief This function should be called by user once restoring the ARM TEE from power down state,
 * 	before any cryptographic operation.
 *
 * @return CC_UTIL_OK on success.
 * @return A non-zero value on failure.
 */
CCUtilError_t CC_PmResume(void);


#endif
