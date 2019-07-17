/****************************************************************************
 * The confidential and proprietary information contained in this file may   *
 * only be used by a person authorised under and to the extent permitted     *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates). *
 *   (C) COPYRIGHT [2001-2018] Arm Limited (or its affiliates).              *
 *       ALL RIGHTS RESERVED                                                 *
 * This entire notice must be reproduced on all copies of this file          *
 * and copies of this file may only be made by a person if such person is    *
 * permitted to do so under the terms of a subsisting license agreement      *
 * from Arm Limited (or its affiliates).                                     *
*****************************************************************************/

#include "test_proj.h"


#define Test_ProjWriteWordsToSram(addr, data, size)                                                                            \
                do {                                                                                                           \
                    uint32_t ii;                                                                                               \
                    volatile uint32_t dummy;                                                                                   \
                    TEST_WRITE_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF,SRAM_ADDR), (addr));                                        \
                    for( ii = 0 ; ii < size/sizeof(uint32_t) ; ii++ ) {                                                        \
                        TEST_WRITE_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF,SRAM_DATA), SWAP_TO_LE(((uint32_t *)data)[ii]));        \
                        do {                                                                                                   \
                            dummy = TEST_READ_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF, SRAM_DATA_READY));                          \
                        }while(!(dummy & 0x1));                                                                                \
                    }                                                                                                          \
                }while(0)

#define Test_ProjReadWordsFromSram(sramAddr , hostBuff , size)                                                                   \
                do {                                                                                                           \
                    uint32_t ii;                                                                                               \
                    volatile uint32_t dummy;                                                                                   \
                    TEST_WRITE_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF,SRAM_ADDR) ,(sramAddr) );                                   \
                    dummy = TEST_READ_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF,SRAM_DATA));                                         \
                    for( ii = 0 ; ii < (size/sizeof(uint32_t)) ; ii++ ) {                                                      \
                        do {                                                                                                   \
                            dummy = TEST_READ_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF, SRAM_DATA_READY));                          \
                        }while(!(dummy & 0x1));                                                                                \
                        dummy = TEST_READ_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF,SRAM_DATA));                                     \
                        ((uint32_t*)hostBuff)[ii] = SWAP_TO_LE(dummy);                                                         \
                    }                                                                                                          \
                    do {                                                                                                       \
                        dummy = TEST_READ_TEE_CC_REG( CC_REG_OFFSET (HOST_RGF, SRAM_DATA_READY));                              \
                    }while(!(dummy & 0x1));                                                                                    \
                }while(0)
