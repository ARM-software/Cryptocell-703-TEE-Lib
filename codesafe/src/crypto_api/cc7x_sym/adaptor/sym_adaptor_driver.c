/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */
#define CC_PAL_LOG_CUR_COMPONENT CC_LOG_MASK_CC_SYM_DRIVER

#include "cc_pal_types.h"
#include "cc_plat.h"
#include "cc_pal_mem.h"
#include "cc_pal_dma.h"
#include "cc_pal_log.h"
#include "cc_pal_mutex.h"
#include "cc_pal_abort.h"
#include "cc_sym_error.h"
#include "cc_plat.h"
#include "cc_crypto_ctx.h"
#include "completion.h"
#include "cc_pal_perf.h"
#include "sym_crypto_driver.h"
#include "sym_adaptor_driver.h"
#include "sym_adaptor_driver_int.h"
#include "cc_sram_map.h"
#include "cc_hal.h"
#include "cc_lli_defs.h"
#include "cc_int_general_defs.h"

#define MAX_DLLI_BLOCK_SIZE                 ((1<<DLLI_SIZE_BIT_SIZE)-1)
#define MAX_MLLI_ENTRY_SIZE                 (1<<LLI_SIZE_BIT_SIZE)

#define INPLACE           1
#define NOT_INPLACE       0

/******************************************************************************
 *              TYPES
 ******************************************************************************/
typedef struct dmaBuffBlocksInfo_t {
    /* number of entries in pBlockEntry member */
    uint32_t numOfBlocks;
    /* an array of blocks and their sizes. the block are fragments of the data */
    CCPalDmaBlockInfo_t pBlockEntry[LLI_MAX_NUM_OF_ENTRIES];
} dmaBuffBlocksInfo_t;

typedef struct interDmaBuildBuffer_t {
    /* stores data needed to construct and map the MLLI table. */
    mlliTable_t devBuffer;
    /* used to for optimisation. when data is less than DLLI_OPTIMIZED_BUFF_SIZE,
     * copy data to since contiguous buffer and use DLLI. */
    uint8_t * optimizationBuff;
    /* holds an array of fragmented blocks of the data */
    dmaBuffBlocksInfo_t blocksList;
    /* a handle to the main data mapping. used to optimized the un-mapping and freeing process */
    CC_PalDmaBufferHandle buffMainH;
    /* a handle to the temp data buffer mapping. used to optimized the un-mapping and freeing process */
    CC_PalDmaBufferHandle buffOptH;
    /* a handle to the compiled MLLI table data. used to optimized the un-mapping and freeing process */
    CC_PalDmaBufferHandle buffMlliH;
} interDmaBuildBuffer_t;

typedef enum eDmaBuiltFlag_t{
    DMA_BUILT_FLAG_NONE = 0x0,
    DMA_BUILT_FLAG_BI_DIR = 0x1,
    DMA_BUILT_FLAG_INPUT_BUFF = 0x2,
    DMA_BUILT_FLAG_OUTPUT_BUFF = 0x4
} eDmaBuiltFlag_t;

typedef enum eUnmapFlag_t {
    UNMAP_FLAG_NONE = 0x0,
    UNMAP_FLAG_CONTIG_DLLI = 0x1,
    UNMAP_FLAG_SMALL_SIZE_DLLI = 0x2,
    UNMAP_FLAG_MLLI_MAIN = 0x4,
    UNMAP_FLAG_MLLI_TABLE = 0x10
} eUnmapFlag_t;

/******************************************************************************
 *				GLOBALS
 ******************************************************************************/
static interDmaBuildBuffer_t g_dmaInBuildBuffH;    // internal buffer for dma, built for to device or bi-directional
static interDmaBuildBuffer_t g_dmaOutBuildBuffH;    // internal buffer for dma, built for from device

extern CC_PalMutex CCSymCryptoMutex;

/******************************************************************************
 *				PRIVATE FUNCTIONS
 ******************************************************************************/

/*!
 * The function returns the context size according to the algorithm type
 *
 *
 * \param pCtx
 *
 * \return int The size of the context in bytes.
 */
static int getCtxSize(enum drv_crypto_alg alg)
{
    uint32_t ctxSize; /*size in words*/

    switch (alg) {
        case DRV_CRYPTO_ALG_DES:
        case DRV_CRYPTO_ALG_AES:
        case DRV_CRYPTO_ALG_SM4:
            /* copied fields block_state + key + xex_key */
            ctxSize = CC_AES_BLOCK_SIZE + CC_AES_KEY_SIZE_MAX + CC_AES_KEY_SIZE_MAX;
            break;
        case DRV_CRYPTO_ALG_HMAC:
            /* digest + k0 size + CurrentDigestedLength */
            ctxSize = CC_DIGEST_SIZE_MAX + CC_HMAC_BLOCK_SIZE_MAX
                            + DRV_HASH_LENGTH_WORDS * sizeof(uint32_t);
            break;
        case DRV_CRYPTO_ALG_HASH:
        case DRV_CRYPTO_ALG_SM3:
            /* digest + CurrentDigestedLength */
            ctxSize = CC_DIGEST_SIZE_MAX + DRV_HASH_LENGTH_WORDS * sizeof(uint32_t);
            break;

        case DRV_CRYPTO_ALG_BYPASS:
            ctxSize = sizeof(uint32_t);
            break;
        case DRV_CRYPTO_ALG_AEAD:
            /* block_state + mac_state + key */
            ctxSize = CC_AES_BLOCK_SIZE + CC_AES_BLOCK_SIZE + CC_AES_KEY_SIZE_MAX;
            break;
        default:
            ctxSize = 0;
            break;
    }
    return ctxSize;
}

