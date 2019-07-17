/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#define CC_PAL_LOG_CUR_COMPONENT CC_LOG_MASK_CCLIB

#include "cc_pal_types.h"
#include "cc_pal_log.h"
#include "cc_pal_mem.h"
#include "cc_lib.h"
#include "cc_hal.h"
#include "cc_hal_defs.h"
#include "cc_pal_init.h"
#include "cc_pal_mutex.h"
#include "cc_pal_interrupt_ctrl_plat.h"
#include "hw_queue.h"
#include "completion.h"
#include "sym_adaptor_driver.h"
#include "cc_pal_dma.h"
#include "cc_pal_perf.h"
#include "cc_general_defs.h"
#include "cc_plat.h"
#include "cc_sram_map.h"
#include "cc_lib_common.h"
#include "cc_hal_axi_ctrl.h"

#ifdef CC_SUPPORT_CH_CERT
#include "cc_chinese_cert.h"
#include "cc_chinese_cert_defs.h"
#include "cc_pal_cert.h"
#endif

/************************ Defines ********************************************/
#define CC_CPP_INTERRUPT_ENABLE_MASK                    \
        (1 << CC_HOST_RGF_IRR_REE_KS_OPERATION_INDICATION_BIT_SHIFT)
//     (1 << CC_HOST_RGF_IRR_GPR0_INT_BIT_SHIFT)
//     (1<<CC_HOST_RGF_IRR_DSCRPTR_COMPLETION_INT_BIT_SHIFT))

/************************ Extern *********************************************/
/* interrupthandler function */
extern void CC_InterruptHandler(void);

/************************ Global Data ****************************************/
CC_PalMutex CCSymCryptoMutex;  /* for use by SM3/4  */
CC_PalMutex CCAsymCryptoMutex;  /* for use by SM2 */
CC_PalMutex CCRndCryptoMutex;
CC_PalMutex CCChCertMutex;
CC_PalMutex *pCCRndCryptoMutex; /* for use by TRNG FE */

/************************ Private Functions **********************************/
static void ClearSram(void)
{
    /* clear symmetric context from SRAM */
    _ClearSram(CC_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR, CC_SRAM_DRIVER_ADAPTOR_CONTEXT_MAX_SIZE);

    return;
}

/*!
 * Common TEE initialisations for both cold and warm boot.
 * This function is the place for CC initialisations that are required both
 * during CC_LibInit() and CC_PmResume()
 */
int CC_CommonInit(void)
{
    int rc = 0;

#ifdef BIG__ENDIAN
    /* Set DMA endianness to big */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ENDIAN), 0xCCUL);
#else /* LITTLE__ENDIAN */
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ENDIAN), 0x00UL);
#endif

    InitCompletion();

    return rc;
}


/*!
 * TEE (Trusted Execution Environment) entry point.
 * Init CryptoCell for TEE.
 *
 * @param[in/out]
 *
 * \return CClibRetCode_t one of the error codes defined in cc_lib.h
 */
