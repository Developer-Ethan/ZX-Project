/*
 * @file     ipark.c
 * @brief    Inverse park tranform file.
 * @version  V1.0.0
 * @date     16-Nov-2021
 * @author   Zhixin Semiconductor
 *
 * @note
 * Copyright (C) 2022 Zhixin Semiconductor Ltd. All rights reserved.
 */

// **************************************************************************
// the includes

#include "ipark.h"

// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

void IPARK_run(IPARK_Obj* obj)
{
  q15 Sine = zx_sinQ15(obj->angle);
  q15 Cosine = zx_cosQ15(obj->angle);

  obj->alpha = zx_mpyQ15(obj->Ds, Cosine) - zx_mpyQ15(obj->Qs, Sine);
  obj->beta = zx_mpyQ15(obj->Qs, Cosine) + zx_mpyQ15(obj->Ds, Sine);
}