static void clearDmaBuildBuffers(interDmaBuildBuffer_t *pDmaBuildBuff)
{
    if (NULL == pDmaBuildBuff) {
        return;
    }
    CC_PalMemSetZero(pDmaBuildBuff->blocksList.pBlockEntry,
                     LLI_MAX_NUM_OF_ENTRIES * sizeof(CCPalDmaBlockInfo_t));
    pDmaBuildBuff->blocksList.numOfBlocks = 0;

    CC_PalMemSetZero((uint8_t * )&pDmaBuildBuff->devBuffer.mlliBlockInfo,
                     sizeof(CCPalDmaBlockInfo_t));

    if (pDmaBuildBuff->optimizationBuff != NULL) {
        CC_PalMemSetZero(pDmaBuildBuff->optimizationBuff, DLLI_OPTIMIZED_BUFF_SIZE);
    }
    if (pDmaBuildBuff->devBuffer.pLliEntry != NULL) {
        CC_PalMemSetZero(pDmaBuildBuff->devBuffer.pLliEntry, LLI_MAX_NUM_OF_ENTRIES * sizeof(lliInfo_t));
    }
}

static void freeDmaBuildBuffers(interDmaBuildBuffer_t *pDmaBuildBuff)
{
    if (NULL == pDmaBuildBuff) {
        return;
    }
    if (pDmaBuildBuff->optimizationBuff != NULL) {
        CC_PalDmaContigBufferFree(DLLI_OPTIMIZED_BUFF_SIZE, pDmaBuildBuff->optimizationBuff);
        pDmaBuildBuff->optimizationBuff = NULL;
    }
    if (pDmaBuildBuff->devBuffer.pLliEntry != NULL) {
        CC_PalDmaContigBufferFree(LLI_MAX_NUM_OF_ENTRIES * sizeof(lliInfo_t),
                                  (uint8_t *) pDmaBuildBuff->devBuffer.pLliEntry);
        pDmaBuildBuff->devBuffer.pLliEntry = NULL;
    }
}

uint32_t allocDmaBuildBuffers(interDmaBuildBuffer_t *pDmaBuildBuff)
{
    uint32_t rc = 0;
    uint8_t *tmpBuff = NULL;

    if (NULL == pDmaBuildBuff) {
        return CC_RET_INVARG;
    }
    tmpBuff = (uint8_t *) pDmaBuildBuff->devBuffer.pLliEntry;
    rc = CC_PalDmaContigBufferAllocate(LLI_MAX_NUM_OF_ENTRIES * sizeof(lliInfo_t), &tmpBuff);
    if (rc != 0) {
        return CC_RET_NOMEM;
    }
    if (!IS_ALIGNED((unsigned long )tmpBuff, 4))
        return CC_RET_INVARG_BAD_ADDR;

    /* casting to void to avoid compilation error , address must be aligned to word , otherwise an error will return */
    pDmaBuildBuff->devBuffer.pLliEntry = (lliInfo_t *) ((void*) tmpBuff);

    rc = CC_PalDmaContigBufferAllocate(DLLI_OPTIMIZED_BUFF_SIZE, &pDmaBuildBuff->optimizationBuff);
    if (rc != 0) {
        freeDmaBuildBuffers(pDmaBuildBuff);
        return CC_RET_NOMEM;
    }
    clearDmaBuildBuffers(pDmaBuildBuff);
    return CC_RET_OK;
}

/**
 * @brief   fills mlli entries based on physical addresses and sizes from blockList
 *
 *
 * @param[in] pUsrBlockList - list of blocks
 * @param[out] pDevBuffer - mlli list to fill
 *
 * @return success/fail
 */
static uint32_t buildMlliTable(mlliTable_t *pDevBuffer, dmaBuffBlocksInfo_t *pUsrBlockList)
{
    uint32_t i = 0;
    uint32_t mlliEntries = 0;
    CCPalPerfData_t perfIdx = 0;

    if ((NULL == pDevBuffer) || (NULL == pUsrBlockList)) {
        return CC_RET_INVARG;
    }

    mlliEntries = pUsrBlockList->numOfBlocks;

    // calculate mlli table size,
    pDevBuffer->mlliBlockInfo.blockSize = mlliEntries * sizeof(lliInfo_t);
    CC_PAL_PERF_OPEN_NEW_ENTRY(perfIdx, PERF_TEST_TYPE_MLLI_BUILD);

    // fill other mlli table entries. Note that pUsrBlockList->pBlockEntry
    for (i = 0; i < mlliEntries; i++) {

        // Verify blockSize is not bigger than MLLI
        if (pUsrBlockList->pBlockEntry[i].blockSize > MAX_MLLI_ENTRY_SIZE) {
            CC_PalMemSetZero(pDevBuffer->pLliEntry, LLI_MAX_NUM_OF_ENTRIES * sizeof(lliInfo_t));
            return 1;
        }
        // set physical address of MLLI entry
        LLI_SET_ADDR(pDevBuffer->pLliEntry[i].lliEntry, pUsrBlockList->pBlockEntry[i].blockPhysAddr);
        // set size of MLLI entry
        LLI_SET_SIZE(pDevBuffer->pLliEntry[i].lliEntry, pUsrBlockList->pBlockEntry[i].blockSize);

        // copy lliEntry to MLLI table - LE/BE must be considered
        pDevBuffer->pLliEntry[i].lliEntry[LLI_WORD0_OFFSET] = SET_WORD_LE(pDevBuffer->pLliEntry[i].lliEntry[LLI_WORD0_OFFSET]);
        pDevBuffer->pLliEntry[i].lliEntry[LLI_WORD1_OFFSET] = SET_WORD_LE(pDevBuffer->pLliEntry[i].lliEntry[LLI_WORD1_OFFSET]);
    }

    CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_MLLI_BUILD);

    return 0;
}

/**
 * @brief   sets user buffer into dma buffer to be used by HW
 *
 *
 * @param[in] pUsrBuffer - address of the buffer allocated by user
 * @param[in] usrBuffSize - data direction: into device, from device or bidirectional
 * @param[in] direction - bi-directional/to/from device
 * @param[out] pDmaBuff - dma buffer to be used by HW
 * @param[out] pInterBuildBuff - mlli list,  page list abd cookies used to build the dms bufffer
 *
 * @return success/fail
 */
