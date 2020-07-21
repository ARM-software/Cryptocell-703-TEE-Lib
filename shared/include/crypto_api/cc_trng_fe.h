/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#ifndef _CC_TRNG_FE_H
#define _CC_TRNG_FE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This file contains API and definitions for generating TRNG buffer in full entropy mode.
*/

 /*!
 @addtogroup trng_fe_apis
 @{
 */

#include "cc_pal_types.h"

/************************ Defines ******************************/

/*! Minimum entropy size in bits. */
#define CC_TRNG_MIN_ENTROPY_SIZE    0
/*! Maximal entropy size in bits. */
#define CC_TRNG_MAX_ENTROPY_SIZE    8192


/****************************************************************************************/
/*!
 @brief The function returns an entropy buffer in the requested size.
 @return \c CC_OK on success.
 @return A non-zero value from cc_trng_error.h on failure.

*/
CCError_t CC_TrngEntropyGet(
			/*![in] The required entropy size in bits.Size must be bigger than CC_TRNG_MIN_ENTROPY_SIZE, and smaller than CC_TRNG_MAX_ENTROPY_SIZE. */
							size_t    entropySizeBits,
							 /*! [out] Pointer to the entropy buffer. */
                            uint8_t   *pOutEntropy,
							/*![in] The entropy buffer size in bytes. The size must be big enough to hold the required entropySizeBits. */
                            size_t    outEntropySizeBytes);

#ifdef __cplusplus
}
#endif
/*!
@}
 */
#endif


