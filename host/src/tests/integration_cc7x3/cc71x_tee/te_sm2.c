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
#define SM2_TV_MAX_DATA_SIZE    14
#define SM2_TV_DIGEST_SIZE      32

/******************************************************************
 * Enums
 ******************************************************************/

/******************************************************************
 * Types
 ******************************************************************/

typedef struct Sm2SignVector_t{
    /* Input */
    const char* id;
    size_t      idSize;
    uint8_t     privateKeyInBytes[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesX[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     publicKeyInBytesY[CC_SM2_MODULE_LENGTH_IN_BYTES];
    uint8_t     randomSeed[SM2_TV_DIGEST_SIZE];
    uint8_t     dataIn[SM2_TV_MAX_DATA_SIZE];
    /* Intermediate results and their sizes */
    CCEcpkiUserPrivKey_t    privKey;
    CCEcpkiUserPublKey_t    pubKey;
    size_t      privateKeySize;
    size_t      pubKeySize;
    uint32_t    msgDigest[CC_SM3_RESULT_SIZE_IN_WORDS];
    size_t      msgDigestSize;

    /* Output */
    uint8_t     dataOut[2 * SM2_TV_DIGEST_SIZE];
    size_t      dataInSize;
    /* Reference */
    uint32_t    msgDigestRef[CC_SM3_RESULT_SIZE_IN_WORDS];
    size_t      msgDigestRefSize;
    uint8_t     dataRef[2 * SM2_TV_DIGEST_SIZE];
    size_t      dataRefSize;
}Sm2SignVector_t;
/******************************************************************
 * Externs
 ******************************************************************/

/******************************************************************
 * Globals
 ******************************************************************/


static Sm2SignVector_t sm2_vector = {
    /* Input */
    .id = "ALICE123@YAHOO.COM",
    .idSize = 18,
    .privateKeyInBytes = { /*128B2FA8 BD433C6C 068C8D80 3DFF7979 2A519A55 171B1B65 0C23661D 15897263 */
            0x12, 0x8B, 0x2F, 0xA8, 0xBD, 0x43, 0x3C, 0x6C,
            0x06, 0x8C, 0x8D, 0x80, 0x3D, 0xFF, 0x79, 0x79,
            0x2A, 0x51, 0x9A, 0x55, 0x17, 0x1B, 0x1B, 0x65,
            0x0C, 0x23, 0x66, 0x1D, 0x15, 0x89, 0x72, 0x63
    },
    .privateKeySize = 32,
    .publicKeyInBytesX = { /*0AE4C779 8AA0F119 471BEE11 825BE462 02BB79E2 A5844495 E97C04FF 4DF2548A*/
            0x0A, 0xE4, 0xC7, 0x79, 0x8A, 0xA0, 0xF1, 0x19,
            0x47, 0x1B, 0xEE, 0x11, 0x82, 0x5B, 0xE4, 0x62,
            0x02, 0xBB, 0x79, 0xE2, 0xA5, 0x84, 0x44, 0x95,
            0xE9, 0x7C, 0x04, 0xFF, 0x4D, 0xF2, 0x54, 0x8A
    },
    .publicKeyInBytesY = { /*7C0240F8 8F1CD4E1 6352A73C 17B7F16F 07353E53 A176D684 A9FE0C6B B798E857*/
            0x7C, 0x02, 0x40, 0xF8, 0x8F, 0x1C, 0xD4, 0xE1,
            0x63, 0x52, 0xA7, 0x3C, 0x17, 0xB7, 0xF1, 0x6F,
            0x07, 0x35, 0x3E, 0x53, 0xA1, 0x76, 0xD6, 0x84,
            0xA9, 0xFE, 0x0C, 0x6B, 0xB7, 0x98, 0xE8, 0x57
    },
    .pubKeySize = 65, /* 32 bytes for x, 32 bytes for y, 1 byte for compression flag */
    .dataIn = {
            0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20,
            0x64, 0x69, 0x67, 0x65, 0x73, 0x74
    },
    .dataInSize = 14,
    .msgDigestSize = CC_SM3_RESULT_SIZE_IN_WORDS,
    .randomSeed ={
            0x6C, 0xB2, 0x8D, 0x99, 0x38, 0x5C, 0x17, 0x5C,
            0x94, 0xF9, 0x4E, 0x93, 0x48, 0x17, 0x66, 0x3F,
            0xC1, 0x76, 0xD9, 0x25, 0xDD, 0x72, 0xB7, 0x27,
            0x26, 0x0D, 0xBA, 0xAE, 0x1F, 0xB2, 0xF9, 0x6F
    },

    /* Expected intermediate results */
    .msgDigestRef = {
            0x52F524B5, 0xB0B882CD, 0x006E4728, 0xB17F375C,
            0xFCE6879A, 0xBB482D68, 0xD9E3425D, 0x76FEEFB9
    },
    .msgDigestRefSize = CC_SM3_RESULT_SIZE_IN_WORDS,

    /* Expected results */
    .dataRef = { /* R */
            0x40, 0xF1, 0xEC, 0x59, 0xF7, 0x93, 0xD9, 0xF4,
            0x9E, 0x09, 0xDC, 0xEF, 0x49, 0x13, 0x0D, 0x41,
            0x94, 0xF7, 0x9F, 0xB1, 0xEE, 0xD2, 0xCA, 0xA5,
            0x5B, 0xAC, 0xDB, 0x49, 0xC4, 0xE7, 0x55, 0xD1,
            /* S */
            0x6F, 0xC6, 0xDA, 0xC3, 0x2C, 0x5D, 0x5C, 0xF1,
            0x0C, 0x77, 0xDF, 0xB2, 0x0F, 0x7C, 0x2E, 0xB6,
            0x67, 0xA4, 0x57, 0x87, 0x2F, 0xB0, 0x9E, 0xC5,
            0x63, 0x27, 0xA6, 0x7E, 0xC7, 0xDE, 0xEB, 0xE7
    },
    .dataRefSize = 2 * SM2_TV_DIGEST_SIZE,
};

/******************************************************************
 * Static Prototypes
 ******************************************************************/
static TE_rc_t sm2_prepare(void *pContext);
static TE_rc_t sm2_execute_sign(void *pContext);
static TE_rc_t sm2_verify_sign(void *pContext);
static TE_rc_t sm2_clean(void *pContext);

/******************************************************************
 * Static functions
 ******************************************************************/
static int Tests_RndGenerateVectorConst( void           *rngState_vptr,    /*in*/
                                      uint8_t        *out_ptr,          /*out*/
                                      size_t         outSizeBytes)      /*in*/
{
    CCError_t  error = CC_OK;
    uint8_t* pData = (uint8_t*)rngState_vptr;

    if (outSizeBytes != 32)
        return 1;

    memcpy (out_ptr, pData, outSizeBytes);
    return error;
}


static TE_rc_t sm2_prepare(void *pContext)
{
    CCEcpkiBuildTempData_t tmpData;
    TE_rc_t res = TE_RC_SUCCESS;
    const CCEcpkiDomain_t* pSm2Domain = CC_EcpkiGetSm2Domain();
    Sm2SignVector_t *sm2_vec = (Sm2SignVector_t *)pContext;
    uint8_t publicKeyInBytes[ 2 * CC_SM2_MODULE_LENGTH_IN_BYTES + 1 /*1 bit for compression flag*/];

    /* Set the public key data in 1 array:  compression flag || x || y */
    publicKeyInBytes[0] = CC_EC_PointUncompressed;
    memcpy(publicKeyInBytes + 1, sm2_vec->publicKeyInBytesX, sm2_vec->pubKeySize/2);
    memcpy(publicKeyInBytes + 1 + sm2_vec->pubKeySize/2,
                  sm2_vec->publicKeyInBytesY, sm2_vec->pubKeySize/2);


    /*
     * Build public and private keys -
     * input the keys as bytes array (public key:  compression flag || x || y)
     * output the keys as structures, which are used as inputs to all SM2 or ECC operations.
     * The public key is needed for both SIGN and VERIFY operations.
     * The private key is needed only for SIGN operations.
     * */
    TE_ASSERT_PASS(CC_EcpkiPublKeyBuildAndCheck (pSm2Domain, publicKeyInBytes, sm2_vec->pubKeySize,
                                        0/*CheckPointersAndSizesOnly*/, &sm2_vec->pubKey, &tmpData) , CC_OK);

    TE_ASSERT_PASS(CC_EcpkiPrivKeyBuild(pSm2Domain, sm2_vec->privateKeyInBytes,  sm2_vec->privateKeySize, &sm2_vec->privKey) , CC_OK);

    goto bail;
bail:
    return res;
}

static TE_rc_t sm2_execute_sign(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    TE_perfIndex_t cookie;
    Sm2SignVector_t *sm2_vec = (Sm2SignVector_t *)pContext;
    uint8_t *workBuff = NULL;
    size_t workBuffSize;

    /* Start performance measurement */
    cookie = TE_perfOpenNewEntry("sm2", "sign");

    workBuffSize = 2 + CC_SM2_MODULE_LENGTH_IN_BYTES*4 + CC_SM2_ORDER_LENGTH_IN_BYTES*2 +
            sm2_vec->idSize + sm2_vec->dataInSize;
    workBuff = Test_PalMalloc(workBuffSize);
    TE_ASSERT(workBuff);

    /* Test SM2 signature integrated API */
    TE_ASSERT_PASS( CC_Sm2ComputeMessageDigest(&sm2_vec->pubKey,
                                          sm2_vec->id, sm2_vec->idSize,
                                          sm2_vec->dataIn, sm2_vec->dataInSize,
                                          workBuff, workBuffSize,
                                          sm2_vec->msgDigest, &(sm2_vec->msgDigestSize) ) , CC_OK);


    TE_ASSERT_PASS( CC_Sm2Sign ( Tests_RndGenerateVectorConst,
                            (void *)sm2_vec->randomSeed,
                            &sm2_vec->privKey,
                            sm2_vec->msgDigest,
                            sm2_vec->msgDigestSize,
                            sm2_vec->dataOut,
                            &sm2_vec->dataRefSize), CC_OK);

    /* Finish performance measurement */
    TE_perfCloseEntry(cookie);

bail:
    if (workBuff != NULL) {
        Test_PalFree(workBuff);
        workBuff = NULL;
    }

    return res;
}

static TE_rc_t sm2_verify_sign(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    Sm2SignVector_t *sm2_vec = (Sm2SignVector_t *)pContext;

    /* Verify intermediate result */
    TE_ASSERT( sm2_vec->msgDigestSize == sm2_vec->msgDigestRefSize);
    TE_ASSERT( memcmp((uint8_t *)&(sm2_vec->msgDigest), (uint8_t *)&(sm2_vec->msgDigestRef), sm2_vec->msgDigestRefSize) == 0);

    /* Verify result */
    TE_ASSERT( memcmp((uint8_t *)&(sm2_vec->dataOut), (uint8_t *)&(sm2_vec->dataRef), sm2_vec->dataRefSize) == 0);

    goto bail;
bail:
    return res;
}

static TE_rc_t sm2_execute_verify(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    TE_perfIndex_t cookie;
    Sm2SignVector_t *sm2_vec = (Sm2SignVector_t *)pContext;
    uint8_t *workBuff = NULL;
    size_t workBuffSize;

    /* Start performance measurement */
    cookie = TE_perfOpenNewEntry("sm2", "verify");

    workBuffSize = 2 + CC_SM2_MODULE_LENGTH_IN_BYTES*4 + CC_SM2_ORDER_LENGTH_IN_BYTES*2 +
            sm2_vec->idSize + sm2_vec->dataInSize;
    workBuff = Test_PalMalloc(workBuffSize);
    TE_ASSERT(workBuff);

    /* Test SM2 verification integrated API */
    TE_ASSERT_PASS( CC_Sm2ComputeMessageDigest(&sm2_vec->pubKey,
                                          sm2_vec->id, sm2_vec->idSize,
                                          sm2_vec->dataIn, sm2_vec->dataInSize,
                                          workBuff, workBuffSize,
                                          sm2_vec->msgDigest /* output */, &(sm2_vec->msgDigestSize) ) , CC_OK);


    TE_ASSERT_PASS( CC_Sm2Verify(&sm2_vec->pubKey,
                                 sm2_vec->dataRef,
                                 sm2_vec->dataRefSize,
                                 sm2_vec->msgDigest,
                                 sm2_vec->msgDigestRefSize), CC_OK);

    /* Finish performance measurement */
    TE_perfCloseEntry(cookie);

bail:
    if (workBuff != NULL) {
        Test_PalFree(workBuff);
        workBuff = NULL;
    }

    return res;
}
static TE_rc_t sm2_clean(void *pContext)
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
int TE_init_sm2_test(void)
{
    TE_rc_t res = TE_RC_SUCCESS;

    TE_perfEntryInit("sm2", "sign");
    TE_perfEntryInit("sm2", "verify");

    TE_ASSERT(TE_registerFlow("sm2",
                              "sign",
                              "none",
                              sm2_prepare,
                              sm2_execute_sign,
                              sm2_verify_sign,
                              sm2_clean,
                              &sm2_vector) == TE_RC_SUCCESS);

    TE_ASSERT(TE_registerFlow("sm2",
                              "verify",
                              "none",
                              sm2_prepare,
                              sm2_execute_verify,
                              sm2_clean, /* No results to verify */
                              sm2_clean,
                              &sm2_vector) == TE_RC_SUCCESS);
    goto bail;

bail:
    return res;
}

