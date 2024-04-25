/**************************************************************************************************/
/**
 * @file      : timer.c
 * @brief     : Software timer file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#include "timer.h"

/** @addtogroup  Z20K14XM_Foc
 *  @{
 */

/** @defgroup TIMER 
 *  @brief TIMER driver modules
 *  @{
 */

/** @defgroup TIMER_Public_Functions
 *  @{
 */

/**
 * @brief      Software timer initial function.
 *
 * @param[inout]  ObjPtr: Counter,period and bool cotnrol address.
 *
 * @return     None.
 *
 */
void TIMER_init(TIMER_Obj* ObjPtr)
{
  ObjPtr->counter = 0;
  ObjPtr->period = 0;
  ObjPtr->enable = false;
  ObjPtr->overflow = false;
}
/** @} end of group TIMER_Public_Functions */

/** @} end of group TIMER driver modules */

/** @} end of group Z20K11XM_Foc */