/**************************************************************************************************/
/**
 * @file      : pid.h
 * @brief     : Pid header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef PID_H
#define PID_H


#include <stdint.h>
#include "zxmathfp.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  PID
 *  @{
 */

/** @defgroup PID_Public_Types 
 *  @{
 */
/**
 *  @brief PID paramter of calculation type definition
 */

typedef struct _PID_Obj_
{
  q15   ref;           //!< Input: Reference input
  q15   fdb;           //!< Input: Feedback input

  q15   output;        //!< Output: PID output

  q15   Kp;            //!< Parameter: Proportional gain
  q15   Ki;            //!< Parameter: Integral gain
  q15   Kd;            //!< Parameter: Derivative gain
  q15   outMax;        //!< Parameter: Maximum output
  q15   outMin;        //!< Parameter: Minimum output

  q15   Ui;            //!< Internal Variable: Integral output
  q15   lastError;     //!< Internal Variable: last error

} PID_Obj;

/** @} end of group PID_Public_Types */
/** @defgroup PID_Public_FunctionDeclaration
 *  @brief PID functions statement
 *  @{
 */
void PID_init(PID_Obj* ObjPtr, q15 Kp, q15 Ki, q15 Kd, q15 min, q15 max);


void PID_run(PID_Obj* ObjPtr);

/** @} end of group PID_Public_FunctionDeclaration */

/** @} end of group PID */

/** @} end of group Z20K11XM_Foc */


#endif /* PID_H */
