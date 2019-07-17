/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

#ifndef _CC_PAL_INTERRUPTCTRL_PLAT_H
#define _CC_PAL_INTERRUPTCTRL_PLAT_H

#include "cc_pal_interrupt_ctrl.h"

typedef void (*CCPalISR)(void);

CCError_t CC_PalInitIrq(CCPalISR pIsrFunction);

#endif /* _CC_PAL_INTERRUPTCTRL_PLAT_H */