static uint32_t buildDmaFromDataPtr(uint8_t *pUsrBuffer,
                                    size_t usrBuffSize,
                                    CCPalDmaBufferDirection_t direction,
                                    DmaBuffer_s *pDmaBuff,
                                    interDmaBuildBuffer_t *pInterBuildBuff)
{
    uint32_t rc = 0;
    uint32_t numOfBlocks = 0;
    mlliTable_t *pDevBuffer = NULL;
    dmaBuffBlocksInfo_t *pUsrBlockList = NULL;
    uint8_t *pOptBuff = NULL;
    uint32_t errorClearFlag = 0;

    CC_PAL_LOG_INFO("%s:%d: cache dir[%s] usrBuffSize[%zu]\n", __func__, __LINE__,
                       (const char *[]){ "none", "to", "from", "bi" }[direction], usrBuffSize);

    // check inputs
    if ((NULL == pInterBuildBuff) || (NULL == pDmaBuff)) {
        CC_PAL_LOG_ERR("invalid parameters\n");
        return CC_RET_INVARG;
    }

    // first check if buffer is NULL, build simple empty dma buffer
    if ((NULL == pUsrBuffer) || (0 == usrBuffSize)) {
        SET_DMA_BUFF_WITH_NULL(pDmaBuff);
        return 0;
    }

    pDevBuffer = &pInterBuildBuff->devBuffer;
    pUsrBlockList = &pInterBuildBuff->blocksList;
    pOptBuff = pInterBuildBuff->optimizationBuff;
    pUsrBlockList->numOfBlocks = LLI_MAX_NUM_OF_ENTRIES;

    // second case, if buffer is contiguous build DLLI
    if (CC_PalIsDmaBufferContiguous(pUsrBuffer, usrBuffSize)) {

        // Verify size of max DLLI
        if (usrBuffSize > MAX_DLLI_BLOCK_SIZE) {
            return CC_RET_NOMEM;
        }
        pUsrBlockList->numOfBlocks = SINGLE_BLOCK_ENTRY;

        rc = CC_PalDmaBufferMap(pUsrBuffer,
                                usrBuffSize,
                                direction,
                                &pUsrBlockList->numOfBlocks,
                                pUsrBlockList->pBlockEntry,
                                &pInterBuildBuff->buffMainH);

        if (rc != 0) {
            CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for dlli contig user buffer 0x%x\n", rc);
            rc = CC_RET_NOMEM;
            goto endError_dataPtrToDma;
        }
        // if case numOfBlocks returned bigger than 1, we declare error
        if (pUsrBlockList->numOfBlocks > SINGLE_BLOCK_ENTRY) {
            errorClearFlag = UNMAP_FLAG_CONTIG_DLLI;
            CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for contig mem numOfBlocks > 1\n");
            rc = CC_RET_OSFAULT;
            goto endError_dataPtrToDma;
        }
        SET_DMA_BUFF_WITH_DLLI(pDmaBuff, pUsrBlockList->pBlockEntry[0].blockPhysAddr, usrBuffSize);
        return 0;
    }

    // in case buffer is not contiguous:
    // if buffer size smaller than  DLLI_OPTIMIZED_BUFF_SIZE:
    // copy user buffer to optimizedBuff to improve performance and build DLLI
    if (usrBuffSize < DLLI_OPTIMIZED_BUFF_SIZE) {
        // copy userBuffer to optimizedBuff
        if ((CC_PAL_DMA_DIR_TO_DEVICE == direction) || (CC_PAL_DMA_DIR_BI_DIRECTION == direction)) {
            CC_PalMemCopy(pOptBuff, pUsrBuffer, usrBuffSize);
        }
        // map optimizedBuff to get physical address and lock+invalidate
        pUsrBlockList->numOfBlocks = SINGLE_BLOCK_ENTRY;
        rc = CC_PalDmaBufferMap(pOptBuff,
                                usrBuffSize,
                                direction,
                                &pUsrBlockList->numOfBlocks,
                                pUsrBlockList->pBlockEntry,
                                &pInterBuildBuff->buffOptH);
        if (rc != 0) {
            CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for dlli optimizedBuff 0x%x\n", rc);
            rc = CC_RET_NOMEM;
            goto endError_dataPtrToDma;
        }
        // if case numOfBlocks returned bigger than 1, we declare error
        if (pUsrBlockList->numOfBlocks > SINGLE_BLOCK_ENTRY) {
            CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for dlli optimizedBuff numOfBlocks > 1\n");
            errorClearFlag = UNMAP_FLAG_SMALL_SIZE_DLLI;
            rc = CC_RET_OSFAULT;
            goto endError_dataPtrToDma;
        }
        SET_DMA_BUFF_WITH_DLLI(pDmaBuff, pUsrBlockList->pBlockEntry[0].blockPhysAddr, usrBuffSize);
        return
                        0;
    }

    pUsrBlockList->numOfBlocks = LLI_MAX_NUM_OF_ENTRIES;
    rc = CC_PalDmaBufferMap(pUsrBuffer,
                            usrBuffSize,
                            direction,
                            &pUsrBlockList->numOfBlocks,
                            pUsrBlockList->pBlockEntry,
                            &pInterBuildBuff->buffMainH);
    if (rc != 0) {
        CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for mlli user buffer 0x%x\n", rc);
        rc = CC_RET_NOMEM;
        goto endError_dataPtrToDma;
    }

    // if case numOfBlocks returned bigger than LLI_MAX_NUM_OF_ENTRIES, we declare error
    if (pUsrBlockList->numOfBlocks > LLI_MAX_NUM_OF_ENTRIES) {
        CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for mlli numOfBlocks > LLI_MAX_NUM_OF_ENTRIES\n");
        errorClearFlag = UNMAP_FLAG_MLLI_MAIN;
        rc = CC_RET_OSFAULT;
        goto endError_dataPtrToDma;
    }

    // build MLLI
    buildMlliTable(pDevBuffer, pUsrBlockList);

    // map MLLI
    numOfBlocks = SINGLE_BLOCK_ENTRY;
    rc = CC_PalDmaBufferMap((uint8_t *) pDevBuffer->pLliEntry,
                            (pUsrBlockList->numOfBlocks) * sizeof(lliInfo_t),
                            CC_PAL_DMA_DIR_BI_DIRECTION,
                            &numOfBlocks,
                            &pDevBuffer->mlliBlockInfo,
                            &pInterBuildBuff->buffMlliH);
    if (rc != 0) {
        CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for mlli table 0x%x\n", rc);
        errorClearFlag = UNMAP_FLAG_MLLI_MAIN;
        rc = CC_RET_NOMEM;
        goto endError_dataPtrToDma;
    }

    // if case numOfBlocks returned bigger than 1, we declare error
    if (numOfBlocks > SINGLE_BLOCK_ENTRY) {
        CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for mlli numOfBlocks > 1\n");
        errorClearFlag = (UNMAP_FLAG_MLLI_MAIN | UNMAP_FLAG_MLLI_TABLE);
        rc = CC_RET_OSFAULT;
        goto endError_dataPtrToDma;
    }

    SET_DMA_BUFF_WITH_MLLI(pDmaBuff,
                           pDevBuffer->mlliBlockInfo.blockPhysAddr,
                           pDevBuffer->mlliBlockInfo.blockSize);
    return 0;

endError_dataPtrToDma:

    if ((UNMAP_FLAG_MLLI_MAIN & errorClearFlag) || (UNMAP_FLAG_CONTIG_DLLI & errorClearFlag)) {
        CC_PalDmaBufferUnmap(pUsrBuffer,
                             usrBuffSize,
                             direction,
                             pUsrBlockList->numOfBlocks,
                             pUsrBlockList->pBlockEntry,
                             pInterBuildBuff->buffMainH);
    }
    if (UNMAP_FLAG_SMALL_SIZE_DLLI & errorClearFlag) {
        CC_PalDmaBufferUnmap(pOptBuff,
                             usrBuffSize,
                             direction,
                             pUsrBlockList->numOfBlocks,
                             pUsrBlockList->pBlockEntry,
                             pInterBuildBuff->buffOptH);
    }
    if (UNMAP_FLAG_MLLI_TABLE & errorClearFlag) {
        CC_PalDmaBufferUnmap((uint8_t *) pDevBuffer->pLliEntry,
                             pUsrBlockList->numOfBlocks * sizeof(lliInfo_t),
                             CC_PAL_DMA_DIR_BI_DIRECTION,
                             SINGLE_BLOCK_ENTRY,
                             &pDevBuffer->mlliBlockInfo,
                             pInterBuildBuff->buffMlliH);
    }
    return rc;

}

