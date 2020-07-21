/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdint.h>
#include <stdlib.h>

#include "test_pal_map_addrs.h"

/******************************************************************************/
void *Test_PalIOMap(void *physAddr, size_t size)
{
	(void)size;
	return physAddr;
}

/******************************************************************************/
void *Test_PalMapAddr(void *physAddr, void *startingAddr, const char *filename,
				size_t size, uint8_t protAndFlagsBitMask)
{
	(void)startingAddr;
	(void)filename;
	(void)size;
	(void)protAndFlagsBitMask;
	return physAddr;
}

/******************************************************************************/
void Test_PalUnmapAddr(void *virtAddr, size_t size)
{
	(void)virtAddr;
	(void)size;
}

