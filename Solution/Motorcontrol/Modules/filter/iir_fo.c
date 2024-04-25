/**************************************************************************************************/
/**
 * @file      : iir_fo.c
 * @brief     : IIR filter file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#include "iir_fo.h"

/** @addtogroup  Z20K14XM_Foc
 *  @{
 */

/** @defgroup IIR_FO 
 *  @brief IIR_FO driver modules
 *  @{
 */
   
/** @defgroup CTRL_Global_Variables
 *  @{
 */

/** @} end of group CTRL_Global_Variables */

/** @defgroup Iir_Fo_Public_Functions
 *  @{
 */
/**
 * @brief      Iir filter initial  function.
 *
 * @param[inout]  ObjPtr: Filter input data and output data.
 * @param[in]  FilterCoeff: Filter coeff value.
 *
 * @return     None.
 *
 */
void IIR_FO_init(IIR_FO_Obj* ObjPtr, q15 filterCoeff)
{
  ObjPtr->isInitial = true;
  ObjPtr->output = 0;
  if(filterCoeff > 0 && filterCoeff < ZxMath_F32toQ15(1.0)) {
    ObjPtr->filterCoeff = filterCoeff;
  }
  else {
    ObjPtr->filterCoeff = ZxMath_F32toQ15(1.0);
  }
}

/**
 * @brief      Iir filter function.
 *
 * @param[inout]  ObjPtr: Filter input data and output data.
 * @param[in]  input: The input value this time.
 *
 * @return     None.
 *
 */
void IIR_FO_pushInput(IIR_FO_Obj* ObjPtr, q15 input)
{
  if(ObjPtr->isInitial) {
    ObjPtr->output = input;
    ObjPtr->isInitial = false;
  }
  else {
    ObjPtr->output = ObjPtr->output + ZxMath_MpyQ15((input - ObjPtr->output), ObjPtr->filterCoeff);
  }
}
/** @} end of group IIR_Public_Functions */

/** @} end of group IIR driver modules */

/** @} end of group Z20K11XM_Foc */