/**
 * @brief   sets user buffer into dma buffer to be used by HW
 *
 *
 * @param[in] pUsrBuffer - address of the buffer allocated by user
 * @param[in] usrBuffSize - data direction: into device, from device or bidirectional
 * @param[in] direction - bi-directional/to/from device
 * @param[in] pDmaBuff - dma buffer to be used by HW
 * @param[in] pInterBuildBuff - mlli list,  page list abd cookies used to build the dms bufffer
 *
 * @return success/fail
 */
static uint32_t buildDataPtrFromDma(uint8_t *pUsrBuffer,
                                    size_t usrBuffSize,
                                    CCPalDmaBufferDirection_t direction,
                                    DmaBuffer_s *pDmaBuff,
                                    interDmaBuildBuffer_t *pInterBuildBuff)
{
    uint32_t rc = 0;
    uint8_t *pOptBuff = NULL;
    mlliTable_t *pDevBuffer = NULL;
    dmaBuffBlocksInfo_t *pUsrBlockList = NULL;
    CC_PAL_LOG_INFO("%s:%d: uncache dir[%s] usrBuffSize[%zu]\n", __func__, __LINE__,
                    (const char *[]){ "none", "to", "from", "bi" }[direction], usrBuffSize);

    CC_UNUSED_PARAM(pDmaBuff);

    // check inputs
    if (NULL == pInterBuildBuff) {
        CC_PAL_LOG_ERR("invalid parameters\n");
        return CC_RET_INVARG;
    }
    // first check if buffer is NULL, build simple empty dma buffer
    if ((NULL == pUsrBuffer) || (0 == usrBuffSize)) {
        return 0;
    }

    pDevBuffer = &pInterBuildBuff->devBuffer;
    pUsrBlockList = &pInterBuildBuff->blocksList;
    pOptBuff = pInterBuildBuff->optimizationBuff;

    // second case, if buffer is contiguous build DLLI
    if (CC_PalIsDmaBufferContiguous(pUsrBuffer, usrBuffSize)) {
        rc = CC_PalDmaBufferUnmap(pUsrBuffer,
                                  usrBuffSize,
                                  direction,
                                  pUsrBlockList->numOfBlocks,
                                  pUsrBlockList->pBlockEntry,
                                  pInterBuildBuff->buffMainH);
        goto endError_dmaToDataPtr;
    }

    // in case buffer is not contiguous:
    // if buffer size smaller than  DLLI_OPTIMIZED_BUFF_SIZE:
    // copy user buffer to optimizedBuff to improve performance and build DLLI
    if (usrBuffSize < DLLI_OPTIMIZED_BUFF_SIZE) {
        // map optimizedBuff to get physical address and lock+invalidate
        rc = CC_PalDmaBufferUnmap((uint8_t *) pOptBuff,
                                  usrBuffSize,
                                  direction,
                                  pUsrBlockList->numOfBlocks,
                                  pUsrBlockList->pBlockEntry,
                                  pInterBuildBuff->buffOptH);

        // copy userBuffer to optimizedBuff
        if ((CC_PAL_DMA_DIR_FROM_DEVICE == direction) || (CC_PAL_DMA_DIR_BI_DIRECTION == direction)) {
            CC_PalMemCopy(pUsrBuffer, pOptBuff, usrBuffSize);
        }
        goto endError_dmaToDataPtr;
    }

    // otherwise (buffer size smaller than  DLLI_OPTIMIZED_BUFF_SIZE) build MLLI:
    // unmap the buffer
    rc = CC_PalDmaBufferUnmap(pUsrBuffer,
                              usrBuffSize,
                              direction,
                              pUsrBlockList->numOfBlocks,
                              pUsrBlockList->pBlockEntry,
                              pInterBuildBuff->buffMainH);
    // Unmap MLLI
    rc |= CC_PalDmaBufferUnmap((uint8_t *) pDevBuffer->pLliEntry,
                               pUsrBlockList->numOfBlocks * sizeof(lliInfo_t),
                               CC_PAL_DMA_DIR_BI_DIRECTION,
                               SINGLE_BLOCK_ENTRY,
                               &pDevBuffer->mlliBlockInfo,
                               pInterBuildBuff->buffMlliH);

endError_dmaToDataPtr:

    if (rc != 0) {
        rc = CC_RET_BUSY;
    }
    return rc;

}

