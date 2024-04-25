/*
 * @file     clarke.c
 * @brief    Clarke transform file.
 * @version  V1.0.0
 * @date     20-Nov-2021
 * @author   Zhixin Semiconductor
 *
 * @note
 * Copyright (C) 2022 Zhixin Semiconductor Ltd. All rights reserved.
 */

// **************************************************************************
// the includes

#include "clarke.h"

// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

void CLARKE_run(CLARKE_Obj* obj)
{
	int32_t aa = obj->As << 1;
	aa = aa - obj->Bs - obj->Cs;
	obj->alpha = zx_mpyQ15(aa, Q15MATH_ONE_OVER_THREE);

	int32_t bb = obj->Bs - obj->Cs;
	obj->beta = zx_mpyQ15(bb, Q15MATH_ONE_OVER_SQRT_THREE);
}
