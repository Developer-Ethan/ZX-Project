/**************************************************************************************************/
/**
 * @file      : iir_fo.h
 * @brief     : IIR filter header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef IIR_FO_H
#define IIR_FO_H

#include <stdint.h>
#include <stdbool.h>
#include "zxmathfp.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  IIR
 *  @{
 */

/** @defgroup IIR_Public_Types 
 *  @{
 */

/**
 *  @brief IIR paramter type definition
 */
typedef struct _IIR_FO_Obj_
{
  q15   output;
  bool  isInitial;
  q15   filterCoeff;

} IIR_FO_Obj;

/** @} end of group IIR_Public_Types*/

/** @defgroup IIR_Public_FunctionDeclaration
 *  @brief IIR functions statement
 *  @{
 */
extern void IIR_FO_init(IIR_FO_Obj* ObjPtr, q15 filterCoeff);


extern void IIR_FO_pushInput(IIR_FO_Obj* ObjPtr, q15 input);


static inline q15 IIR_FO_getOuput(IIR_FO_Obj* ObjPtr)
{
  return ObjPtr->output;
}


/** @} end of group IIF_Public_FunctionDeclaration */

/** @} end of group IIR_H */

/** @} end of group Z20K11XM_Foc */

#endif /* IIR_FO_H */