static void isCopyCtxRequired(enum drv_crypto_alg alg, int mode, uint8_t *flag)
{
    *flag = 0;

    switch (alg) {
        case DRV_CRYPTO_ALG_AES:
            if (mode == DRV_CIPHER_XCBC_MAC) {
                *flag = 1;
            }
            break;
        case DRV_CRYPTO_ALG_AEAD:
        case DRV_CRYPTO_ALG_DES:
        case DRV_CRYPTO_ALG_HMAC:
        case DRV_CRYPTO_ALG_HASH:
        case DRV_CRYPTO_ALG_SM3:
            *flag = 1;
            break;
        case DRV_CRYPTO_ALG_BYPASS:
            break;
        default:
            break;
    }
}

/******************************************************************************
 *				PUBLIC FUNCTIONS
 ******************************************************************************/

/*!
 * Allocate sym adaptor driver resources
 *
 * \param None
 *
 * \return 0 for success, otherwise failure
 */
int SymDriverAdaptorModuleInit()
{
    int symRc = CC_RET_OK;

    symRc = allocDmaBuildBuffers(&g_dmaInBuildBuffH);
    if (symRc != CC_RET_OK) {
        return CC_RET_NOMEM;
    }

    symRc = allocDmaBuildBuffers(&g_dmaOutBuildBuffH);
    if (symRc != CC_RET_OK) {
        freeDmaBuildBuffers(&g_dmaInBuildBuffH);
        return CC_RET_NOMEM;
    }

    symRc = AllocCompletionPlatBuffer();
    if (symRc != CC_RET_OK) {
        freeDmaBuildBuffers(&g_dmaInBuildBuffH);
        freeDmaBuildBuffers(&g_dmaOutBuildBuffH);
        return CC_RET_NOMEM;
    }

    return CC_RET_OK;
}

/*!
 * Release sym adaptor driver resources
 *
 * \param None
 *
 * \return always success
 */
int SymDriverAdaptorModuleTerminate()
{
    freeDmaBuildBuffers(&g_dmaInBuildBuffH);
    freeDmaBuildBuffers(&g_dmaOutBuildBuffH);
    FreeCompletionPlatBuffer();

    return CC_RET_OK;
}

static uint32_t SymDriverAdaptorCopySramBuff(driverAdaptorDir_t dir,
                                             CCSramAddr_t sram_addr,
                                             uint32_t *buff,
                                             uint32_t size)
{
    DmaBuffer_s dmaBuffIn, dmaBuffOut;
    CC_PalDmaBufferHandle dmaHandle;
    CCPalDmaBlockInfo_t dmaBlockEntry;
    uint32_t numOfBlocks;
    uint32_t rc, symRc;
    numOfBlocks = SINGLE_BLOCK_ENTRY;

    rc = CC_PalDmaBufferMap((uint8_t *) buff,
                            size,
                            CC_PAL_DMA_DIR_BI_DIRECTION,
                            &numOfBlocks,
                            &dmaBlockEntry,
                            &dmaHandle);
    if (rc != 0) {
        CC_PAL_LOG_ERR("failed to CC_PalDmaBufferMap for contig user context  0x%x\n", rc);
        return CC_RET_NOMEM;
    }

    if (dir == DRIVER_ADAPTOR_DIR_IN) {
        SET_DMA_BUFF_WITH_DLLI(((DmaBuffer_s* )&dmaBuffIn), dmaBlockEntry.blockPhysAddr, size);
        SET_DMA_BUFF(((DmaBuffer_s* )&dmaBuffOut), sram_addr, size, DMA_BUF_SEP, 0);
    } else {
        SET_DMA_BUFF_WITH_DLLI(((DmaBuffer_s* )&dmaBuffOut), dmaBlockEntry.blockPhysAddr, size);
        SET_DMA_BUFF(((DmaBuffer_s* )&dmaBuffIn), sram_addr, size, DMA_BUF_SEP, 0);
    }

    /* Write BYPASS without use of context. the ALG of bypass is now passed as a parameter to the
     dispatch process. the context address is not used by processBypass  */
    symRc = SymDriverDispatchProcess(0, buff, &dmaBuffIn, &dmaBuffOut, DRV_CRYPTO_ALG_BYPASS);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    WaitForSequenceCompletion();

EndWithErr:

    rc = CC_PalDmaBufferUnmap((uint8_t *) buff,
                              size,
                              CC_PAL_DMA_DIR_BI_DIRECTION,
                              numOfBlocks,
                              &dmaBlockEntry,
                              dmaHandle);
    if (symRc) {
        return symRc;
    }

    return rc;
}

