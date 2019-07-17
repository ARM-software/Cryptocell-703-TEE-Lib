/****************************************************************************
 * The confidential and proprietary information contained in this file may    *
 * only be used by a person authorised under and to the extent permitted      *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates).    *
 * 	(C) COPYRIGHT [2001-2019] Arm Limited (or its affiliates).	     *
 *	    ALL RIGHTS RESERVED						     *
 * This entire notice must be reproduced on all copies of this file           *
 * and copies of this file may only be made by a person if such person is     *
 * permitted to do so under the terms of a subsisting license agreement       *
 * from Arm Limited (or its affiliates).					     *
 *****************************************************************************/

#ifndef _TEST_PROJ_OTP_PLAT_H__
#define _TEST_PROJ_OTP_PLAT_H__

#include <stdint.h>
#include "cc_pal_types.h"
#include "cc_otp_defs.h"

/*!
  @file
  @brief This file contains definitions and APIs for the tests OTP interface.
         the Apis here must have a specific implementation for the integrated platform.
 */


 /*!
 @addtogroup otp_apis
 @{
 */
/******************************************************************
 * Defines
 ******************************************************************/
/*! Enumeration definition chip state indication. */
typedef enum {
    PROJ_OTP_CHIP_STATE_NOT_INITIALIZED = 0, /*!< Chip state not initialized. */
    PROJ_OTP_CHIP_STATE_TEST = 1,           /*!< Chip state for test chip. */
    PROJ_OTP_CHIP_STATE_PRODUCTION = 2,     /*!< Chip state for production chip. */
    PROJ_OTP_CHIP_STATE_ERROR = 3,          /*!< Chip state error indication. */
    PROJ_OTP_CHIP_STATE_RESERVED = CC_MAX_UINT32_VAL, /*!< Reserved value. */
} ProjOtp_ChipState_t;


/*! Enumeration definition chip state indication. */
typedef enum {
    PROJ_OTP_RMA_NO = 0,        /*!< No RMA mode. */
    PROJ_OTP_RMA_ICV = 1,       /*!< Only ICV RMA bit is set. */
    PROJ_OTP_RMA_OEM = 2,       /*!< Only OEM RMA bit is set. */
    PROJ_OTP_RMA_FULL = 3,      /*!< Both OEM and ICV RMA bits are set. */
    PROJ_OTP_RMA_RESERVED = CC_MAX_UINT32_VAL,  /*!< Reserved value. */
} ProjOtp_Rma_t;


/*! Defines the memory offset used for OTP region.
 *  \note You must implement OTP access that is compatible with your system.
 */
#define ENV_OTP_START_OFFSET        0x2000UL

/*! Defines OTP word write.
 *   \note You must implement OTP write that is compatible with your system.
 */
#define TEST_WRITE_OTP_BY_ENV(wordOffset, val) \
        TEST_WRITE_ENV_REG(ENV_OTP_START_OFFSET + ((wordOffset)*CC_32BIT_WORD_SIZE), val)

/*! Defines OTP word read.
 *   \note You must implement OTP write that is compatible with your system.
 */
#define TEST_READ_OTP_BY_ENV(wordOffset) \
        TEST_READ_ENV_REG(ENV_OTP_START_OFFSET + ((wordOffset)*CC_32BIT_WORD_SIZE))


/******************************************************************
 * Externs
 ******************************************************************/

/*! The random OTP mask (a representation of the mask that is located in the RTL used for OTP masking).
 * CryptoCell FPGA implementation includes two different values,
 * you can put the same value in both definitions. */
/*!
 *   \note You must implement the OTP masking that is compatible with your system.
 */
extern uint32_t gTestOtpMaskV1[];
extern uint32_t gTestOtpMaskV2[];



/******************************/
/*   function declaration     */
/*****************************/

/*!
@brief This function reads a word from the OTP using environment (test dedicated) registers.
*  \note You must replace TEST_READ_OTP_BY_ENV() macro with implementation that is compatible with your system.

@return \c The OTP read word
 */
unsigned int Test_ProjReadOtpWord (uint32_t offsetInWords  /*!< [in] OTP word offset to be read. */
);


/*!
@brief This function burns the OTP buffer with the defined mask.
        You must set chipIndication before calling this function explicitly by calling Test_ProjSetChipIndication()
        or by calling Test_ProjBuildDefaultOtp().
        \n Words with 0 value, are not being burned
   \note You must replace TEST_WRITE_OTP_BY_ENV() and TEST_WRITE_ENV_REG() macros with implementation that is compatible with your system.

@return \c TEST_OK on success.
@return A non-zero value from test_proj_common.h on failure.
 */
unsigned int Test_ProjBurnOtp(unsigned int  *otpBuff, /*!< [in] OTP buffer to be burned. */
                                   unsigned int  nextLcs /*!< [in] The LCS expected after burning the OTP. */
);

/*!
@brief This function sets the OTP buffer with mandatory fields (LCS changing fields): HUK and HBK#.
       \n  Class keys should be set by the user out of this function scope by calling Test_ProjSetOtpField().
        \n baseLcs valid values are CM/DM/SE only, if RMA is needed set rmaMode to PROJ_OTP_RMA_FULL.
        \n baseLcs should be based on required fields in OTP to be filled
   \note You must replace TEST_WRITE_OTP_BY_ENV() and TEST_WRITE_ENV_REG() macros with implementation that is compatible with your system.

@return \c TEST_OK on success.
@return A non-zero value from test_proj_common.h on failure.
 */
unsigned int Test_ProjBuildDefaultOtp(unsigned int  *otpBuff, /*!< [in] OTP buffer to be built. */
                                      uint32_t  otpBuffWordSize, /*!< [in] The size in words of otpBuffer. */ /*is this the prevous param otpBuff?*/
                                      unsigned int  baseLcs,    /*!< [in] The OTP buffer base CM/DM/SE. */
                                      ProjOtp_ChipState_t chipIndication, /*!< [in] The OTP chip indication. */
                                      ProjOtp_Rma_t rmaMode, /*!< [in] The OTP RMA mode. */
                                      uint32_t  isSd,   /*!< [in] The OTP secure disable flag. */
                                      uint32_t isHbkFull  /*!< [in] The OTP HBK full flag. */
);

/*!
 @}
 */
#endif //_TEST_PROJ_OTP_PLAT_H__
