/****************************************************************************
 * The confidential and proprietary information contained in this file may    *
 * only be used by a person authorised under and to the extent permitted      *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates).  *
 *   (C) COPYRIGHT [2001-2019] Arm Limited (or its affiliates).                    *
 *       ALL RIGHTS RESERVED                                                  *
 * This entire notice must be reproduced on all copies of this file           *
 * and copies of this file may only be made by a person if such person is     *
 * permitted to do so under the terms of a subsisting license agreement       *
 * from Arm Limited (or its affiliates).                                      *
*****************************************************************************/

#ifndef _TEST_PROJ_PLAT_H_
#define _TEST_PROJ_PLAT_H_

#include <stdint.h>

/*!
  @file
  @brief This file contains definitions and APIs that set the testing environment.
 */


  /*!
  @addtogroup cc_env_tests
  @{
 */
/******************************************************************
 * Defines
 ******************************************************************/

/*! Defines the cache parameters group set for the environment register. */
typedef enum TestProjCache_t {
     TEST_PROJ_HW_CACHE, /*!< AxUSER - HW - 1. */
     TEST_PROJ_SW_CACHE /*!< AxUSER - HW - 0. */
} TestProjCache_t;


/*! Defines Environment register read.
 *   \note You must implement the read environment register that is compatible with your system.
 */
#define TEST_READ_ENV_REG(offset) \
        *(volatile uint32_t *)(processMap.processTeeHwEnvBaseAddr + (offset))

/*! Defines Environment register write.
 *   \note You must implement the write environment register that is compatible with your system.
 */
#define TEST_WRITE_ENV_REG(offset, val)  { \
        volatile uint32_t ii1; \
        (*(volatile uint32_t *)(processMap.processTeeHwEnvBaseAddr + (offset))) = (uint32_t)(val); \
        for(ii1=0; ii1<500; ii1++); \
}



/****************************************************************************
 * Functions
****************************************************************************/
/*!
@brief This function maps the CryptoCell base register and environment base register.
*   \note You must replace the environment mapping with implementation that is compatible with your system.

@return \c TEST_OK on success.
@return A non-zero value from test_proj_common.h on failure.
 */
uint32_t Test_ProjMap(void);


/*!
@brief This function unmaps the CryptoCell base register and environment base register.
*   \note You must replace the environment un-mapping with implementation that is compatible with your system.

@return \c None.
 */
void Test_ProjUnmap(void);


/*!
@brief This function performs power-on-reset to CryptoCell, AO & environment modules using environment register.
*   \note You must define power-on-reset implementation that is compatible with your system.

@return \c None
 */
void Test_ProjPerformPowerOnReset(void);

/*!
@brief This function performs cold-reset to CryptoCell and AO modules using environment register.
*   \note  You must define cold-reset implementation that is compatible with your system.

@return \c None
 */
void Test_ProjPerformColdReset(void);

/*!
@brief This function performs warm-reset to CryptoCell module using environment register.
*   \note  You must define warm-reset implementation that is compatible with your system.

@return \c None
 */
void Test_ProjPerformWarmReset(void);


/*!
@brief This function sets the sp enable bit to CryptoCell module.

@return \c None
 */
void Test_ProjSetSpEnable(void);


/*!
@brief This function sets the cache parameters.
*   The set operation is done via environment registers.
*   \note  You must replace TEST_READ_OTP_BY_ENV() macro with implementation that is compatible with your system.

@return \c None
 */
void Test_ProjSetCacheParams(TestProjCache_t cacheType);


/*!
@brief This function sets the device security mode.
*   The set operation is done via environment registers.
*   \note  You must replace TEST_READ_OTP_BY_ENV() macro with implementation that is compatible with your system.

@return \c None
 */
void Test_ProjSetSecureMode(void);
 /*!
  @}
  */


/*!
@brief This function sets the fpga to slim/full mode according to CC_SUPPORT_FULL_PROJECT flag.
*   The set operation is done via environment registers.
*   This function is needed for testing with FPGA.

@return \c None
 */
void Test_ProjSetFlavor(void);

/*!
* @brief This function resets the FPGA to the original flavor it was in - full
*   The set operation is done via environment registers.
*   This function is needed for testing with FPGA.

@return \c None
 */
void Test_ProjSetFullFlavor(void);

#endif /*_TEST_PROJ_PLAT_H_ */