static uint32_t SymDriverAdaptorBuildDataPtrFromDma(void* pDataIn,
                                                    void* pDataOut,
                                                    size_t dataSize,
                                                    DmaBuffer_s *pDmaBuffIn,
                                                    DmaBuffer_s *pDmaBuffOut,
                                                    uint32_t dmaBuiltFlag,
                                                    bool isSm4Ofb)
{
    uint32_t retCode = 0;
    uint32_t finRetCode = 0;

    // in case of inplace - unmap only one buffer bi directional
    if (dmaBuiltFlag & DMA_BUILT_FLAG_BI_DIR) {
        retCode = buildDataPtrFromDma((uint8_t *) pDataIn,
                                      dataSize,
                                      CC_PAL_DMA_DIR_BI_DIRECTION,
                                      pDmaBuffIn,
                                      &g_dmaInBuildBuffH);
        if (retCode != 0) {
            CC_PAL_LOG_ERR("failed to buildDataPtrFromDma for pDataIn inplace 0x%x\n", retCode);
            finRetCode = retCode;
        }
    }

    /* for SM4 OFB use const DIN - no dma buffer exists */
    if ((dmaBuiltFlag & DMA_BUILT_FLAG_INPUT_BUFF) && (isSm4Ofb == false)) {
        retCode = buildDataPtrFromDma((uint8_t *) pDataIn,
                                      dataSize,
                                      CC_PAL_DMA_DIR_TO_DEVICE,
                                      pDmaBuffIn,
                                      &g_dmaInBuildBuffH);
        if (retCode != 0) {
            CC_PAL_LOG_ERR("failed to buildDataPtrFromDma for pDataIn 0x%x\n", retCode);
            if (finRetCode == 0) {
                finRetCode = retCode;
            }
        }
    }

    if (dmaBuiltFlag & DMA_BUILT_FLAG_OUTPUT_BUFF) {
        retCode = buildDataPtrFromDma((uint8_t *) pDataOut,
                                      dataSize,
                                      CC_PAL_DMA_DIR_FROM_DEVICE,
                                      pDmaBuffOut,
                                      &g_dmaOutBuildBuffH);
        if (retCode != 0) {
            CC_PAL_LOG_ERR("failed to buildDataPtrFromDma for pDataOut 0x%x\n", retCode);
            if (finRetCode == 0) {
                finRetCode = retCode;
            }
        }
    }

    return finRetCode;
}

static uint32_t SymDriverAdaptorBuildDmaFromDataPtr(void* pDataIn,
                                                    void* pDataOut,
                                                    size_t dataSize,
                                                    DmaBuffer_s *pDmaBuffIn,
                                                    DmaBuffer_s *pDmaBuffOut,
                                                    uint32_t *pDmaBuiltFlag,
                                                    uint8_t isInPlace,
                                                    bool isSm4Ofb)
{
    uint32_t retCode = 0;

    /* in case of inplace - map only one buffer bi directional */
    if (isInPlace == INPLACE) {
        retCode = buildDmaFromDataPtr((uint8_t *) pDataIn,
                                      dataSize,
                                      CC_PAL_DMA_DIR_BI_DIRECTION,
                                      pDmaBuffIn,
                                      &g_dmaInBuildBuffH);
        if (retCode != 0) {
            CC_PAL_LOG_ERR("failed to buildDmaFromDataPtr for pDataIn inplace 0x%x\n", retCode);
            goto EndWithErr;
        }

        *pDmaBuiltFlag = DMA_BUILT_FLAG_BI_DIR;
        COPY_DMA_BUFF((*pDmaBuffOut), (*pDmaBuffIn));
    } else {

        if (isSm4Ofb == true) {
             /* for SM4 OFB use const DIN - no need to build dma buffer */
            pDmaBuffIn->size = dataSize;
            pDmaBuffIn->dmaBufType = DMA_SRAM;
        } else {

            retCode = buildDmaFromDataPtr((uint8_t *) pDataIn,
                                        dataSize,
                                        CC_PAL_DMA_DIR_TO_DEVICE,
                                        pDmaBuffIn,
                                        &g_dmaInBuildBuffH);

            if (retCode != 0) {
                CC_PAL_LOG_ERR("failed to buildDmaFromDataPtr for pDataIn 0x%x\n", retCode);
                goto EndWithErr;
            }
        }

        *pDmaBuiltFlag = DMA_BUILT_FLAG_INPUT_BUFF;
        retCode = buildDmaFromDataPtr((uint8_t *) pDataOut,
                                      dataSize,
                                      CC_PAL_DMA_DIR_FROM_DEVICE,
                                      pDmaBuffOut,
                                      &g_dmaOutBuildBuffH);

        if (retCode != 0) {
            CC_PAL_LOG_ERR("failed to buildDmaFromDataPtr for pDataOut 0x%x\n", retCode);
            uint32_t localRetCode = 0;

            localRetCode = buildDataPtrFromDma((uint8_t *) pDataIn,
                                               dataSize,
                                               CC_PAL_DMA_DIR_TO_DEVICE,
                                               pDmaBuffIn,
                                               &g_dmaInBuildBuffH);
            if (localRetCode != 0) {
                CC_PAL_LOG_ERR("failed to buildDataPtrFromDma for pDataIn 0x%x\n", localRetCode);
                goto EndWithErr;
            }

        }

        *pDmaBuiltFlag |= DMA_BUILT_FLAG_OUTPUT_BUFF;
    }

    return 0;

EndWithErr:
    *pDmaBuiltFlag = 0;
    return retCode;
}

uint32_t SymDriverAdaptorCopyCtx(driverAdaptorDir_t dir,
                                 CCSramAddr_t sram_address,
                                 uint32_t *pCtx,
                                 enum drv_crypto_alg alg)
{
    uint32_t rc = 0;
    switch (dir) {
        case DRIVER_ADAPTOR_DIR_IN:
        case DRIVER_ADAPTOR_DIR_OUT:
            rc = SymDriverAdaptorCopySramBuff(dir,
                                              sram_address,
                                              (uint32_t*) pCtx,
                                              getCtxSize(alg));
            break;
        default:
            rc = CC_RET_INVARG;
            break;
    }

