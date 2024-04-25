/**************************************************************************************************/
/**
 * @file      : communication.c
 * @brief     : communication demo main file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Global_Structure.h"

	 
LOOPCTRL_DEF	LoopCtrl;
LOOPTYPE_DEF 	LoopType;
/**
 * @brief      Speed_Adjust Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

q15 Loop_Control_Ref(q15 SpeedRef,q15 PowerRef)
{
	static q15 ReferenceValue = 0;
	switch(LoopType)
	{
		case SpeedLoop:
			if(Speed_Ctrl.SpeedLimFlag == 0)
			{
				ReferenceValue = SpeedRef;
			}
			else
			{
				ReferenceValue = SPEED_LIMIT;
			}
			break;
			
		case PowerLoop:			
			ReferenceValue = PowerRef;			
			break;	
		}
	
	return (ReferenceValue);
}

/**
 * @brief      Speed_Adjust Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

q15 Loop_Control_Feedback(q15 SpeedBack,q15 PowerBack)
{
	static q15 FeedbackValue = 0;
	switch(LoopType)
	{
		case SpeedLoop:			
			FeedbackValue = SpeedBack;		
			break;
			
		case PowerLoop:			
			FeedbackValue = PowerBack;			
			break;	
	}
	
	return (FeedbackValue);
}

/**
 * @brief      Speed_Adjust Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void LoopControl(HWSAMPLE_PARAM *HwSample,LOOPCTRL_DEF *LoopCtrl)
{
	if(ctrlObj.state == CTRL_State_CloseLoop2)
	{
		if(LoopType == SpeedLoop)
		{
			if((Speed_Ctrl.SpeedRef >= SPEED_LOOPSWITCH) && (Get_CurrentRealtime(&HWSample) >= CURRENT_DRYROTOR) \
				&& (Speed_Ctrl.SpeedLimFlag == 0))
			{
				if(LoopCtrl->SpeedLoopCnt++ >= TIME_SPEEDLOOP)
				{				
					LoopCtrl->SpeedLoopCnt = 0;
					LoopType = PowerLoop;
					LoopCtrl->PowerLoopFlag = 1;
				}
			}
			else if((Speed_Ctrl.SpeedLimFlag == 1) && (Get_CurrentRealtime(&HWSample) >= LIMITCURRENT_VALUE_HWSAMP))
			{
				if(LoopCtrl->LoopSwitchCnt++ >= TIME_SPEEDLOOP)
				{
					LoopCtrl->LoopSwitchCnt = 0;
					LoopType = PowerLoop;
					Speed_Ctrl.SpeedLimFlag = 0;
				}
			}
			else
			{
				if(LoopCtrl->SpeedLoopCnt > 0)
				{
					LoopCtrl->SpeedLoopCnt--;
				}
				if(LoopCtrl->LoopSwitchCnt > 0)
				{
					LoopCtrl->LoopSwitchCnt--;
				}
			}
		}
		else if(LoopType == PowerLoop)
		{
			if(Speed_Ctrl.SpeedRef <= (SPEED_LOOPSWITCH - SPEED_RETURN))
			{
				if(LoopCtrl->PowerLoopCnt++ >= TIME_POWERLOOP)
				{
					LoopCtrl->PowerLoopCnt = 0;
					LoopType = SpeedLoop;
					LoopCtrl->SpeedLoopFlag = 1;
				}
			}
			else
			{
				LoopCtrl->PowerLoopCnt = 0;
			}
				
			if(ElecFreq_LPF >= SPEED_LIMIT)
			{
				if(Speed_Ctrl.SpeedLimCnt++ >= SPEED_LIMIT_DETECT_TIME)
				{
					Speed_Ctrl.SpeedLimCnt = 0;
					Speed_Ctrl.SpeedLimFlag = 1;
					LoopType = SpeedLoop;
				}
			}
			else
			{
				if(Speed_Ctrl.SpeedLimCnt > 0)
				{
					Speed_Ctrl.SpeedLimCnt--;
				}
			}
		}
	}
}