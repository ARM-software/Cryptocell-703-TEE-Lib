/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#include <stdint.h>
#include <time.h>
#include <math.h>
#include "test_pal_time.h"

/******************************************************************************/
/* The smallest resolution in no_os is seconds */
void Test_PalDelay(const uint32_t usec)
{
	/* Rounds up */
	uint32_t secs = (usec + 999) / 1000;

	/* Gets the end time */
	uint32_t retTime = time(0) + secs;

	/* Loop until end time arrives */
	while (time(0) < retTime);
}
