/**************************************************************************************************/
/**
 * @file      : pid.c
 * @brief     : Location type PID file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#include "pid.h"

/** @addtogroup  Z20K14XM_Foc
 *  @{
 */

/** @defgroup PID 
 *  @brief PID driver modules
 *  @{
 */

/** @defgroup PID_Public_Functions
 *  @{
 */
/**
 * @brief      PID initial function.
 *
 * @param[inout]  ObjPtr: PID input data and kp ki parameters address.
 * @param[in]  Ki: coeff of Kp value.
 * @param[in]  Kp: coeff of Ki value.
 * @param[in]  Kd: coeff of Kd value.
 * @param[in]  Min: Lower limite value of saturation.
 * @param[in]  Max: Upper limite value of saturation.
 *
 * @return     None.
 *
 */
void PID_init(PID_Obj* ObjPtr, q15 Kp, q15 Ki, q15 Kd, q15 min, q15 max)
{
  ObjPtr->Kp = Kp;
  ObjPtr->Ki = Ki;
  ObjPtr->Kd = Kd;
  ObjPtr->outMin = min;
  ObjPtr->outMax = max;

  ObjPtr->ref = 0;
  ObjPtr->fdb = 0;
  ObjPtr->Ui = 0;
  ObjPtr->lastError = 0;
  ObjPtr->output = 0;
}

/**
 * @brief      Run PID function.
 *
 * @param[inout]  ObjPtr: PID input data and kp ki parameters address.
 *
 * @return     None.
 *
 */
void PID_run(PID_Obj* ObjPtr)
{
  // Compute the error
  q15 error = ObjPtr->ref - ObjPtr->fdb;

  // Compute the proportional output
  q15 Up = ZxMath_MpyQ15(ObjPtr->Kp, error);

  // Compute the integral output
  ObjPtr->Ui = ObjPtr->Ui + ZxMath_MpyQ15(ObjPtr->Ki, error);
  // Saturate Ui
  if(ObjPtr->Ui > ObjPtr->outMax) {
    ObjPtr->Ui = ObjPtr->outMax;
  }
  if(ObjPtr->Ui < ObjPtr->outMin) {
    ObjPtr->Ui = ObjPtr->outMin;
  }


  // Compute the pre-saturated output
  q15 tmpOutput = Up + ObjPtr->Ui;

  // Saturate the output
  if (tmpOutput > ObjPtr->outMax) {
    ObjPtr->output = ObjPtr->outMax;
  }
  else if (tmpOutput < ObjPtr->outMin) {
    ObjPtr->output = ObjPtr->outMin;
  }
  else {
    ObjPtr->output = tmpOutput;
  }
}
/** @} end of group PID_Public_Functions */

/** @} end of group PID driver modules */

/** @} end of group Z20K11XM_Foc */