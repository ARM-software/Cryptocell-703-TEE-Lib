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
#include <stdint.h>
#include "cc_otp_defs.h"
#include "test_proj_common.h"

#define TEST_OTP_LAST_WORD_IN_MASK 0x22
uint32_t gHukBuff[CC_OTP_HUK_SIZE_IN_WORDS] =    {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F,
                                                  0x10111213, 0x14151617, 0x18191A1B, 0x1C1D1E1F};  // 0xB0 (176) zeros
uint32_t gHbk256Buff[CC_OTP_HBK_SIZE_IN_WORDS] = {0x10111213, 0x14151617, 0x18191A1B, 0x1C1D1E1F,
                                                  0x20212223, 0x24252627, 0x28292A2B, 0x2C2D2E2F};  // 0xA0 (160) zeros
uint32_t gHbk0Buff[CC_OTP_HBK0_SIZE_IN_WORDS] =  {0x30313233, 0x34353637, 0x38393A3B, 0x3C3D3E3F};  // 0x40 (64) zeros
uint32_t gHbk1Buff[CC_OTP_HBK1_SIZE_IN_WORDS] =  {0x40414243, 0x44454647, 0x48494A4B, 0x4C4D4E4F};  // 0x50 (80) zeros
uint32_t gClassKeyBuff[CC_OTP_KPICV_SIZE_IN_WORDS] = {0x50515253, 0x54555657, 0x58595A5B, 0x5C5D5E5F};   // 0x40 (64) zeros
uint32_t gClear128Buff[TEST_PROJ_128BIT_KEY_SIZE_WORDS] = {0x0};
uint32_t gClear256Buff[TEST_PROJ_256BIT_KEY_SIZE_WORDS] = {0x0};


uint32_t gTestOtpMaskV1[CC_OTP_USER_DEFINED_OFFSET] = {
        0x04030201,    // 0x0
        0x08070605,    // 0x1
        0x0c0b0a09,    // 0x2
        0x100f0e0d,    // 0x3
        0x14131211,    // 0x4
        0x18171615,    // 0x5
        0x1c1b1a19,    // 0x6
        0x201f1e1d,    // 0x7
        0x24232221,    // 0x8
        0x28272625,    // 0x9
        0x2c2b2a29,    // 0xa
        0x302f2e2d,    // 0xb
        0x34333231,    // 0xc
        0x38373635,    // 0xd
        0x3c3b3a39,    // 0xe
        0x403f3e3d,    // 0xf
        0x00000000,    // 0x10
        0x00000000,    // 0x11
        0x00000000,    // 0x12
        0x00000000,    // 0x13
        0x00000000,    // 0x14
        0x00000000,    // 0x15
        0x00000000,    // 0x16
        0x00000000,    // 0x17
        0x00000000,    // 0x18
        0x00000000,    // 0x19
        0x00000000,    // 0x1A
        0x44434241,    // 0x1B
        0x48474645,    // 0x1C
        0x4c4b4a49,    // 0x1D
        0x504f4e4d,    // 0x1E
        0x54535251,    // 0x1F
        0x58575655,    // 0x20
        0x5c5b5a59,    // 0x21
        0x605f5e5d,    // 0x22
        0x00000000,    // 0x23
        0x00000000,    // 0x24
        0x00000000,    // 0x25
        0x00000000,    // 0x26
        0x00000000,    // 0x27
        0x00000000,    // 0x28
        0x00000000,    // 0x29
        0x00000000,    // 0x2A
        0x00000000,    // 0x2B
        0x00000000,    // 0x2C
        0x00000000,    // 0x2D
        0x00000000,    // 0x2E
        0x00000000,    // 0x2F
        0x00000000,    // 0x30
        0x00000000,    // 0x31
        0x00000000,    // 0x32
        0x00000000,    // 0x33
};


uint32_t gTestOtpMaskV2[CC_OTP_USER_DEFINED_OFFSET] = {
        0x64636261,    // 0x0
        0x68676665,    // 0x1
        0x6c6b6a69,    // 0x2
        0x706f6e6d,    // 0x3
        0x74737271,    // 0x4
        0x78777675,    // 0x5
        0x7c7b7a79,    // 0x6
        0x807f7e7d,    // 0x7
        0x84838281,    // 0x8
        0x88878685,    // 0x9
        0x8c8b8a89,    // 0xa
        0x908f8e8d,    // 0xb
        0x94939291,    // 0xc
        0x98979695,    // 0xd
        0x9c9b9a99,    // 0xe
        0xa09f9e9d,    // 0xf
        0x00000000,    // 0x10
        0x00000000,    // 0x11
        0x00000000,    // 0x12
        0x00000000,    // 0x13
        0x00000000,    // 0x14
        0x00000000,    // 0x15
        0x00000000,    // 0x16
        0x00000000,    // 0x17
        0x00000000,    // 0x18
        0x00000000,    // 0x19
        0x00000000,    // 0x1A
        0xa4a3a2a1,    // 0x1B
        0xa8a7a6a5,    // 0x1C
        0xacabaaa9,    // 0x1D
        0xb0afaead,    // 0x1E
        0xb4b3b2b1,    // 0x1F
        0xb8b7b6b5,    // 0x20
        0xbcbbbab9,    // 0x21
        0xc0bfbebd,    // 0x22
        0x00000000,    // 0x23
        0x00000000,    // 0x24
        0x00000000,    // 0x25
        0x00000000,    // 0x26
        0x00000000,    // 0x27
        0x00000000,    // 0x28
        0x00000000,    // 0x29
        0x00000000,    // 0x2A
        0x00000000,    // 0x2B
        0x00000000,    // 0x2C
        0x00000000,    // 0x2D
        0x00000000,    // 0x2E
        0x00000000,    // 0x2F
        0x00000000,    // 0x30
        0x00000000,    // 0x31
        0x00000000,    // 0x32
        0x00000000,    // 0x33
};

