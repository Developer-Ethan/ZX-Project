/*
 * @file     park.h
 * @brief    Park transform head file.
 * @version  V1.0.0
 * @date     18-Nov-2021
 * @author   Zhixin Semiconductor
 *
 * @note
 * Copyright (C) 2022 Zhixin Semiconductor Ltd. All rights reserved.
 */

#ifndef _PARK_H_
#define _PARK_H_

//! \brief  Contains the public interface to the 
//!         Park transform module routines 
//!


// **************************************************************************
// the includes

#include "zxmathfp.h"

#ifdef __cplusplus
extern "C" {
#endif


// **************************************************************************
// the defines



// **************************************************************************
// the typedefs

//! \brief Defines the PARK object
//!
typedef struct _PARK_Obj_
{
  q15  alpha;    // Input: stationary d-axis stator variable
  q15  beta;     // Input: stationary q-axis stator variable
  q15  angle;    // Input: rotating angle (pu)
  q15  Ds;       // Output: rotating d-axis stator variable
  q15  Qs;       // Output: rotating q-axis stator variable

} PARK_Obj;


// **************************************************************************
// the function prototypes

extern void PARK_run(PARK_Obj* obj);


#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of _PARK_H_ definition

