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

#ifndef _TEST_PROJ_H_
#define _TEST_PROJ_H_

#include <stdint.h>

#include "test_proj_common.h"
#include "dx_reg_base_host.h"
#include "test_proj_plat.h"

#define TEST_PROJ_CC_REG_MAP_AREA_LEN     0x20000

#ifdef CC_PLAT_ZYNQ7000
    #define DX_REE_BASE_CC          0x80000000
#elif defined CC_PLAT_JUNO
    #define DX_REE_BASE_CC          0x60010000
#endif


struct ProcessMappingArea_t {
    unsigned long       processTeeHwRegBaseAddr;
    unsigned long       processTeeHwEnvBaseAddr;
    unsigned long       processReeHwRegBaseAddr;
    unsigned long       processTeeUnmanagedBaseAddr;
};

extern struct ProcessMappingArea_t processMap;

#define TEST_READ_TEE_CC_REG(offset) \
        *(volatile uint32_t *)(processMap.processTeeHwRegBaseAddr + (offset))

#define TEST_WRITE_TEE_CC_REG(offset, val)  { \
        volatile uint32_t ii1; \
        (*(volatile uint32_t *)(processMap.processTeeHwRegBaseAddr + (offset))) = (uint32_t)(val); \
        for(ii1=0; ii1<500; ii1++); \
}

#define TEST_READ_REE_REG(offset) \
        (*(volatile uint32_t *)(processMap.processReeHwRegBaseAddr  + (offset)));

#define TEST_WRITE_REE_REG(offset, val)   { \
    volatile uint32_t ii1; \
        (*(volatile uint32_t *)(processMap.processReeHwRegBaseAddr  + (offset))) = (uint32_t)(val); \
        for(ii1=0; ii1<500; ii1++); \
}
/****************************************************************************/
/*   							External API  								*/
/****************************************************************************/
/*
 * @brief This function initializes platform, i.e maps its relevant memory regions.
 *
 * @param[in/out] *pProcessMap - mapping regions
 *
 * @return rc - 0 for success, 1 for failure.
 */
int Test_ProjInit(void);

/****************************************************************************/
/*
 * @brief This function frees previously allocated resources
 *
 * @param[in/out] *pProcessMap - mapping regions
 *
 * @return rc - 0 for success, 1 for failure
 */
void Test_ProjFree(void);

#endif /*_TEST_PROJ_H_ */

