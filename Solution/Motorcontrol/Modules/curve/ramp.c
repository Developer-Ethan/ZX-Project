/**************************************************************************************************/
/**
 * @file      : ramp.c
 * @brief     : Ramp caculation head file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#include "ramp.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup RAMP 
 *  @brief RAMP driver modules
 *  @{
 */

/** @defgroup RAMP_Public_Functions
 *  @{
 */

/**
 * @brief      Exponential initial function.
 *
 * @param[inout]  ObjPtr: Target,output and deadband data.
 * @param[in]  step: Decrease or increase value every time.
 *
 * @return     None.
 *
 */
void RAMP_init(RAMP_Obj* ObjPtr, q15 step)
{
    ObjPtr->target = 0;
    ObjPtr->output = 0;
    ObjPtr->step = step;
}
/**
 * @brief      Ramp  function.
 *
 * @param[inout]  ObjPtr: input and output data.
 *
 * @return     None.
 *
 */
void RAMP_calc(RAMP_Obj* ObjPtr)
{
    q15 step = ZxMath_Abs(ObjPtr->step);

    if (ZxMath_Abs(ObjPtr->target - ObjPtr->output) > step)
    {
        if (ObjPtr->target > ObjPtr->output)
        {
            ObjPtr->output += step;
        }
        else
        {
            ObjPtr->output -= step;
        }
    }
    else {
        // less than one step needed, just jump to the target
        ObjPtr->output = ObjPtr->target;
    }
}
/** @} end of group RAMP_Public_Functions */

/** @} end of group RAMP driver modules */

/** @} end of group Z20K11XM_Foc */