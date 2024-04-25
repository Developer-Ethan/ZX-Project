/**************************************************************************************************/
/**
 * @file      : err.h
 * @brief     : Erro marker header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef ERR_H
#define ERR_H


#include <stdint.h>

/** @addtogroup  Z20K14XM_Foc
 *  @{
 */

/** @addtogroup  ERR
 *  @{
 */

/** @defgroup ERR_Public_Types 
 *  @{
 */

/** @defgroup ERR_Private_Defines
 *  @{
 */
/** @} end of group ERR_Private_Defines */
   
/** @defgroup ERR_Public_Types 
 *  @{
 */

/**
 *  @brief erro type definition
 */
typedef enum
{
  ERR_No_Erro                = 0,    //!< No erro  
  ERR_Motor_WindShort        = 1,    //!< The motor winding is shorting   
  ERR_Motor_OverLoad         = 2,    //!< The motor load is too high
  ERR_Motor_PhaseLoss        = 3,    //!< The motor phase is loss or open (might be some connection error)
  ERR_Inv_OverVoltage        = 4,    //!< The inverter's voltage is too high
  ERR_Inv_UnderVoltage       = 5,    //!< The inverter's voltage is too low
  ERR_Drive_fault            = 6,    //!< The inverter's IPM has fault signal over current  
  ERR_CT_Failure             = 7,    //!< The current sensor failure
  ERR_MCU_Fault              = 8,
} ERR_Type_e;


typedef struct _ERR_Obj_
{
  uint16_t faultCode;  //!< fault code's each bit stands for one kind of fault
  uint8_t LV_ctr;      //!< Record the low voltage times
  uint8_t HV_ctr;      //!< Record the high voltage times
  uint8_t HT_ctr;      //!< Record the high temperature times
  uint8_t OC_ctr;      //!< Record the over current times
  uint8_t ZS_ctr;      //!< Record the zero speed times  

} ERR_Obj;


extern ERR_Obj g_errObj;
/** @} end of group EER_Public_Types */

// **************************************************************************
// the function prototypes

extern void ERR_init(ERR_Obj* obj);


static inline void ERR_errorOccur(ERR_Obj* obj, ERR_Type_e errorType)
{
  obj->faultCode |= errorType;
}


static inline uint32_t ERR_getFaultCode(ERR_Obj* obj)
{
  return obj->faultCode;
}


/** @} end of group ERR_Public_FunctionDeclaration */

/** @} end of group ERR */

/** @} end of group Z20K11XM_Foc */

#endif /* ERR_H */
