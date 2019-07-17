/****************************************************************************
* The confidential and proprietary information contained in this file may   *
* only be used by a person authorised under and to the extent permitted     *
* by a subsisting licensing agreement from Arm Limited (or its affiliates). *
*     (C) COPYRIGHT [2018] Arm Limited (or its affiliates).                 *
*         ALL RIGHTS RESERVED                                               *
* This entire notice must be reproduced on all copies of this file          *
* and copies of this file may only be made by a person if such person is    *
* permitted to do so under the terms of a subsisting license agreement      *
* from Arm Limited (or its affiliates).                                     *
*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "test_engine.h"

#include "test_pal_mem.h"
#include "cc_trng_fe.h"

/******************************************************************
 * Defines
 ******************************************************************/
#define TRNG_FE_DATA_SIZE    32

/******************************************************************
 * Types
 ******************************************************************/
typedef struct TrngFeVector_t{
    uint8_t dataBuff1[TRNG_FE_DATA_SIZE];
    uint8_t dataBuff2[TRNG_FE_DATA_SIZE];
} TrngFeVector_t;


/******************************************************************
 * Externs
 ******************************************************************/

/******************************************************************
 * Globals
 ******************************************************************/
static TrngFeVector_t *pTrngfe_vec;

/******************************************************************
 * Static Prototypes
 ******************************************************************/
static TE_rc_t trngfe_prepare(void *pContext);
static TE_rc_t trngfe_execute(void *pContext);
static TE_rc_t trngfe_verify(void *pContext);
static TE_rc_t trngfe_clean(void *pContext);

/******************************************************************
 * Static functions
 ******************************************************************/
static TE_rc_t trngfe_prepare(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    TE_UNUSED(pContext);

    pTrngfe_vec = Test_PalDMAContigBufferAlloc(sizeof(TrngFeVector_t));
    TE_ASSERT (pTrngfe_vec != NULL);

    goto bail;
bail:
    return res;
}

static TE_rc_t trngfe_execute(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;
    TE_perfIndex_t cookie;

    TE_UNUSED(pContext);

    TE_ASSERT (pTrngfe_vec != NULL);

    cookie = TE_perfOpenNewEntry("trngfe", "random");

    /* test TRNG FE  API */
    TE_ASSERT( CC_TrngEntropyGet(TRNG_FE_DATA_SIZE*CC_BITS_IN_BYTE,
                                 pTrngfe_vec->dataBuff1,
                                 TRNG_FE_DATA_SIZE) == CC_OK);

    TE_perfCloseEntry(cookie);


    /* test TRNG FE  API */
    TE_ASSERT( CC_TrngEntropyGet(TRNG_FE_DATA_SIZE*CC_BITS_IN_BYTE,
                                 pTrngfe_vec->dataBuff2,
                                 TRNG_FE_DATA_SIZE) == CC_OK);


    goto bail;

bail:
    return res;
}

static TE_rc_t trngfe_verify(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;

    TE_UNUSED(pContext);

    TE_ASSERT (pTrngfe_vec != NULL);

    TE_ASSERT( memcmp(pTrngfe_vec->dataBuff1, pTrngfe_vec->dataBuff2, TRNG_FE_DATA_SIZE) != 0);

    goto bail;
bail:
    return res;
}

static TE_rc_t trngfe_clean(void *pContext)
{
    TE_rc_t res = TE_RC_SUCCESS;

    TE_UNUSED(pContext);

    if (pTrngfe_vec != NULL) {
        Test_PalDMAContigBufferFree(pTrngfe_vec);
    }

    goto bail;
bail:
    return res;
}

/******************************************************************
 * Public
 ******************************************************************/
int TE_init_trngfe_test(void)
{
    TE_rc_t res = TE_RC_SUCCESS;

    TE_perfEntryInit("trngfe", "random");

    TE_ASSERT(TE_registerFlow("trngfe",
                               "none",
                               "random",
                               trngfe_prepare,
                               trngfe_execute,
                               trngfe_verify,
                               trngfe_clean,
                               NULL) == TE_RC_SUCCESS);

    goto bail;

bail:
	return res;
}