CClibRetCode_t CC_LibInit(bool isChCertSupport, CCCertKatContext_t  *pCertCtx, CCAxiFields_t  *pAxiFields)
{
    int rc = CC_LIB_RET_OK;
    uint32_t pidReg[CC_PID_SIZE_WORDS] = { 0 };
    uint32_t cidReg[CC_CID_SIZE_WORDS] = { 0 };
    uint32_t imrValue = 0;
    uint32_t imrMask = 0;
    uint32_t removedEngines = 0;
    const uint32_t pidVal[CC_PID_SIZE_WORDS] = { CC_PID_0_VAL,
            CC_PID_1_VAL,
            CC_PID_2_VAL,
            CC_PID_3_VAL & ~(CC_PID_3_IGNORE_MASK),
            CC_PID_4_VAL };

    const uint32_t cidVal[CC_CID_SIZE_WORDS] = { CC_CID_0_VAL,
            CC_CID_1_VAL,
            CC_CID_2_VAL,
            CC_CID_3_VAL };

    if (pAxiFields == NULL) {
        return SA_SILIB_RET_EINVAL;
    }

    rc = CC_PalInit();
    if (rc != CC_LIB_RET_OK) {
        rc = CC_LIB_RET_PAL;
        goto InitErr;
    }

    rc = CC_HalInit();
    if (rc != CC_LIB_RET_OK) {
        rc = CC_LIB_RET_HAL;
        goto InitErr1;
    }

    /* verify peripheral ID (PIDR) */
    pidReg[0] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, PERIPHERAL_ID_0));
    pidReg[1] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, PERIPHERAL_ID_1));
    pidReg[2] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, PERIPHERAL_ID_2));
    /* poll NVM register to assure that the NVM boot is finished (and LCS and the keys are valid) */
    pidReg[3] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, PERIPHERAL_ID_3)) & ~(CC_PID_3_IGNORE_MASK);
    pidReg[4] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, PERIPHERAL_ID_4));
    if (CC_PalMemCmp((uint8_t*)pidVal, (uint8_t*)pidReg, sizeof(pidVal)) != 0) {
        rc = CC_LIB_RET_EINVAL_PIDR;
        goto InitErr2;
    }

    /* verify component ID (CIDR) */
    cidReg[0] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, COMPONENT_ID_0));
    cidReg[1] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, COMPONENT_ID_1));
    cidReg[2] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, COMPONENT_ID_2));
    cidReg[3] = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, COMPONENT_ID_3));
    if (CC_PalMemCmp((uint8_t*)cidVal, (uint8_t*)cidReg, sizeof(cidVal)) != 0) {
        rc = CC_LIB_RET_EINVAL_CIDR;
        goto InitErr2;
    }

    /*wait for reset to be completed - by polling on the NVM idle register*/
    CC_LIB_WAIT_ON_NVM_IDLE_BIT();

    /* verify hw is configured to slim
     * verification is done by checking that only the relevant engines are enabled (SM3, SM4),
     * and all other HW engines are disabled. */
    removedEngines = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_REMOVE_INPUT_PINS));
    if (removedEngines != CC_HW_ENGINES_SLIM_CONFIG) {
        rc = CC_LIB_INCORRECT_HW_VERSION_SLIM_VS_FULL;
        goto InitErr2;
    }

    /* init interrupt and register interrupt handler */
    rc = CC_PalInitIrq(CC_InterruptHandler);
    if (rc != CC_LIB_RET_OK) {
        rc = CC_LIB_RET_PAL;
        goto InitErr2;
    }

    /* unmask appropriate interrupts */
    imrValue = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR));

    CC_REG_FLD_SET(HOST_RGF, HOST_RGF_IMR, REE_KS_OPERATION_INDICATION_MASK, imrMask, 1);
    CC_REG_FLD_SET(HOST_RGF, HOST_RGF_IMR, AXIM_COMP_INT_MASK, imrMask, 1);
    CC_REG_FLD_SET(HOST_RGF, HOST_RGF_IMR, AXI_ERR_MASK, imrMask, 1);
    CC_REG_FLD_SET(HOST_RGF, HOST_RGF_IMR, RNG_INT_MASK, imrMask, 1);

    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR), imrValue & (~imrMask));

    /* set axi parameters */
    rc = CC_HalSetCacheParams (pAxiFields);
    if (rc != CC_LIB_RET_OK) {
        rc = CC_LIB_RET_CACHE_PARAMS_ERROR;
        goto InitErr2;
    }

    /* common initialisations */
    rc = CC_CommonInit();
    if (rc != CC_LIB_RET_OK) {
        goto InitErr2;
    }

    rc = SymDriverAdaptorModuleInit();
    if (rc != CC_LIB_RET_OK) {
        rc = SA_SILIB_RET_COMPLETION;  // check
        goto InitErr2;
    }

    CC_PAL_PERF_INIT();

    /* clear SRAM sensitive data */
    ClearSram();

#ifdef CC_SUPPORT_CH_CERT
    rc = ChCertSetState(isChCertSupport ? CC_CH_CERT_STATE_SUPPORTED : CC_CH_CERT_STATE_NOT_SUPPORTED);
    if (rc != CC_OK) {
        rc = SA_SILIB_RET_ECHCERT;
        goto InitErr2;
    }
    rc = ChCertRunPowerUpTest(pCertCtx);
    if (rc != CC_OK) {
        rc = SA_SILIB_RET_ECHCERT;
        goto InitErr;   /* do not terminate hal and pal, since CC API should work and return error */
    }

    rc = CC_CH_CERT_CRYPTO_USAGE_SET_APPROVED();
    if (rc != CC_OK) {
        rc = SA_SILIB_RET_ECHCERT;
        goto InitErr2;
    }

#else
    CC_UNUSED_PARAM(isChCertSupport);
    CC_UNUSED_PARAM(pCertCtx);
#endif



    return CC_LIB_RET_OK;

InitErr2:
    CC_HalTerminate();

InitErr1:
    CC_PalTerminate();

InitErr:
    return (CClibRetCode_t)rc;
}

/*!
 * Common TEE finalisations for both cold and warm boot.
 * This function is the place for CC finalisations that are required both
 * during CC_LibFini() and CC_PmSuspend()
 */
void CC_CommonFini(void)
{
    return;
}

/*!
 * TEE (Trusted Execution Environment) exit point.
 * Finalize CryptoCell for TEE operation, release associated resources.
 *                                                                    .
 * @param[in/out] rndContext_ptr  - Pointer to the RND context buffer.
 */
void CC_LibFini(void)
{
    uint32_t imrValue;

    /* common finalisations */
    CC_CommonFini();

    SymDriverAdaptorModuleTerminate();

    /* mask appropriate interrupts and finish interrupt handling */
    imrValue = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR));
    CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IMR),
                          imrValue | CC_CPP_INTERRUPT_ENABLE_MASK);

    CC_PalFinishIrq();

    CC_HalTerminate();
    CC_PalTerminate();
    CC_PAL_PERF_FIN();
}

