/**************************************************************************************************/
/**
 * @file      : Oneshunt.h
 * @brief     : Oneshunt module header file
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef ONESHUNT_H
#define ONESHUNT_H

#include "zxmathfp.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  ONESHUNT
 *  @{
 */

/** @defgroup ONESHUNT_Public_Types 
 *  @{
 */

/**
 *  @brief ONESHUNT paramter of calculation type definition
 */
typedef struct 
{
    q15 Ia_pu;              /*!< Real Iu */
    q15 Ib_pu;              /*!< Real Iv */
    q15 Ic_pu;              /*!< Real Iw */
	q24 Ia_Sum;
	q24 Ib_Sum;
	q24 Ic_Sum;
    q15 IaLast;
    q15 IbLast;
    q15 IcLast;   
    uint16_t ShiftDuty1;
    uint16_t ShiftDuty2;    
} OneShunt_ObjType;
/** @} end of group ONESHUNT_Public_Types */

/** @defgroup ONESHUNT_Public_FunctionDeclaration
 *  @brief ONESHUNT functions statement
 *  @{
 */

/**
 * @brief      Caculate sample current function.
 *
 * @param[inout]  ObjPtr: real phase current id.
 * @param[in]  Current1_pu Current2_pu: current value in one shuntsample.
 * @param[in]  Sector: control sector.
 *
 * @return     None.
 *
 */
__ramfunc void OneShunt_CalcSampeCurrent(OneShunt_ObjType * ObjPtr, q15 Current1_pu, q15 Current2_pu,uint8_t Sector);

/** @} end of group ONESHUNT_Public_FunctionDeclaration */

/** @} end of group ONESHUNT */

/** @} end of group Z20K11XM_Foc */
#endif  /* ONESHUNT_H */