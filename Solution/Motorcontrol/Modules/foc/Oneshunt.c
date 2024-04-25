/**************************************************************************************************/
/**
 * @file      : Oneshunt.c
 * @brief     : Oneshunt module source file
 *              - Platform: Z20K14xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2021-2022 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Oneshunt.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup ONESHUNT 
 *  @brief ONESHUNT driver modules
 *  @{
 */

/** @defgroup ONESHUNT_Public_Functions
 *  @{
 */

/**
 * @brief      Caculate sample current function.
 *
 * @param[inout]  OneShuntPtr: real phase current id.
 * @param[in]  Sector: control sector.
 *
 * @return     None.
 *
 */
   #if 0
__ramfunc void OneShunt_CalcSampeCurrent(OneShunt_ObjType * ObjPtr, q15 Current1_pu, q15 Current2_pu, uint8_t Sector)
{
    switch(Sector)
    {
        case 1:
            if(ObjPtr->ShiftDuty1>1)
            {
             ObjPtr->Ia_pu = ObjPtr->IaLast;
             ObjPtr->Ic_pu = - Current2_pu;
            }
            else if(ObjPtr->ShiftDuty2>1)
            {
             ObjPtr->Ia_pu = Current1_pu;              
             ObjPtr->Ic_pu = ObjPtr->IcLast;
            }
            else
            {
            ObjPtr->Ia_pu = Current1_pu;
            ObjPtr->Ic_pu = - Current2_pu;              
            }
            ObjPtr->Ib_pu = - ObjPtr->Ia_pu - ObjPtr->Ic_pu;
            break;
          
        case 2:
            if(ObjPtr->ShiftDuty1>1)
            {
            ObjPtr->Ib_pu = ObjPtr->IbLast;
            ObjPtr->Ic_pu = - Current2_pu;
            }
            else if(ObjPtr->ShiftDuty2>1)
            {
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ic_pu = ObjPtr->IcLast;              
            }
            else
            {
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ic_pu = - Current2_pu;
            }
            ObjPtr->Ia_pu = - ObjPtr->Ib_pu - ObjPtr->Ic_pu;
            break; 
          
        case 3:
            if(ObjPtr->ShiftDuty1>1)
            {
            ObjPtr->Ib_pu = ObjPtr->IbLast;
            ObjPtr->Ia_pu = - Current2_pu;             
            }
            else if(ObjPtr->ShiftDuty2>1) 
            {
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ia_pu = ObjPtr->IaLast;              
            }
            else
            {
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ia_pu = - Current2_pu;
            }
            ObjPtr->Ic_pu = - ObjPtr->Ib_pu - ObjPtr->Ia_pu; 
            break;
          
        case 4:
            if(ObjPtr->ShiftDuty1>1)
            {
            ObjPtr->Ic_pu = ObjPtr->IcLast;
            ObjPtr->Ia_pu = - Current2_pu;              
            }
            else if(ObjPtr->ShiftDuty2>1)
            {
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ia_pu = ObjPtr->IaLast;              
            }
            else
            {
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ia_pu = - Current2_pu;
            }
            ObjPtr->Ib_pu = - ObjPtr->Ia_pu - ObjPtr->Ic_pu; 
            break;  
          
        case 5:
            if(ObjPtr->ShiftDuty1>1)
            {
            ObjPtr->Ic_pu = ObjPtr->IcLast;
            ObjPtr->Ib_pu = - Current2_pu;              
            }
            else if(ObjPtr->ShiftDuty2>1)
            {
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ib_pu = ObjPtr->IbLast;              
            }
            else
            {
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ib_pu = - Current2_pu;
            }
            ObjPtr->Ia_pu = - ObjPtr->Ib_pu - ObjPtr->Ic_pu; 
            break;  
          
        case 6:
            if(ObjPtr->ShiftDuty1>1)
            {
            ObjPtr->Ia_pu = ObjPtr->IaLast;
            ObjPtr->Ib_pu = - Current2_pu;              
            }
            else if(ObjPtr->ShiftDuty2>1)
            {
            ObjPtr->Ia_pu = Current1_pu;
            ObjPtr->Ib_pu = ObjPtr->IbLast;              
            }
            else
            {
            ObjPtr->Ia_pu = Current1_pu;
            ObjPtr->Ib_pu = - Current2_pu;
            }
            ObjPtr->Ic_pu = - ObjPtr->Ia_pu - ObjPtr->Ib_pu; 
            break; 
            
        default:
            ObjPtr->Ia_pu = 0U;
            ObjPtr->Ib_pu = 0U;
            ObjPtr->Ic_pu = 0U; 
            break;   
    }
    
          ObjPtr->IaLast = ObjPtr->Ia_pu;
          ObjPtr->IbLast = ObjPtr->Ib_pu;
          ObjPtr->IcLast = ObjPtr->Ic_pu;   
}
#endif

#if 1
__ramfunc void OneShunt_CalcSampeCurrent(OneShunt_ObjType * ObjPtr, q15 Current1_pu, q15 Current2_pu, uint8_t Sector)
{
    switch(Sector)
    {
        case 1:
            ObjPtr->Ia_pu = Current1_pu;
            ObjPtr->Ic_pu = - Current2_pu;
            ObjPtr->Ib_pu = - ObjPtr->Ia_pu - ObjPtr->Ic_pu;
            break;
          
        case 2:
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ic_pu = - Current2_pu;
            ObjPtr->Ia_pu = - ObjPtr->Ib_pu - ObjPtr->Ic_pu;
            break; 
          
        case 3:
            ObjPtr->Ib_pu = Current1_pu;
            ObjPtr->Ia_pu = - Current2_pu;
            ObjPtr->Ic_pu = - ObjPtr->Ib_pu - ObjPtr->Ia_pu; 
            break;
          
        case 4:
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ia_pu = - Current2_pu;
            ObjPtr->Ib_pu = - ObjPtr->Ia_pu - ObjPtr->Ic_pu; 
            break;  
          
        case 5:
            ObjPtr->Ic_pu = Current1_pu;
            ObjPtr->Ib_pu = - Current2_pu;
            ObjPtr->Ia_pu = - ObjPtr->Ib_pu - ObjPtr->Ic_pu; 
            break;  
          
        case 6:
            ObjPtr->Ia_pu = Current1_pu;
            ObjPtr->Ib_pu = - Current2_pu;
            ObjPtr->Ic_pu = - ObjPtr->Ia_pu - ObjPtr->Ib_pu; 
            break; 
            
        default: 
            ObjPtr->Ia_pu = 0U;
            ObjPtr->Ib_pu = 0U;
            ObjPtr->Ic_pu = 0U; 
            break;   
    }
    
}
#endif

/** @} end of group ONESHUNT_Public_Functions */

/** @} end of group ONESHUNT driver modules */

/** @} end of group Z20K11XM_Foc */