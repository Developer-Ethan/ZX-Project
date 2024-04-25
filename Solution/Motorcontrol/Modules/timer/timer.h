/**************************************************************************************************/
/**
 * @file      : timer.h
 * @brief     : Software timer header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  TIMER
 *  @{
 */
/** @defgroup TIMER_Public_Types 
 *  @{
 */

/**
 *  @brief TIMER control params type definition
 */
typedef struct _TIMER_Obj_
{
  uint32_t  counter;  //!< The counter register
  uint32_t  period;   //!< The period, time out will occur when count reach period
  bool      enable;   //!< Whether the timer is started
  bool      overflow; //!< Whether the timer is overflow
  
} TIMER_Obj;

/** @} end of group TIMER control types definitions */


//!
extern void TIMER_init(TIMER_Obj* ObjPtr);


static inline void TIMER_start(TIMER_Obj* ObjPtr, uint32_t period)
{
  ObjPtr->counter = 0;
  ObjPtr->period = period;
  ObjPtr->enable = true;
  ObjPtr->overflow = false;
}

static inline void TIMER_tick(TIMER_Obj* ObjPtr)
{
  if(ObjPtr->enable && ObjPtr->overflow == false) {
    if(ObjPtr->counter < ObjPtr->period) {
      ObjPtr->counter++;
      if(ObjPtr->counter == ObjPtr->period) {
        ObjPtr->overflow = true;
      }
    }
  }
}



static inline bool TIMER_timeout(TIMER_Obj* ObjPtr)
{
  return ObjPtr->overflow;
}


static inline bool TIMER_isActive(TIMER_Obj* ObjPtr)
{
   return ObjPtr->enable;
}

/** @}end of group TIMER FunctionDeclaration */

/** @}end of group TIMER */

/** @}end of group Z20K11XM_Foc */

#endif /* TIMER_H */
