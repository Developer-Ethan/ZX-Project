/**************************************************************************************************/
/**
 * @file      : err.c
 * @brief     : Erro marker file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#include "err.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup ERR 
 *  @brief ERR driver modules
 *  @{
 */

/** @defgroup ERR_Public_Functions
 *  @{
 */
ERR_Obj g_errObj;

/**
 * @brief      Intitial parameters function.
 *
 * @param[inout]  obj: fault mark and record counters.
 *
 * @return     None.
 *
 */
void ERR_init(ERR_Obj* obj)
{
  obj->faultCode = 0;
  obj->LV_ctr = 0;
  obj->HV_ctr = 0;
  obj->HT_ctr = 0; 
  obj->OC_ctr = 0;
  obj->ZS_ctr = 0;  
    
}
/** @} end of group ERR_Public_Functions */

/** @} end of group ERR driver modules */

/** @} end of group Z20K11XM_Foc */