/****************************************************************************
* The confidential and proprietary information contained in this file may   *
* only be used by a person authorised under and to the extent permitted     *
* by a subsisting licensing agreement from Arm Limited (or its affiliates). *
*     (C) COPYRIGHT [2019] Arm Limited (or its affiliates).                 *
*         ALL RIGHTS RESERVED                                               *
* This entire notice must be reproduced on all copies of this file          *
* and copies of this file may only be made by a person if such person is    *
* permitted to do so under the terms of a subsisting license agreement      *
* from Arm Limited (or its affiliates).                                     *
*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

/* Test engine and testAL headers*/
#include "test_engine.h"
#include "test_pal_mem.h"

/* CryptoCell headers*/
#include "cc_sm2.h"
#include "cc_ecpki_domain_sm2.h"
#include "cc_ecpki_build.h"


/******************************************************************
 * Defines
 ******************************************************************/
#define SM2_TV_RAND_SEED_SIZE           32
#define SM2_TV_MAX_KEY_SIZE_IN_BYTES    16

/******************************************************************
 * Enums
 ******************************************************************/

/******************************************************************
 * Types
 ******************************************************************/

typedef struct Sm2KeyExchangeVector_t{
    /* Input - User A */
    const char* id_UserA;
    size_t      idSize_UserA;
    uint8_t     privateKeyInBytes_UserA[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesX_UserA[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesY_UserA[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     confirmationrequired_UserA;
    uint8_t     randomSeed_UserA[SM2_TV_RAND_SEED_SIZE];

    /* Input - User B */
    const char* id_UserB;
    size_t      idSize_UserB;
    uint8_t     privateKeyInBytes_UserB[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesX_UserB[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesY_UserB[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     confirmationrequired_UserB;
    uint8_t     randomSeed_UserB[SM2_TV_RAND_SEED_SIZE];

    /* Input - common */
    size_t      privateKeySize;
    size_t      pubKeySize;
    size_t      requiredKeySizeInBits;

    /* Intermediate results and their sizes - User A */
    CCEcpkiUserPrivKey_t    privKey_UserA;
    CCEcpkiUserPublKey_t    pubKey_UserA;

    /* Intermediate results and their sizes - User B */
    CCEcpkiUserPrivKey_t    privKey_UserB;
    CCEcpkiUserPublKey_t    pubKey_UserB;


    /* Output */
    uint8_t     outputKey_UserA[SM2_TV_MAX_KEY_SIZE_IN_BYTES];
    size_t      outputKeySize_UserA; /* in/out */
    uint8_t     outputKey_UserB[SM2_TV_MAX_KEY_SIZE_IN_BYTES];
    size_t      outputKeySize_UserB; /* in/out */

    /* Reference */
    uint8_t     referenceKey[SM2_TV_MAX_KEY_SIZE_IN_BYTES];
    size_t      referenceKeySize;
}Sm2KeyExchangeVector_t;
/******************************************************************
 * Externs
 ******************************************************************/

/******************************************************************
 * Globals
 ******************************************************************/


static Sm2KeyExchangeVector_t sm2_vector = {
    /* Input - User A */
    .id_UserA = "ALICE123@YAHOO.COM",
    .idSize_UserA = 18,
    .privateKeyInBytes_UserA = { /* 6FCBA2EF 9AE0AB90 2BC3BDE3 FF915D44 BA4CC78F 88E2F8E7 F8996D3B 8CCEEDEE */
            0x6F, 0xCB, 0xA2, 0xEF, 0x9A, 0xE0, 0xAB, 0x90,
            0x2B, 0xC3, 0xBD, 0xE3, 0xFF, 0x91, 0x5D, 0x44,
            0xBA, 0x4C, 0xC7, 0x8F, 0x88, 0xE2, 0xF8, 0xE7,
            0xF8, 0x99, 0x6D, 0x3B, 0x8C, 0xCE, 0xED, 0xEE
    },
    .publicKeyInBytesX_UserA = { /* 3099093B F3C137D8 FCBBCDF4 A2AE50F3 B0F216C3 122D7942 5FE03A45 DBFE1655 */
            0x30, 0x99, 0x09, 0x3B, 0xF3, 0xC1, 0x37, 0xD8,
            0xFC, 0xBB, 0xCD, 0xF4, 0xA2, 0xAE, 0x50, 0xF3,
            0xB0, 0xF2, 0x16, 0xC3, 0x12, 0x2D, 0x79, 0x42,
            0x5F, 0xE0, 0x3A, 0x45, 0xDB, 0xFE, 0x16, 0x55
    },
    .publicKeyInBytesY_UserA = { /* 3099093B F3C137D8 FCBBCDF4 A2AE50F3 B0F216C3 122D7942 5FE03A45 DBFE1655 */
            0x3D, 0xF7, 0x9E, 0x8D, 0xAC, 0x1C, 0xF0, 0xEC,
            0xBA, 0xA2, 0xF2, 0xB4, 0x9D, 0x51, 0xA4, 0xB3,
            0x87, 0xF2, 0xEF, 0xAF, 0x48, 0x23, 0x39, 0x08,
            0x6A, 0x27, 0xA8, 0xE0, 0x5B, 0xAE, 0xD9, 0x8B
    },
    .randomSeed_UserA = {
            0x83, 0xA2, 0xC9, 0xC8, 0xB9, 0x6E, 0x5A, 0xF7,
            0x0B, 0xD4, 0x80, 0xB4, 0x72, 0x40, 0x9A, 0x9A,
            0x32, 0x72, 0x57, 0xF1, 0xEB, 0xB7, 0x3F, 0x5B,
            0x07, 0x33, 0x54, 0xB2, 0x48, 0x66, 0x85, 0x63
    },
    .outputKeySize_UserA = SM2_TV_MAX_KEY_SIZE_IN_BYTES, /* in/out */
    .confirmationrequired_UserA = 3U,


    /* Input - User B */
    .id_UserB = "BILL456@YAHOO.COM",
    .idSize_UserB = 17,
    .privateKeyInBytes_UserB = { /* 5E35D7D3 F3C54DBA C72E6181 9E730B01 9A84208C A3A35E4C 2E353DFC CB2A3B53 */
            0x5E, 0x35, 0xD7, 0xD3, 0xF3, 0xC5, 0x4D, 0xBA,
            0xC7, 0x2E, 0x61, 0x81, 0x9E, 0x73, 0x0B, 0x01,
            0x9A, 0x84, 0x20, 0x8C, 0xA3, 0xA3, 0x5E, 0x4C,
            0x2E, 0x35, 0x3D, 0xFC, 0xCB, 0x2A, 0x3B, 0x53
    },
    .publicKeyInBytesX_UserB = { /* 245493D4 46C38D8C C0F11837 4690E7DF 633A8A4B FB3329B5 ECE604B2 B4F37F43 */
            0x24, 0x54, 0x93, 0xD4, 0x46, 0xC3, 0x8D, 0x8C,
            0xC0, 0xF1, 0x18, 0x37, 0x46, 0x90, 0xE7, 0xDF,
            0x63, 0x3A, 0x8A, 0x4B, 0xFB, 0x33, 0x29, 0xB5,
            0xEC, 0xE6, 0x04, 0xB2, 0xB4, 0xF3, 0x7F, 0x43
    },
    .publicKeyInBytesY_UserB = { /* 53C0869F 4B9E1777 3DE68FEC 45E14904 E0DEA45B F6CECF99 18C85EA0 47C60A4C */
            0x53, 0xC0, 0x86, 0x9F, 0x4B, 0x9E, 0x17, 0x77,
            0x3D, 0xE6, 0x8F, 0xEC, 0x45, 0xE1, 0x49, 0x04,
            0xE0, 0xDE, 0xA4, 0x5B, 0xF6, 0xCE, 0xCF, 0x99,
            0x18, 0xC8, 0x5E, 0xA0, 0x47, 0xC6, 0x0A, 0x4C
    },
    .randomSeed_UserB = {
            0x33, 0xFE, 0x21, 0x94, 0x03, 0x42, 0x16, 0x1C,
            0x55, 0x61, 0x9C, 0x4A, 0x0C, 0x06, 0x02, 0x93,
            0xD5, 0x43, 0xC8, 0x0A, 0xF1, 0x97, 0x48, 0xCE,
            0x17, 0x6D, 0x83, 0x47, 0x7D, 0xE7, 0x1C, 0x80
    },
    .outputKeySize_UserB = SM2_TV_MAX_KEY_SIZE_IN_BYTES, /* in/out */
    .confirmationrequired_UserB = 3U,

    /* Input - common */
    .privateKeySize = 32,
    .pubKeySize = 65,
    .requiredKeySizeInBits = SM2_TV_MAX_KEY_SIZE_IN_BYTES * CC_BITS_IN_BYTE,

    /* Reference */
    .referenceKey = {
            0x55, 0xB0, 0xAC, 0x62, 0xA6, 0xB9, 0x27, 0xBA, 0x23, 0x70, 0x38, 0x32, 0xC8, 0x53, 0xDE, 0xD4
    },
    .referenceKeySize = SM2_TV_MAX_KEY_SIZE_IN_BYTES,
};

/******************************************************************
 * Static Prototypes
 ******************************************************************/
static TE_rc_t sm2_ke_prepare(void *pContext);
static TE_rc_t sm2_ke_execute_2_parties(void *pContext);
static TE_rc_t sm2_ke_verify(void *pContext);
static TE_rc_t sm2_ke_clean(void *pContext);

/******************************************************************
 * Static functions
 ******************************************************************/
static int Tests_RndGenerateVectorConst( void           *rngState_vptr,    /*in*/
                                         uint8_t        *out_ptr,          /*out*/
                                         size_t         outSizeBytes)      /*in*/
{
    CCError_t  error = CC_OK;
    uint8_t* pData = (uint8_t*)rngState_vptr;

    if (outSizeBytes != SM2_TV_RAND_SEED_SIZE)
        return 1;

    memcpy (out_ptr, pData, outSizeBytes);
    return error;
}


static TE_rc_t sm2_ke_prepare(void *pContext)
{
    CCEcpkiBuildTempData_t tmpData;
    TE_rc_t res = TE_RC_SUCCESS;
    const CCEcpkiDomain_t* pSm2Domain = CC_EcpkiGetSm2Domain();
    Sm2KeyExchangeVector_t *sm2_vec = (Sm2KeyExchangeVector_t *)pContext;
    uint8_t publicKeyInBytes[ 2 * CC_SM2_MODULE_LENGTH_IN_BYTES + 1 /*1 bit for compression flag*/];


    /* User A  - Set key data in a structure */
    /* Set the public key data in 1 array:  compression flag || x || y */
    publicKeyInBytes[0] = CC_EC_PointUncompressed;
    memcpy(publicKeyInBytes + 1, sm2_vec->publicKeyInBytesX_UserA, sm2_vec->pubKeySize/2);
    memcpy(publicKeyInBytes + 1 + sm2_vec->pubKeySize/2,
           sm2_vec->publicKeyInBytesY_UserA, sm2_vec->pubKeySize/2);


    /* User A -
     * Build public and private keys -
     * input the keys as bytes array (public key:  compression flag || x || y)
     * output the keys as structures, which are used as inputs to all SM2 or ECC operations.
     * */
    TE_ASSERT_PASS(CC_EcpkiPublKeyBuildAndCheck (pSm2Domain, publicKeyInBytes, sm2_vec->pubKeySize,
                                                 0/*CheckPointersAndSizesOnly*/, &sm2_vec->pubKey_UserA, &tmpData) , CC_OK);

    TE_ASSERT_PASS(CC_EcpkiPrivKeyBuild(pSm2Domain, sm2_vec->privateKeyInBytes_UserA,
                                        sm2_vec->privateKeySize, &sm2_vec->privKey_UserA) , CC_OK);

    /* User B  - Set key data in a structure */
    /* Set the public key data in 1 array:  compression flag || x || y */
    publicKeyInBytes[0] = CC_EC_PointUncompressed;
    memcpy(publicKeyInBytes + 1, sm2_vec->publicKeyInBytesX_UserB, sm2_vec->pubKeySize/2);
    memcpy(publicKeyInBytes + 1 + sm2_vec->pubKeySize/2,
           sm2_vec->publicKeyInBytesY_UserB, sm2_vec->pubKeySize/2);


    /* User B -
     * Build public and private keys -
     * input the keys as bytes array (public key:  compression flag || x || y)
     * output the keys as structures, which are used as inputs to all SM2 or ECC operations.
     * */
    TE_ASSERT_PASS(CC_EcpkiPublKeyBuildAndCheck (pSm2Domain, publicKeyInBytes, sm2_vec->pubKeySize,
                                                 0/*CheckPointersAndSizesOnly*/, &sm2_vec->pubKey_UserB, &tmpData) , CC_OK);

    TE_ASSERT_PASS(CC_EcpkiPrivKeyBuild(pSm2Domain, sm2_vec->privateKeyInBytes_UserB,
                                        sm2_vec->privateKeySize, &sm2_vec->privKey_UserB) , CC_OK);

    goto bail;
bail:
    return res;
}

static TE_rc_t sm2_ke_execute_2_parties(void *pContext)
{
    Sm2KeyExchangeVector_t  *sm2_vec = (Sm2KeyExchangeVector_t *)pContext;
    TE_rc_t         res = TE_RC_SUCCESS;
    TE_perfIndex_t  cookie;
    uint8_t*        workBuff = NULL;
    size_t          workBuffSize = 0;
    size_t          largestIdSize = 0;

    CCEcpkiUserPublKey_t    randomPoint_UserA;
    CCEcpkiUserPublKey_t    randomPoint_UserB;

    CC_Sm2KeContext_t       Sm2KeContext_a;
    CC_Sm2KeContext_t       Sm2KeContext_b;

    uint8_t outConfValue_UserA[CC_SM2_CONF_VALUE_LENGTH_IN_BYTES];
    uint8_t outConfValue_UserB[CC_SM2_CONF_VALUE_LENGTH_IN_BYTES];
    size_t  outConfValueSize_UserA = CC_SM2_CONF_VALUE_LENGTH_IN_BYTES;
    size_t  outConfValueSize_UserB = CC_SM2_CONF_VALUE_LENGTH_IN_BYTES;


    /* Start performance measurement */
    cookie = TE_perfOpenNewEntry("sm2", "key exchange - 2 parties");

    /* Allocate working buffer */
    largestIdSize = sm2_vec->idSize_UserA > sm2_vec->idSize_UserB ? sm2_vec->idSize_UserA : sm2_vec->idSize_UserB;
    workBuffSize = 2 + CC_SM2_MODULE_LENGTH_IN_BYTES*4 + CC_SM2_ORDER_LENGTH_IN_BYTES*2 + largestIdSize;
    workBuff = Test_PalMalloc(workBuffSize);
    TE_ASSERT(workBuff);


    /* User A -
     * Initialise context with:
     * user A's private and public keys,
     * user B's public key
     * ID's of both users.
     * The context needs to be saved until the common key is derived.
     * */
    TE_ASSERT_PASS( CC_Sm2KeyExchangeContext_init (&Sm2KeContext_a,
                                                   workBuff, workBuffSize,
                                                   &sm2_vec->pubKey_UserA, &sm2_vec->privKey_UserA,
                                                   &sm2_vec->pubKey_UserB,
                                                   sm2_vec->id_UserA, sm2_vec->idSize_UserA,
                                                   sm2_vec->id_UserB, sm2_vec->idSize_UserB,
                                                   1U, /* initiator */
                                                   sm2_vec->confirmationrequired_UserA) , CC_OK);

    /* User B -
     * Initialise context with:
     * user B's private and public keys,
     * user A's public key
     * ID's of both users
     * The context needs to be saved until the common key is derived.
     * */
    TE_ASSERT_PASS( CC_Sm2KeyExchangeContext_init (&Sm2KeContext_b,
                                                   workBuff, workBuffSize,
                                                   &sm2_vec->pubKey_UserB, &sm2_vec->privKey_UserB,
                                                   &sm2_vec->pubKey_UserA,
                                                   sm2_vec->id_UserB, sm2_vec->idSize_UserB,
                                                   sm2_vec->id_UserA, sm2_vec->idSize_UserA,
                                                   0U, /* not initiator */
                                                   sm2_vec->confirmationrequired_UserB) , CC_OK);

    /* User A -
     * Calculate a random point. */
    TE_ASSERT_PASS( CC_Sm2CalculateECPoint (Tests_RndGenerateVectorConst,
                                            (void *)sm2_vec->randomSeed_UserA,
                                            &Sm2KeContext_a,
                                            &randomPoint_UserA ) , CC_OK);

    /* User B-
     * Calculate a random point. */
    TE_ASSERT_PASS( CC_Sm2CalculateECPoint (Tests_RndGenerateVectorConst,
                                            (void *)sm2_vec->randomSeed_UserB,
                                            &Sm2KeContext_b,
                                            &randomPoint_UserB ) , CC_OK);
    /* User B-
     * Calculate the shared secret with the random point previously calculated by user B (saved in the context) and the
     * random point that was received from A. */
    TE_ASSERT_PASS( CC_Sm2CalculateSharedSecret(&Sm2KeContext_b,
                                                &randomPoint_UserA, /* received from A */
                                                outConfValue_UserB, &outConfValueSize_UserB) , CC_OK);
    /* User A-
     * Calculate the shared secret with the random point previously calculated by user A (saved in the context) and the
     * random point that was received from B. */
    TE_ASSERT_PASS( CC_Sm2CalculateSharedSecret(&Sm2KeContext_a,
                                                &randomPoint_UserB, /* received from B */
                                                outConfValue_UserA, &outConfValueSize_UserA) , CC_OK);

    /* Verify the confirmation value is correct */
    TE_ASSERT_PASS( CC_Sm2Confirmation (&Sm2KeContext_a, outConfValue_UserB, outConfValueSize_UserB) , CC_OK);
    TE_ASSERT_PASS( CC_Sm2Confirmation (&Sm2KeContext_b, outConfValue_UserA, outConfValueSize_UserA) , CC_OK);

    /* User A -
     * derive the common key */
    TE_ASSERT_PASS( CC_Sm2Kdf(&Sm2KeContext_a,
                              sm2_vec->requiredKeySizeInBits,
                              sm2_vec->outputKey_UserA, &sm2_vec->outputKeySize_UserA) , CC_OK);

    /* User B -
     * derive the common key */
    TE_ASSERT_PASS( CC_Sm2Kdf(&Sm2KeContext_b,
                              sm2_vec->requiredKeySizeInBits,
                              sm2_vec->outputKey_UserB, &sm2_vec->outputKeySize_UserB) , CC_OK);


    /* Finish performance measurement */
    TE_perfCloseEntry(cookie);

bail:
    if (workBuff != NULL) {
        Test_PalFree(workBuff);
        workBuff = NULL;
    }

    return res;
}

static TE_rc_t sm2_ke_verify(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    Sm2KeyExchangeVector_t *sm2_vec = (Sm2KeyExchangeVector_t *)pContext;

    /* Verify results */
    /* compare output of A and B - key size */
    TE_ASSERT( sm2_vec->referenceKeySize == sm2_vec->outputKeySize_UserA);
    TE_ASSERT( sm2_vec->referenceKeySize == sm2_vec->outputKeySize_UserB);

    /* compare output of A and B - key buffer */
    TE_ASSERT( memcmp (sm2_vec->outputKey_UserA, sm2_vec->referenceKey, sm2_vec->referenceKeySize) == 0);
    TE_ASSERT( memcmp (sm2_vec->outputKey_UserB, sm2_vec->referenceKey, sm2_vec->referenceKeySize) == 0);

    goto bail;
bail:
    return res;
}

static TE_rc_t sm2_ke_clean(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    TE_UNUSED(pContext);

    goto bail;
bail:
    return res;
}

/******************************************************************
 * Public
 ******************************************************************/
int TE_init_sm2_ke_test(void)
{
    TE_rc_t res = TE_RC_SUCCESS;

    TE_perfEntryInit("sm2", "key exchange - 2 parties");

    TE_ASSERT(TE_registerFlow("sm2",
                              "key exchange",
                              "2 parties",
                              sm2_ke_prepare,
                              sm2_ke_execute_2_parties,
                              sm2_ke_verify,
                              sm2_ke_clean,
                              &sm2_vector) == TE_RC_SUCCESS);


    goto bail;

bail:
    return res;
}

