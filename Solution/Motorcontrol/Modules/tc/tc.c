/**************************************************************************************************/
/**
 * @file      : tc.c
 * @brief     : Speed and Torque control file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "tc.h"
#include "Global_Structure.h"
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup TC 
 *  @brief TC driver modules
 *  @{
 */

/** @defgroup TC_Global_Variables
 *  @{
 */

/** @defgroup TC_Public_Functions
 *  @{
 */

extern TC_Obj          tcObj;

/**
 * @brief      TC_Init  function.
 *
 * @param[inout]  obj: Speed,current and fw control data address.
 * @param[in]  pParams: Motor parameters and control limit data address.
 *
 * @return     None.
 *
 */
void TC_init(TC_Obj* obj, TC_Params* pParams)
{
  obj->speedRef = 0;
  obj->IsMax = ZxMath_F32toQ15(pParams->maxCurrent_A/pParams->IBase_A);
  PID_init(&obj->pidSpd, TC_KP_GAIN, TC_KI_GAIN, TC_KD_GAIN, ZxMath_F32toQ15(0), obj->IsMax);
  PID_init(&obj->pidPow, POW_KP_GAIN, POW_KI_GAIN, POW_KD_GAIN, ZxMath_F32toQ15(0), obj->IsMax);
  RAMP_init(&obj->spdRamp, SPDRAMP_LOOP1_STEP);
}

/**
 * @brief      TC_Run  function.
 *
 * @param[inout]  ObjPtr: speed,current data address.
 *
 * @return     None.
 *
 */
void TC_run(TC_Obj* obj)
{
    // Ramp for the reference
    	
	if(LoopType == SpeedLoop)
	{
		if(LoopCtrl.PowerLoopFlag == 1)
		{
			LoopCtrl.PowerLoopFlag = 0;
			obj->pidSpd.Ui = obj->IqRef;
		}
		obj->spdRamp.target = obj->speedRef;
		RAMP_calc(&obj->spdRamp);
		obj->pidSpd.ref = obj->spdRamp.output;
    	obj->pidSpd.fdb = obj->speed;
    	PID_run(&obj->pidSpd);
		obj->IqRef = obj->pidSpd.output;
	}
	else if(LoopType == PowerLoop)
	{
		if(LoopCtrl.SpeedLoopFlag == 1)
		{
			LoopCtrl.SpeedLoopFlag = 0;
			obj->pidPow.Ui = obj->IqRef;
		}
		
		obj->pidPow.ref = obj->speedRef;//obj->PowRamp.output;
    	obj->pidPow.fdb = obj->speed;
		PID_run(&obj->pidPow);//power loop control
		obj->IqRef = obj->pidPow.output;
	}
	  
    // Confirm the final IqRef   
    if(obj->IqRef > obj->IsMax) obj->IqRef = obj->IsMax;
}

/**
 * @brief      Reset tc function.
 *
 * @param[inout]  None.
 *
 * @return     None.
 *
 */
void Tc_Reset()
{
    tcObj.pidSpd.Ui = 0;
    tcObj.IdRef = 0;
    tcObj.IqRef = 0; 
    tcObj.spdRamp.step = SPDRAMP_LOOP1_STEP;
}

/**
 * @brief      Set step value in loop2 function.
 *
 * @param[inout]  None.
 *
 * @return     None.
 *
 */
void TC_setloop2_step()
{
  tcObj.spdRamp.step = SPDRAMP_LOOP2_STEP;  
}
/**
 * @brief      Set step value in loop3 function.
 *
 * @param[inout]  None.
 *
 * @return     None.
 *
 */
void TC_setloop3_step()
{
  tcObj.spdRamp.step = SPDRAMP_LOOP3_STEP;  
}
/** @} end of group TC_Public_Functions */

/** @} end of group TC driver modules */

/** @} end of group Z20K11XM_Foc */