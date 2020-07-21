/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "test_pal_log.h"
#include "test_pal_thread.h"

/******************************************************************************/
size_t Test_PalGetMinimalStackSize(void)
{
	return 0;
}

/******************************************************************************/
uint32_t Test_PalGetHighestPriority(void)
{
	return 0;
}

/******************************************************************************/
uint32_t Test_PalGetLowestPriority(void)
{
	return 0;
}

/******************************************************************************/
uint32_t Test_PalGetDefaultPriority(void)
{
	return 0;
}

/******************************************************************************/
ThreadHandle Test_PalThreadCreate(size_t stackSize,
				  void *(*threadFunc)(void *),
				  int priority, void *args,
				  const char *threadName,
				  uint8_t nameLen, uint8_t dmaAble)
{
	(void)stackSize;
	(void)threadFunc;
	(void)priority;
	(void)args;
	(void)threadName;
	(void)nameLen;
	(void)dmaAble;
	return NULL;
}

/******************************************************************************/
uint32_t Test_PalThreadJoin(ThreadHandle threadHandle, void **threadRet)
{
	(void)threadHandle;
	(void)threadRet;
	return 0;
}

/******************************************************************************/
uint32_t Test_PalThreadDestroy(ThreadHandle threadHandle)
{
	(void)threadHandle;
	return 0;
}
