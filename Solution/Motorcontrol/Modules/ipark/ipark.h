/*
 * @file     ipark.h
 * @brief    Inverse park transform head file.
 * @version  V1.0.0
 * @date     16-Nov-2021
 * @author   Zhixin Semiconductor
 *
 * @note
 * Copyright (C) 2022 Zhixin Semiconductor Ltd. All rights reserved.
 */

#ifndef _IPARK_H_
#define _IPARK_H_

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

//! \brief Defines the IPARK object
//!
typedef struct _IPARK_Obj_
{
  q15  alpha;    // Input: stationary d-axis stator variable
  q15  beta;     // Input: stationary q-axis stator variable
  q15  angle;    // Input: rotating angle (pu)
  q15  Ds;       // Output: rotating d-axis stator variable
  q15  Qs;       // Output: rotating q-axis stator variable

} IPARK_Obj;


// **************************************************************************
// the function prototypes

extern void IPARK_run(IPARK_Obj* obj);


#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of _IPARK_H_ definition

