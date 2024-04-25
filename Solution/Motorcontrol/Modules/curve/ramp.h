/**************************************************************************************************/
/**
 * @file      : ramp.h
 * @brief     : Ramp module header file
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef RAMP_H
#define RAMP_H


#include <stdint.h>
#include "zxmathfp.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  RAMP
 *  @{
 */

/** @defgroup RAMP_Public_Types 
 *  @{
 */

/**
 *  @brief RAMP paramter type definition
 */
typedef struct _RAMP_Obj_
{
  q15   target;  //!< Input: target input
  q15   step;    //!< Parameter: step of ramp
  q15   output;  //!< Output: output value

} RAMP_Obj;

// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

extern void RAMP_init(RAMP_Obj* ObjPtr, q15 step);


extern void RAMP_calc(RAMP_Obj* ObjPtr);

/** @} end of group RAMP_Public_FunctionDeclaration */

/** @} end of group RAMP */

/** @} end of group Z20K11XM_Foc */

#endif /* _RAMP_H_ */
