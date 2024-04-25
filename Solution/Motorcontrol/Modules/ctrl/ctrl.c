/**************************************************************************************************/
/**
 * @file       ctrl.c
 * @brief      Control state machine file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "ctrl.h"

extern CTRL_Obj        ctrlObj;
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup CTRL 
 *  @brief CTRL driver modules
 *  @{
 */

/** @defgroup CTRL_Global_Variables
 *  @{
 */

/** @defgroup CTRL_Public_Functions
 *  @{
 */
/**
 * @brief      CTRL initial function.
 *
 * @param[inout]  ObjPtr: Align and startup and freq and related control data address.
 * @param[in]  ParamsPtr: Start up control parameters address.
 *
 * @return     None.
 *
 */
void CTRL_init(CTRL_Obj* ObjPtr , CTRL_ParamsType* ParamsPtr)
{
    scm_reg_t * SCMx = (scm_reg_t *)(ParamsPtr->scmBaseAddr);
    while (0 != SCMx->SCM_DEVID.FAMILY_ID);
    // Set parameters
    ObjPtr->steadytime = ParamsPtr->steadytime;
    ObjPtr->calitime = ParamsPtr->calitime;
    ObjPtr->needCalibI = false;
    ObjPtr->needRotorAlign = true;
    ObjPtr->rotorAlignCurrent_pu = ParamsPtr->rotorAlignCurrentA;

    // Initialize values
    ObjPtr->enablePwm = false;
    ObjPtr->enableCurrentCalib = false;
    ObjPtr->enableVoltageCalib = false;
    ObjPtr->startMotor = false;
    ObjPtr->freqRef = 0;
    ObjPtr->freqfb = 0;
    ObjPtr->Cntr = 0U;
    ObjPtr->state = CTRL_State_Powerup;
    ObjPtr->biasThreshold = ParamsPtr->biasThreshold;
    ObjPtr->rotorAlignTimeMs = ParamsPtr->rotorAlignTimeMs;
    ObjPtr->openloopMaxtimeMs = ParamsPtr->openloopMaxtimeMs;
    ObjPtr->loop1TimeMs = ParamsPtr->loop1TimeMs;
    ObjPtr->cntThreshold = ParamsPtr->cntThreshold;
    ObjPtr->LFreqThresholdHz = ParamsPtr->LFreqThresholdHz;
    ObjPtr->finalFreqHz = ParamsPtr->finalFreqHz;
    RAMP_init(&ObjPtr->rotorAlignCurrentRamp, ParamsPtr->alignCurrentStep);
    //0.02 AMP drop down
    RAMP_init(&ObjPtr->IFCurrentRamp, ParamsPtr->IFCurrentStep);        
    TIMER_init(&ObjPtr->steadyTimer);
    TIMER_init(&ObjPtr->calibTimer);
    TIMER_init(&ObjPtr->rotorAlignTimer);
    TIMER_init(&ObjPtr->IFTimer);
    TIMER_init(&ObjPtr->closeLoop1Timer);
}
/**
 * @brief      Switch to closeloop function.
 *
 * @param[inout]  None.
 *
 * @return     None.
 *
 */
void CTRL_Switchto_Closeloop3()
{
    if (ctrlObj.state > CTRL_State_Openloop)
    {
        TC_setloop3_step();
        ctrlObj.state = CTRL_State_CloseLoop3;
    }
    else 
    {
        ctrlObj.enablePwm = false;
        ctrlObj.state = CTRL_State_Idle;
        Foc_Reset();
        Tc_Reset();
    }
}
/**
 * @brief      Update the status function.
 *
 * @param[inout]  ObjPtr: Control state data address.
 *
 * @return     None.
 *
 */
