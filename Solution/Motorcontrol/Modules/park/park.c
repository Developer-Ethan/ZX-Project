/*
 * @file     park.c
 * @brief    Park transform file.
 * @version  V1.0.0
 * @date     18-Nov-2021
 * @author   Zhixin Semiconductor
 *
 * @note
 * Copyright (C) 2022 Zhixin Semiconductor Ltd. All rights reserved.
 */


// **************************************************************************
// the includes

#include "park.h"

// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

void PARK_run(PARK_Obj* obj)
{
	q15 Sine = zx_sinQ15(obj->angle);
	q15 Cosine = zx_cosQ15(obj->angle);
	obj->Ds = zx_mpyQ15(obj->alpha, Cosine) + zx_mpyQ15(obj->beta, Sine);
	obj->Qs = zx_mpyQ15(obj->beta, Cosine) - zx_mpyQ15(obj->alpha, Sine);
}