    return rc;
}

/*!
 * Initializes the caller context by invoking the symmetric dispatcher driver.
 * The caller context may resides in SRAM or DCACHE SEP areas.
 * This function flow is synchronous.
 *
 * \param pCtx
 *
 * \return int One of CC_RET_* error codes defined in cc_sym_error.h.
 */
int SymDriverAdaptorInit(uint32_t *pCtx, enum drv_crypto_alg alg, int mode)
{
    int symRc = CC_RET_OK;
    CCPalPerfData_t perfIdx = 0;
    uint8_t isCpyCtxFlag = 0;

    CC_PAL_PERF_OPEN_NEW_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_INIT);
    CC_PAL_LOG_INFO("pCtx=%p\n", pCtx);
    if (pCtx == NULL) {
        CC_PAL_LOG_ERR("NULL pointer was given for ctx\n");
        CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_INIT);
        return CC_RET_INVARG_CTX;
    }

    symRc = CC_PalMutexLock(&CCSymCryptoMutex, CC_INFINITE);
    if (symRc != CC_SUCCESS) {
        CC_PalAbort("Fail to acquire mutex\n");
    }

    isCopyCtxRequired(alg, mode, &isCpyCtxFlag);

    if (isCpyCtxFlag) {
        symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_IN,
                                        CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                        pCtx,
                                        alg);
        if (symRc != CC_RET_OK) {
            goto EndWithErr;
        }
    }

    /* call the dispatcher with the new context pointer in SRAM */
    symRc = SymDriverDispatchInit(CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR, pCtx, alg);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    WaitForSequenceCompletion();

    if (isCpyCtxFlag) {
        symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_OUT,
                                        CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                        pCtx,
                                        alg);
    }

EndWithErr:

    CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_INIT);

    if (CC_PalMutexUnlock(&CCSymCryptoMutex) != CC_SUCCESS) {
        CC_PalAbort("Fail to release mutex\n");
    }
    return symRc;
}

/*!
 * Process a cryptographic data by invoking the symmetric dispatcher driver.
 * The invoker may request any amount of data aligned to the given algorithm
 * block size. It uses a scratch pad to copy (in cpu mode) the user
 * data from DCACHE/ICACHE to SRAM for processing. This function flow is
 * synchronous.
 *
 * \param pCtx may resides in SRAM or DCACHE SeP areas
 * \param pDataIn The input data buffer. It may reside in SRAM, DCACHE or ICACHE SeP address range
 * \param pDataOut The output data buffer. It may reside in SRAM or DCACHE SeP address range
 * \param DataSize The data input size in octets
 * \param alg The algorithm of the operation.
 *
 * \return int One of CC_RET_* error codes defined in cc_sym_error.h.
 */
int SymDriverAdaptorProcess(uint32_t* pCtx,
                            void* pDataIn,
                            void* pDataOut,
                            size_t DataSize,
                            enum drv_crypto_alg alg)
{
    int symRc = CC_RET_OK;
    int localSymRc = CC_RET_OK;

    DmaBuffer_s dmaBuffIn;
    DmaBuffer_s dmaBuffOut;
    CCError_t retCode;
    struct drv_ctx_cipher *pContext = (struct drv_ctx_cipher *) pCtx;
    uint32_t dmaBuiltFlag = DMA_BUILT_FLAG_NONE;
    CCPalPerfData_t perfIdx = 0;
    bool isSm4Ofb;

    CC_PAL_PERF_OPEN_NEW_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_PROC);
    CC_PAL_LOG_INFO("pCtx=%p\n", pCtx);
    CC_PAL_LOG_INFO("IN addr=%p OUT addr=%p DataSize=%u\n", pDataIn, pDataOut, DataSize);

    if (pCtx == NULL) {
        CC_PAL_LOG_ERR("NULL pointer was given for ctx\n");
        CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_PROC);
        return CC_RET_INVARG_CTX;
    }

    isSm4Ofb = (alg == DRV_CRYPTO_ALG_SM4) && (pContext->isSm4Ofb == true);

    if ((isSm4Ofb == false) && (pDataIn == NULL)) {
        CC_PAL_LOG_ERR("NULL pointer was given for din\n");
        CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_PROC);
        return CC_RET_INVARG_CTX;
    }

    /* In AES mac modes there is no output so it needs special treatment */
    if ((alg == DRV_CRYPTO_ALG_AES)
                    && ((pContext->mode == DRV_CIPHER_CBC_MAC)
                                    || (pContext->mode == DRV_CIPHER_XCBC_MAC)
                                    || (pContext->mode == DRV_CIPHER_CMAC))) {
        /* clear the output to mark that it is not used */
        pDataOut = NULL;
    }

    retCode = CC_PalMutexLock(&CCSymCryptoMutex, CC_INFINITE);
    if (retCode != CC_SUCCESS) {
        CC_PalAbort("Fail to acquire mutex\n");
    }

    symRc = SymDriverAdaptorBuildDmaFromDataPtr(pDataIn,
                                                pDataOut,
                                                DataSize,
                                                &dmaBuffIn,
                                                &dmaBuffOut,
                                                &dmaBuiltFlag,
                                                (pDataIn == pDataOut) ? INPLACE : NOT_INPLACE,
                                                isSm4Ofb);
    if (symRc != CC_RET_OK) {
        /* skip unmapping of the buffers */
        goto ProcessUnlockMutex;
    }

    symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_IN,
                                    CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                    pCtx,
                                    alg);

    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    symRc = SymDriverDispatchProcess(CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                     pCtx,
                                     &dmaBuffIn,
                                     &dmaBuffOut,
                                     alg);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    WaitForSequenceCompletion();
    symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_OUT,
                                    CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                    pCtx,
                                    alg);