void CTRL_updateState(CTRL_Obj* ObjPtr)
{
    switch (ObjPtr->state)
    {
    case CTRL_State_Powerup:
        TIMER_start(&ObjPtr->steadyTimer, ObjPtr->steadytime);
        ObjPtr->state = CTRL_State_Precharging;
        break;

    case CTRL_State_Precharging:
        if (TIMER_timeout(&ObjPtr->steadyTimer))
        {
            ObjPtr->needCalibI = true;
            ObjPtr->state = CTRL_State_CalibratingI;
            TIMER_start(&ObjPtr->calibTimer, ObjPtr->calitime);
        }
        break;

    case CTRL_State_CalibratingI:
        if (TIMER_timeout(&ObjPtr->calibTimer))
        {
			if(ObjPtr->biasFilterCnt++ < 64)
			{
				ObjPtr->Ia_pu_Sum += ObjPtr->Ia_pu;
				ObjPtr->Ib_pu_Sum += ObjPtr->Ib_pu;
				ObjPtr->Ic_pu_Sum += ObjPtr->Ic_pu;				
			}
			else
			{
				ObjPtr->biasFilterCnt = 0;
				
				ObjPtr->Ia_pu = ObjPtr->Ia_pu_Sum >> 6;
				ObjPtr->Ib_pu = ObjPtr->Ib_pu_Sum >> 6;
				ObjPtr->Ic_pu = ObjPtr->Ic_pu_Sum >> 6;	
				
			    // Check if current sensing has issues
				if (ZxMath_Abs(ObjPtr->Ia_pu) > ObjPtr->biasThreshold
					|| ZxMath_Abs(ObjPtr->Ib_pu) > ObjPtr->biasThreshold
					|| ZxMath_Abs(ObjPtr->Ic_pu) > ObjPtr->biasThreshold)
				{
					g_errObj.faultCode = ERR_CT_Failure;
				}
				
				ObjPtr->needCalibI = false;
				ObjPtr->state = CTRL_State_Idle;
			}
        }
        break;
        
    case CTRL_State_Idle:
        if (ObjPtr->startMotor == true)
        {
            if (ObjPtr->needRotorAlign == true)
            {
                ObjPtr->rotorAlignCurrentRamp.output = 0;
                ObjPtr->rotorAlignCurrentRamp.target = ObjPtr->rotorAlignCurrent_pu;
                TIMER_start(&ObjPtr->rotorAlignTimer, ObjPtr->rotorAlignTimeMs);
                ObjPtr->state = CTRL_State_RotorAlign;
                ObjPtr->enablePwm = true;
            }
        }
        if (g_errObj.faultCode != 0)
        {
            ObjPtr->state = CTRL_State_Error;
        }
        break;

    case CTRL_State_RotorAlign:
        if (TIMER_timeout(&ObjPtr->rotorAlignTimer))
        {
            CTRL_Resetopenloopdata();
            TIMER_start(&ObjPtr->IFTimer, ObjPtr->openloopMaxtimeMs);
            ObjPtr->state = CTRL_State_Openloop;
        }
        if (g_errObj.faultCode != 0)
        {
            ObjPtr->state = CTRL_State_Error;
        }
        break;

    case CTRL_State_Openloop:
        if ((ObjPtr->Cntr > ObjPtr->cntThreshold && ObjPtr->freqfb > ObjPtr->LFreqThresholdHz)
            || (TIMER_timeout(&ObjPtr->IFTimer)))
        {
            ObjPtr->freqRef = ObjPtr->finalFreqHz;
            CTRL_switchToCloseLoop();
            ObjPtr->state = CTRL_State_CloseLoop1;
            TIMER_start(&ObjPtr->closeLoop1Timer, ObjPtr->loop1TimeMs);
        }
		if (ObjPtr->startMotor == false)
        {
            TC_setloop3_step();
            ObjPtr->state = CTRL_State_CloseLoop3;
        }
        if (g_errObj.faultCode != 0)
        {
            ObjPtr->state = CTRL_State_Error;
        }
        break;

    case CTRL_State_CloseLoop1:
        if (TIMER_timeout(&ObjPtr->closeLoop1Timer))
        {
            TC_setloop2_step();
            ObjPtr->state = CTRL_State_CloseLoop2;
        }
        if (g_errObj.faultCode != 0)ObjPtr->state = CTRL_State_Error;
        break;

    case CTRL_State_CloseLoop2:
        if (ObjPtr->startMotor == false)
        {
            TC_setloop3_step();
            ObjPtr->state = CTRL_State_CloseLoop3;
        }
        if (g_errObj.faultCode != 0)ObjPtr->state = CTRL_State_Error;
        break;

    case CTRL_State_CloseLoop3:
        if (ObjPtr->enablePwm == false)
        {
            ObjPtr->state = CTRL_State_Idle;
            Foc_Reset();
            Tc_Reset();
        }
        if (g_errObj.faultCode != 0)
        {
            ObjPtr->state = CTRL_State_Error;
        }
        break;

    case CTRL_State_Error:
         ObjPtr->enablePwm = false;
         Foc_Reset();
         Tc_Reset();
        break;
    default:
        break;
    }
}
/** @} end of group CTRL_Public_Functions */

/** @} end of group CTRL driver modules */

/** @} end of group Z20K11XM_Foc */