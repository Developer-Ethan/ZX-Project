/**************************************************************************************************/
/**
 * @file      : eventhandle.c
 * @brief     : eventhandle demo main file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Global_Structure.h"

HWSAMPLE_PARAM		HWSample;
RECONSTRUCT_PARAM	Reconstruct;
/**
 * @brief      Currentloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
q15 ElecFreq_LPF = 0;
void EventHandle_1ms(void)
{
	Pmsm_IRQHandleSpeedLoopIsr();
	
	Detection_FundamentalFault();
	
	ON_OFFControl();
	
	PwmOut_DiagInfor();
	
	Speed_Adjust();
	
	ElecFreq_LPF = (estObj.ElecFreq * (32767 - FREQ_COEFF_LPF) + ElecFreq_LPF * FREQ_COEFF_LPF) >> 15U;
	
	HWSample.BusCurrent = (adcData.BusCurrent * (32767 - BUSCURRENT_COEFF_LPF) + HWSample.BusCurrent * BUSCURRENT_COEFF_LPF) >> 15U;
	
	LoopControl(&HWSample,&LoopCtrl);
}

	/**
 * @brief      Currentloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
	
q15 Get_BusCurrentLimit(Foc_ObjType *pFocObj)
{
	q15 Target_BusCurrent = 0;
	
	Target_BusCurrent = (q15)(((float)POWER_TARGET / pFocObj->RealVdc) * 32767);

	if(Target_BusCurrent > LIMITCURRENT_VALUE_HWSAMP)//Sample current differ from real current about 0.5A
	{
		Target_BusCurrent = LIMITCURRENT_VALUE_HWSAMP;
	}
	return	(Target_BusCurrent);
}
/**
 * @brief      Currentloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

q15 Get_CurrentRealtime(HWSAMPLE_PARAM *pHWSample)
{
	q15 BusCurrent_RealTime = 0;

	BusCurrent_RealTime = pHWSample->BusCurrent;
		
	return (BusCurrent_RealTime);
}