EndWithErr:
    localSymRc = SymDriverAdaptorBuildDataPtrFromDma(pDataIn,
                                                     pDataOut,
                                                     DataSize,
                                                     &dmaBuffIn,
                                                     &dmaBuffOut,
                                                     dmaBuiltFlag,
                                                     isSm4Ofb);

    if (symRc == CC_RET_OK) {
        symRc = localSymRc;
    }

ProcessUnlockMutex:
    CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_PROC);

    if (CC_PalMutexUnlock(&CCSymCryptoMutex) != CC_SUCCESS) {
        CC_PalAbort("Fail to release mutex\n");
    }

    return symRc;
}

/*!
 * Finalizing the cryptographic data by invoking the symmetric dispatcher driver.
 * It calls the `SymDriverDcacheAdaptorFinalize` function for processing by leaving
 * any reminder for the finalize operation.
 *
 * \param pCtx may resides in SRAM or DCACHE SeP areas
 * \param pDataIn The input data buffer. It may reside in SRAM, DCACHE or ICACHE SeP address range
 * \param pDataOut The output data buffer. It may reside in SRAM or DCACHE SeP address range
 * \param DataSize The data input size in octets
 * \param alg The algorithm of the operation.
 *
 * \return int One of CC_RET_* error codes defined in cc_sym_error.h.
 */
int SymDriverAdaptorFinalize(uint32_t* pCtx,
                             void* pDataIn,
                             void* pDataOut,
                             size_t DataSize,
                             enum drv_crypto_alg alg)
{
    DmaBuffer_s dmaBuffIn;
    DmaBuffer_s dmaBuffOut;
    int symRc = CC_RET_OK;
    int localSymRc = CC_RET_OK;
    struct drv_ctx_cipher *pAesContext = (struct drv_ctx_cipher *) pCtx;
    uint32_t retCode;
    /* used to differ AES MAC modes (where the dout is not NULL, but is not access via DMA */
    uint32_t isMac = CC_FALSE;
    void *pTmpDataOut = pDataOut;
    uint32_t dmaBuiltFlag = DMA_BUILT_FLAG_NONE;
    CCPalPerfData_t perfIdx = 0;
    bool isSm4Ofb;

    CC_PAL_PERF_OPEN_NEW_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_FIN);

    CC_PAL_LOG_INFO("pCtx=%p\n", pCtx);
    CC_PAL_LOG_INFO("IN addr=%p OUT addr=%p DataSize=%u\n", pDataIn, pDataOut, DataSize);

    /* do not check din pointer since hash/hmac algs has no data input */
    if (pCtx == NULL) {
        CC_PAL_LOG_ERR("NULL pointer was given for ctx\n");
        CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_FIN);
        return CC_RET_INVARG_CTX;
    }

    isSm4Ofb = (alg == DRV_CRYPTO_ALG_SM4) && (pAesContext->isSm4Ofb == true) &&
            (DataSize != 0);

    if ((alg == DRV_CRYPTO_ALG_AES)
                    && ((pAesContext->mode == DRV_CIPHER_CBC_MAC)
                                    || (pAesContext->mode == DRV_CIPHER_XCBC_MAC)
                                    || (pAesContext->mode == DRV_CIPHER_CMAC))) {
        isMac = CC_TRUE;
        pTmpDataOut = NULL;
    }

    retCode = CC_PalMutexLock(&CCSymCryptoMutex, CC_INFINITE);
    if (retCode != CC_SUCCESS) {
        CC_PalAbort("Fail to acquire mutex\n");
    }

    // in case of inplace - map only one buffer bi directional
    symRc = SymDriverAdaptorBuildDmaFromDataPtr(pDataIn,
                                                pTmpDataOut,
                                                DataSize,
                                                &dmaBuffIn,
                                                &dmaBuffOut,
                                                &dmaBuiltFlag,
                                                (pDataIn == pDataOut && !isMac) ?
                                                                INPLACE : NOT_INPLACE,
                                                isSm4Ofb);
    if (symRc != CC_RET_OK) {
        /* skip unmapping of the buffers */
        goto finalizeUnlockMutex;
    }

    symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_IN,
                                    CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                    pCtx,
                                    alg);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    symRc = SymDriverDispatchFinalize(CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                      pCtx,
                                      &dmaBuffIn,
                                      &dmaBuffOut,
                                      alg);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    WaitForSequenceCompletion();

    symRc = SymDriverAdaptorCopyCtx(DRIVER_ADAPTOR_DIR_OUT,
                                    CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR,
                                    pCtx,
                                    alg);
    if (symRc != CC_RET_OK) {
        goto EndWithErr;
    }

    if (isMac == CC_TRUE) {
        switch (pAesContext->mode) {
            case DRV_CIPHER_CBC_MAC:
            case DRV_CIPHER_XCBC_MAC:
            case DRV_CIPHER_CMAC:
                if (pDataOut == NULL) { /* in case of MAC the data out must not be NULL (MAC is copied to it) */
                    symRc = CC_RET_INVARG;
                    goto EndWithErr;
                }
                CC_PalMemCopy(pDataOut, pAesContext->block_state, CC_AES_BLOCK_SIZE);
                break;
            default:
                break;
        }
    }

EndWithErr:
    localSymRc = SymDriverAdaptorBuildDataPtrFromDma(pDataIn,
                                                     pTmpDataOut,
                                                     DataSize,
                                                     &dmaBuffIn,
                                                     &dmaBuffOut,
                                                     dmaBuiltFlag,
                                                     isSm4Ofb);

    if (symRc == CC_RET_OK) {
        symRc = localSymRc;
    }

finalizeUnlockMutex:

    CC_PAL_PERF_CLOSE_ENTRY(perfIdx, PERF_TEST_TYPE_SYM_DRV_FIN);

    if (CC_PalMutexUnlock(&CCSymCryptoMutex) != CC_SUCCESS) {
        CC_PalAbort("Fail to release mutex\n");
    }

    return symRc;
}

