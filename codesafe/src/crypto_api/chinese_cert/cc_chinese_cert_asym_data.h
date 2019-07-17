/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#ifndef  _CC_CH_CERT_ASYM_DATA_H
#define  _CC_CH_CERT_ASYM_DATA_H

//-----------------------------------------------------------------------------------------------------------------//
//-------------------------------- SM2 ----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
#define CH_CERT_SM2_PUBLIC_KEY1_SIZE    65
#define CH_CERT_SM2_PUBLIC_KEY1         { /*CC_EC_PointUncompressed*/ \
                                        0x04,                       \
                                        /* x value*/                \
                                        0x0A, 0xE4, 0xC7, 0x79, 0x8A, 0xA0, 0xF1, 0x19, 0x47, 0x1B, 0xEE, 0x11, 0x82, 0x5B, 0xE4, 0x62, \
                                        0x02, 0xBB, 0x79, 0xE2, 0xA5, 0x84, 0x44, 0x95, 0xE9, 0x7C, 0x04, 0xFF, 0x4D, 0xF2, 0x54, 0x8A, \
                                        /* y value */               \
                                        0x7C, 0x02, 0x40, 0xF8, 0x8F, 0x1C, 0xD4, 0xE1, 0x63, 0x52, 0xA7, 0x3C, 0x17, 0xB7, 0xF1, 0x6F, \
                                        0x07, 0x35, 0x3E, 0x53, 0xA1, 0x76, 0xD6, 0x84, 0xA9, 0xFE, 0x0C, 0x6B, 0xB7, 0x98, 0xE8, 0x57 }

#define CH_CERT_SM2_PRIVATE_KEY1_SIZE   32
#define CH_CERT_SM2_PRIVATE_KEY1        { 0x12, 0x8B, 0x2F, 0xA8, 0xBD, 0x43, 0x3C, 0x6C, 0x06, 0x8C, 0x8D, 0x80, 0x3D, 0xFF, 0x79, 0x79, \
                                        0x2A, 0x51, 0x9A, 0x55, 0x17, 0x1B, 0x1B, 0x65, 0x0C, 0x23, 0x66, 0x1D, 0x15, 0x89, 0x72, 0x63 }

#define CH_CERT_SM2_ID_A1_SIZE          18
#define CH_CERT_SM2_ID_A1               "ALICE123@YAHOO.COM"

#define CH_CERT_SM2_SIGN_INPUT1_SIZE    14
#define CH_CERT_SM2_SIGN_INPUT1         { 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20, 0x64, 0x69, 0x67, 0x65, 0x73, 0x74 }

#define CH_CERT_SM2_SIGN_OUT1_SIZE       64

#endif  /* _CC_CH_CERT_ASYM_DATA_H */


