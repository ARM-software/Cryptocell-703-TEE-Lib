/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <unistd.h>
#include <pthread.h>

#include "cc_pal_cert.h"
#include "cc_pal_types.h"
#include "cc_pal_mem.h"
#include "cc_hal.h"
#include "cc_registers.h"
#include "cc_regs.h"


#ifdef CC_SUPPORT_CH_CERT
#include "cc_chinese_cert_defs.h"
#endif

#ifdef CC_SUPPORT_FIPS
#include "cc_fips_defs.h"
#endif

#ifdef CC_SUPPORT_CH_CERT
CCChCertStateData_t   gStateData = { CC_CH_CERT_STATE_CRYPTO_APPROVED, CC_TEE_CH_CERT_ERROR_OK, CC_CH_CERT_TRACE_NONE };
#endif

#ifdef CC_SUPPORT_FIPS
CCFipsStateData_t 	gStateData = { CC_FIPS_STATE_CRYPTO_APPROVED, CC_TEE_FIPS_ERROR_OK, CC_FIPS_TRACE_NONE };
pthread_t threadId;
bool thread_exit = false;
uint32_t threadRc;

#define GPR0_IRR_MASK (1<<CC_HOST_RGF_IRR_GPR0_INT_BIT_SHIFT)

void *certIrqThread(void *params)
{
	uint32_t regVal = 0;
	uint32_t certMask = GPR0_IRR_MASK;

	CC_UNUSED_PARAM(params);

	while(!thread_exit) {
		regVal = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_IRR));
		if (regVal & certMask) {
			CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_RGF_ICR), certMask);
			CC_FipsIrqHandle();
		}
		usleep(100); // wait 100 milisecond
	}
	threadRc = 0;
        pthread_exit(&threadRc);
}


CCError_t CC_PalCertWaitForReeStatus(void)
{
	uint32_t rc;

	thread_exit = false;
	rc = pthread_create(&threadId, NULL, certIrqThread, NULL);
	if (rc != 0) {
		return rc;
	}
	// join will be in the termination function
	return CC_OK;
}

CCError_t CC_PalCertStopWaitingRee(void)
{
	void *threadRet;

	thread_exit = true; // The fips thread checks this flag and act accordingly
	pthread_join(threadId, &threadRet);
	return CC_OK;
}
#endif


CCError_t CC_PalCertGetState(uint32_t *pCertState)
{
	*pCertState = gStateData.state;

	return CC_OK;
}


CCError_t CC_PalCertGetError(uint32_t *pCertError)
{
	*pCertError = gStateData.error;

	return CC_OK;
}


CCError_t CC_PalCertGetTrace(uint32_t *pCertTrace)
{
	*pCertTrace = gStateData.trace;

	return CC_OK;
}

CCError_t CC_PalCertSetState(uint32_t certState)
{
	gStateData.state = certState;

	return CC_OK;
}

CCError_t CC_PalCertSetError(uint32_t certError)
{
	gStateData.error = certError;

	return CC_OK;
}

CCError_t CC_PalCertSetTrace(uint32_t certTrace)
{
	gStateData.trace = (gStateData.trace | certTrace);

	return CC_OK;
}

