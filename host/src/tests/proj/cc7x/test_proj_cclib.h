/****************************************************************************
 * The confidential and proprietary information contained in this file may    *
 * only be used by a person authorised under and to the extent permitted      *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates).  *
 *   (C) COPYRIGHT [2001-2018] Arm Limited (or its affiliates).                    *
 *       ALL RIGHTS RESERVED                                                  *
 * This entire notice must be reproduced on all copies of this file           *
 * and copies of this file may only be made by a person if such person is     *
 * permitted to do so under the terms of a subsisting license agreement       *
 * from Arm Limited (or its affiliates).                                      *
*****************************************************************************/

#ifndef _TEST_PROJ_CCLIB_H_
#define _TEST_PROJ_CCLIB_H_

#ifdef CC_SUPPORT_FULL_PROJECT
#ifdef CC_SUPPORT_FIPS
#include "cc_fips.h"
#endif /* CC_SUPPORT_FIPS */
#endif /* CC_SUPPORT_FULL_PROJECT */

/****************************************************************************/
/*
 * @brief This function
 *
 * @param[in/out]
  *
 * @return rc -
 */
int Test_Proj_CC_LibInit_Wrap(void);

void Test_Proj_CC_LibFini_Wrap(void);

#ifdef CC_SUPPORT_FULL_PROJECT
#ifdef CC_SUPPORT_FIPS
int Test_ProjSetReeFipsError(uint32_t  reeError, CCFipsState_t expfipsState);
#endif /* CC_SUPPORT_FIPS */
#endif /* CC_SUPPORT_FULL_PROJECT */

#endif /* _TEST_PROJ_CCLIB_H_ */

