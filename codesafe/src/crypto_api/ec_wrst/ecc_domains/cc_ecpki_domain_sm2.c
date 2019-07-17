/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */


#include "cc_pal_types.h"
#include "cc_ecpki_types.h"



/***********************************************************************************
 *   Data base of CC_ECPKI_DomainID_Sm2: structure of type  CCEcpkiDomain_t        *
 *       All data is given in little endian order of words in arrays               *
 ***********************************************************************************/
static const CCEcpkiDomain_t ecpki_domain_sm2 = {

    /* Field modulus :  GF_Modulus =  8542D69E 4C044F18 E8B92435 BF6FF7DE 45728391 5C45517D 722EDB8B 08F1DFC3 - big end*/
    {0x08F1DFC3,0x722EDB8B,0x5C45517D,0x45728391,0xBF6FF7DE,0xE8B92435,0x4C044F18,0x8542D69E},

    /* EC equation parameters a, b  */
    /* a = 787968B4 FA32C3FD 2417842E 73BBFEFF 2F3C848B 6831D7E0 EC65228B 3937E498  - big end  from SM2 */
    {0x3937E498,0xEC65228B,0x6831D7E0,0x2F3C848B,0x73BBFEFF,0x2417842E,0xFA32C3FD,0x787968B4},
    /* b = 63E4C6D3 B23B0C84 9CF84241 484BFE48 F61D59A5 B16BA06E 6E12D1DA 27C5249A  - big end  from SM2 */
    {0x27C5249A,0x6E12D1DA,0xB16BA06E,0xF61D59A5,0x484BFE48,0x9CF84241,0xB23B0C84,0x63E4C6D3},

    /* Order of generator: 8542D69E 4C044F18 E8B92435 BF6FF7DD 29772063 0485628D 5AE74EE7 C32E79B7 big end  from SM2 */
    {0xC32E79B7,0x5AE74EE7,0x0485628D,0x29772063,0xBF6FF7DD,0xE8B92435,0x4C044F18,0x8542D69E},

    /* Generator  coordinates in affine form: EC_Gener_X, EC_Gener_Y (in ordinary representation) */
    /* 421DEBD6 1B62EAB6 746434EB C3CC315E 32220B3B ADD50BDC 4C4E6C14 7FEDD43D   X - big end  from SM2 */
    {0x7FEDD43D,0x4C4E6C14,0xADD50BDC,0x32220B3B,0xC3CC315E,0x746434EB,0x1B62EAB6,0x421DEBD6},
    /* 0680512B CBB42C07 D47349D2 153B70C4 E5D7FDFC BFA36EA1 A85841B9 E46E09A2   Y - big end  from SEC2 */
    //0680512B CBB42C07 D47349D2 153B70C4 E5D7FDFC BFA36EA1 A85841B9 E46E09A2
    {0xE46E09A2,0xA85841B9,0xBFA36EA1,0xE5D7FDFC,0x153B70C4,0xD47349D2,0xCBB42C07,0x0680512B},

    1, /* EC cofactor K */

     /* Barrett tags NP,RP */
#ifdef CC_SUPPORT_PKA_128_32
    {0xFDAB0BFA, 0xF3D0A0AA, 0x2BB5DCCE, 0xE4AB1E30, 0x000000F5,
    0xFDAB0E06, 0xF3D0A0AA, 0x2BB5DCCE, 0xE4AB1E30, 0x000000F5},
#else  // CC_SUPPORT_PKA_64_16
    {0x2BB5DCCE, 0xE4AB1E30, 0x000000F5, 0x00000000, 0x00000000,
    0x2BB5DCCE, 0xE4AB1E30, 0x000000F5, 0x00000000, 0x00000000},
#endif
    256, /* Size of field modulus in bits */
    256, /* Size of order of generator in bits */
    5,   /* Size of each inserted Barret tag in words; 0 - if not inserted */

    CC_ECPKI_DomainID_sm2,	/* EC Domain identifier - enum */
    "SM2_PRIME_256" /*SM2*/

};




/**************************************************************************
 *                CC_EcpkiGetSm2Domain
 **************************************************************************/
/*!
 @brief    the function returns the domain pointer id the domain is supported for the product;
		otherwise return NULL
 @return   return domain pointer or NULL

*/
const CCEcpkiDomain_t *CC_EcpkiGetSm2Domain(void)
{
    return &ecpki_domain_sm2;
}

