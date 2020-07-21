/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdio.h>
#include "test_pal_semphr.h"

/******************************************************************************/
uint8_t Test_PalMutexCreate(Test_PalMutex *ppMutexId)
{
	*ppMutexId =  NULL;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalMutexDestroy(Test_PalMutex *ppMutexId)
{
	(void)ppMutexId;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalMutexLock(Test_PalMutex *ppMutexId, uint32_t timeout)
{
	(void)ppMutexId;
	(void)timeout;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalMutexUnlock(Test_PalMutex *ppMutexId)
{
	(void)ppMutexId;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalBinarySemaphoreCreate(Test_PalBinarySemaphore *ppBinSemphrId)
{
	(void)ppBinSemphrId;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalBinarySemaphoreDestroy(Test_PalBinarySemaphore *ppBinSemphrId)
{
	(void)ppBinSemphrId;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalBinarySemaphoreTake(Test_PalBinarySemaphore *ppBinSemphrId,
							uint32_t timeout)
{
	(void)ppBinSemphrId;
	(void)timeout;
	return 0;
}

/******************************************************************************/
uint8_t Test_PalBinarySemaphoreGive(Test_PalBinarySemaphore *ppBinSemphrId)
{
	(void)ppBinSemphrId;
	return 0;
}
