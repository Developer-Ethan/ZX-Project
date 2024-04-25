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

SPEED_CTRL_DEF Speed_Ctrl;
TIM_TO_DEF Time_To;
MC_DEF MotorCtrl;

/**
 * @brief      Detection_Key function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Detection_Key(PORT_ID_t port, PORT_GPIONO_t gpioNo)
{
  static uint16_t Status_keypin = 0;
  static uint16_t Press_count = 0;
  static uint16_t Loosen_count = 0;
  static uint16_t Status_key = 0;
  static uint16_t Event_key = 0;
  Status_keypin = GPIO_ReadPinLevel(port,gpioNo);
  if(1 == Status_keypin)
  {
    Loosen_count = 0;
    if(Press_count++ >= 5)
    {
      Press_count = 0;
      Status_key = 1;
    }
  }
  else
  {
    Press_count = 0;
    if(1 == Status_key)
    {
      if(Loosen_count++ >= 5)
      {
        Status_key = 0;
        Loosen_count = 0;
        Event_key = 1;
      }
    }
  }

  if(0 == ctrlObj.startMotor)
  {
    if(1 == Event_key)
    {
      Event_key = 0;
      ctrlObj.startMotor = 1;
    }
  }
  else
  {
    if(1 == Event_key)
    {
      Event_key = 0;
      ctrlObj.startMotor = 0;
    }
  }
}
/**
 * @brief      Obtain Z20A8300A status function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
Status_Z20A8300A GetStatus_Diag(void)
{
  Status_Z20A8300A Status_DiagPin = Z20A8300A_NoFault;
  Status_DiagPin = (Status_Z20A8300A)GPIO_ReadPinLevel(Z20A8300A_DIAG_PORT,Z20A8300A_DIAG_PIN); 
  return (Status_DiagPin);
}

/**
 * @brief      PWM_Capture Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
extern uint8_t Flag_TO_CH3Int;
uint8_t BorderFlag = 0;
void PWM_Capture(void)
{
	 static uint16_t ccvVal1 = 0;
	 static uint16_t ccvVal2 = 0,ccvVal2_Last = 0;
	 static uint32_t Pulse = 0,Pulse_Last = 0;
	 static uint32_t Period = 0,Period_Last = 0;
	 static uint16_t Duty = 0;
     static uint16_t OverFlowTime_Read;
	 static uint8_t CaptureCnt = 0;
	 static uint8_t OverFreqCnt = 0;
	
	ccvVal1 = TIM_GetCCVal(TIM2_ID, TIM_CHANNEL_2);
	ccvVal2 = TIM_GetCCVal(TIM2_ID, TIM_CHANNEL_3);
	
	if((Time_To.LeverState == STATUS_HL) || (Time_To.LeverState == STATUS_LL))
	{
		Time_To.LeverState = 0;
		OverFlowTime_Read = 0;
		Time_To.OverFlowCount = 0;
		ccvVal2_Last = 0;
		Period_Last = 0;
		Pulse_Last = 0;
	}
	else
	{
    	OverFlowTime_Read = Time_To.OverFlowCount;
	}

	Time_To.OverFlowCount = 0;
    Time_To.LowLeverCount = 0;
    Time_To.HighLeverCount = 0;
    
    if(ccvVal2 > ccvVal1)
    {
        Pulse = ccvVal2 - ccvVal1;
    }
    else
    {
        Pulse = ccvVal2 - ccvVal1 + 65535;
    }
	
	if(BorderFlag)
	{
		BorderFlag = 0;
		OverFlowTime_Read++;
	}
	
	if(Flag_TO_CH3Int)
	{
		Flag_TO_CH3Int = 0;
		
		if(ccvVal2 > 32767)
		{
			OverFlowTime_Read--;
			BorderFlag = 1;
		}
		else
		{
			BorderFlag = 0;
		}
	}
		
	Period = ccvVal2 - ccvVal2_Last + OverFlowTime_Read * 65535;
	
	if((Period > T_FREQ_MIN) || (Period < T_FREQ_MAX))
	{
		OverFreqCnt++;
		if(OverFreqCnt >= 2)
		{
			Time_To.OverFreqFlag = 1;
			CaptureCnt = 0;
			Time_To.LeverState = STATUS_OF;
		}
	}
	else
	{
		OverFreqCnt = 0;
		Time_To.LeverState = 0;
		Time_To.OverFreqFlag = 0;
		
		if(CaptureCnt < 2)
		{
			CaptureCnt++;
		}
		else
		{	
			CaptureCnt = 0;
			if(ZxMath_Abs((q15)(Period - Period_Last)) > 20)
			{
				Period = Period_Last;
			}
			Period = (Period + Period_Last) >> 1;
			Pulse = (Pulse + Pulse_Last) >> 1;
			Duty = (uint16_t)(((float)Pulse / Period)*32767);
			Speed_Ctrl.Duty = Duty;
		}
	}
	
	ccvVal2_Last = ccvVal2;
	Period_Last = Period;
	Pulse_Last = Pulse;
}

/**
 * @brief      TIM_ToIsr Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void TIM_ToIsr(void)
{
    Time_To.OverFlowCount++;
	
    if(GPIO_HIGH == GPIO_ReadPinLevel(PORT_A,GPIO_12))
    {
        Time_To.LowLeverCount = 0;
        if(Time_To.HighLeverCount++ >= 4)
        {
            Speed_Ctrl.Duty = DUTY_100;
            Time_To.OverFreqFlag = 0;
            Time_To.LeverState = STATUS_HL;
            Time_To.HighLeverCount = 0;
			Time_To.OverFlowCount = 0;
        }
    }
    else if(GPIO_LOW == GPIO_ReadPinLevel(PORT_A,GPIO_12))
    {
        Time_To.HighLeverCount = 0;
        if(Time_To.LowLeverCount++ >= 4)
        {
            Speed_Ctrl.Duty = DUTY_0;
            Time_To.OverFreqFlag = 0;
            Time_To.LeverState = STATUS_LL;
            Time_To.LowLeverCount = 0;
			Time_To.OverFlowCount = 0;
        }
    }
}

/**
 * @brief      Speed_Adjust Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Speed_Adjust(void)
{
	static q15 MinSpeed_ReduceLoad;
	
	if(Time_To.OverFreqFlag == 0)
	{
		if((Speed_Ctrl.Duty > DUTY_0) && (Speed_Ctrl.Duty <= DUTY_7))
		{
			Speed_Ctrl.SpeedRef = FREQUENCY_MAX;
		}	
		else if((Speed_Ctrl.Duty > DUTY_12) && (Speed_Ctrl.Duty <= DUTY_17))
		{
			Speed_Ctrl.SpeedRef = FREQUENCY_MIN;
		}
		else if((Speed_Ctrl.Duty > DUTY_17) && (Speed_Ctrl.Duty <= DUTY_93))
		{
			Speed_Ctrl.SpeedRef = (uint32_t)((Speed_Ctrl.Duty - DUTY_17) * FREQUENCY_K) + FREQUENCY_MIN;
		}
		else if(Speed_Ctrl.Duty > DUTY_93)
		{
			Speed_Ctrl.SpeedRef = FREQUENCY_MAX;
		}
	}
	else
	{
    	Speed_Ctrl.SpeedRef = FREQUENCY_MAX;
	}

	if(OC_REDUCELOAD_SPEED < OT_REDUCELOAD_SPEED)
	{
		MinSpeed_ReduceLoad = OC_REDUCELOAD_SPEED;
	}
	else
	{
		MinSpeed_ReduceLoad = OT_REDUCELOAD_SPEED;
	}
	
	if((Fault_All.Fault_OverCurrent.OcReduceLoadFlag == 1) && (Fault_All.Fault_OverTemperature.OtReduceLoadFlag == 0))
	{
		Speed_Ctrl.SpeedRef = OC_REDUCELOAD_SPEED;
	}
	else if((Fault_All.Fault_OverTemperature.OtReduceLoadFlag == 1) && (Fault_All.Fault_OverCurrent.OcReduceLoadFlag == 0))
	{
		Speed_Ctrl.SpeedRef = OT_REDUCELOAD_SPEED;
	}
	else if((Fault_All.Fault_OverTemperature.OtReduceLoadFlag == 1) && (Fault_All.Fault_OverCurrent.OcReduceLoadFlag == 1))
	{
		Speed_Ctrl.SpeedRef = MinSpeed_ReduceLoad;
	}
	else
	{
		Speed_Ctrl.SpeedRef = Speed_Ctrl.SpeedRef;
	}
}

/**
 * @brief      Speed_Adjust Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void ON_OFFControl(void)
{
	if(Time_To.OverFreqFlag == 0)
	{
        if(Speed_Ctrl.Duty == DUTY_0)
        {
            ctrlObj.startMotor = 0;
        }
        else if((Speed_Ctrl.Duty > DUTY_0 + HYSTERESIS) && (Speed_Ctrl.Duty <= DUTY_7))
		{
			ctrlObj.startMotor = 1;
		}	
		else if((Speed_Ctrl.Duty > DUTY_7 + HYSTERESIS) && (Speed_Ctrl.Duty <= DUTY_12))
		{
			ctrlObj.startMotor = 0;
		}
		else if(Speed_Ctrl.Duty > DUTY_12 + HYSTERESIS)
		{
			ctrlObj.startMotor = 1;
		}
	}
	else
	{
		ctrlObj.startMotor = 1;
	}
}
/**
 * @brief      Init_LIN Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Init_LIN(void)
{
	PORT_PinmuxConfig(HAL_LIN_SLP_N_PORT, HAL_LIN_SLP_N_PIN, HAL_LIN_SLP_N_PINMUX);
	GPIO_SetPinsDir(HAL_LIN_SLP_N_PORT,HAL_LIN_SLP_N_PIN);
	GPIO_SetPinOutput(HAL_LIN_SLP_N_PORT,HAL_LIN_SLP_N_PIN);
	PORT_PullConfig(HAL_LIN_SLP_N_PORT,HAL_LIN_SLP_N_PIN,PORT_PULL_UP);	
}