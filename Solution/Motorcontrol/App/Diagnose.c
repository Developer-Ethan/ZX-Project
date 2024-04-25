/**************************************************************************************************/
/**
 * @file      : diagnose.c
 * @brief     : diagnose demo main file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Global_Structure.h"

DIAG_INFOR_DEF Diag_Infor_DryRotor;
DIAG_INFOR_DEF Diag_Infor_StallOrOc;
DIAG_INFOR_DEF Diag_Infor_Ot;
DIAG_INFOR_DEF Diag_Infor_LossSpeed;
DIAG_INFOR_DEF Diag_Infor_NoFault;
DIAG_INFOR_DEF Diag_Infor;
STATUS_CONDITION_NOFAULT GetCondition_NoFault = Condition_Meet;
/**
 * @brief      Confirm_Prior Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

PRIOR_FAULTCODE_DEF Confirm_Prior(FAULT_ALL_TYPE_DEF *pFault_All_Type)
{
	static PRIOR_FAULTCODE_DEF HighPrior = NoFault;
	
	if(pFault_All_Type->all & 0X0020)
	{
		HighPrior = DryRotor;
	}
	else if(pFault_All_Type->all & 0X0018)
	{
		HighPrior = StallOrOc;
	}
	else if(pFault_All_Type->all & 0X0004)
	{
		HighPrior = OverTemperature;
	}
	else if(pFault_All_Type->all & 0X0040)
	{
		HighPrior = LossSpeed;
	}
	else if(Condition_Meet == GetCondition_NoFault)
	{
		HighPrior = NoFault;
	}
	
	return (HighPrior);
}

/**
 * @brief      Send_DiagInfor Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void PwmOut_DiagInfor(void)
{   
	static PRIOR_FAULTCODE_DEF Prior_FaultCode = NoFault;
	static PRIOR_FAULTCODE_DEF Prior_FaultCode_Last = NoFault;
	static PRIOR_FAULTCODE_DEF Prior_FaultCode1 = NoFault;

	static STATUS_CONDITION_NOFAULT GetCondition_NoFault_Last1 = Condition_Meet;
	static STATUS_CONDITION_NOFAULT GetCondition_NoFault_Last2 = Condition_Meet;
	
	static uint8_t FirstStatus = 0;
	
    GetCondition_NoFault = Condition_Diag_NoFault();  
	Prior_FaultCode1 = Confirm_Prior(&Fault_All_Type);

/*get prior_faultcode logic*/

	if(GetCondition_NoFault == Condition_NoMeet)
	{
		if(GetCondition_NoFault_Last1 == Condition_Meet) // state switch
		{			
			if(Diag_Infor.SendDoneFlag == 1)
			{
				Diag_Infor.SendDoneFlag = 0;
				GetCondition_NoFault_Last1 = Condition_NoMeet;
				Prior_FaultCode = Prior_FaultCode1;				
			}
		}
		else if(GetCondition_NoFault_Last1 == Condition_NoMeet) // state switch
		{
			if(FirstStatus == 0)
			{
				FirstStatus = 1;
				Prior_FaultCode = Prior_FaultCode1;
			}
			else
			{			
				if(Diag_Infor.SendDoneFlag == 1)
				{
					Diag_Infor.SendDoneFlag = 0;
					Diag_Infor.SendDoneCnt++;
				}
				if((Diag_Infor.SendDoneCnt <= SEND_TIME) && (Diag_Infor.SendDoneCnt > 0)) //the first state is the newest obtained
				{                                                                         //and after is the last status
					Prior_FaultCode = Prior_FaultCode_Last;
				}
				else if(Diag_Infor.SendDoneCnt > SEND_TIME)
				{
					Diag_Infor.SendDoneCnt = 0;
					FirstStatus = 0; // update prior_faultcode every SEND_TIME
				}

				Prior_FaultCode_Last = Prior_FaultCode;
			}
		}
		
		GetCondition_NoFault_Last2 = GetCondition_NoFault;
	}
	else if(GetCondition_NoFault == Condition_Meet)
	{		
		if(GetCondition_NoFault_Last2 == Condition_NoMeet) // state switch
		{
			if((Diag_Infor.SendDoneCnt <= SEND_TIME) && (Diag_Infor.SendDoneCnt > 0))
			{
				if(Diag_Infor.SendDoneFlag == 1)
				{
					Diag_Infor.SendDoneCnt++;
					Diag_Infor.SendDoneFlag = 0;
				}
			}
			else
			{
				Diag_Infor.SendDoneCnt = 0;
				if(Diag_Infor.SendDoneFlag == 1)
				{
					Diag_Infor.SendDoneFlag = 0;
					Prior_FaultCode = Prior_FaultCode1;
					GetCondition_NoFault_Last2 = Condition_Meet;
				}
			}
		}
		else if(GetCondition_NoFault_Last2 == Condition_Meet) // state switch
		{
			Prior_FaultCode = Prior_FaultCode1;
			
			if((Diag_Infor.SendDoneFlag == 1) && (Fault_DiagVoltage.FailureFlag_1st == 0))
			{
				Diag_Infor.SendDoneFlag = 0;
			}
		}
		
		GetCondition_NoFault_Last1 = GetCondition_NoFault;
	}
	
/*diagnose information send*/
    if(Enable_DiagOutput == Get_Status_DiagOutput())
    {
        switch(Prior_FaultCode)
        {
            case DryRotor:
                    
                if(Flag_AllFaultSend.Fault_DryRotor.FaultSendFlag)
                {
                    Duty_DiagInfor(T_HIGH_DRYROTOR,T_LOW_DRYROTOR,PWM_OUT_PORT,PWM_OUT_PIN,&Diag_Infor);
                }

            break;
                
            case StallOrOc:
				
                if((Flag_AllFaultSend.Fault_OverCurrent.FaultSendFlag) ||\
					(Flag_AllFaultSend.Fault_StallRotor.FaultSendFlag))
                {
                    Duty_DiagInfor(T_HIGH_StallOrOc,T_LOW_StallOrOc,PWM_OUT_PORT,PWM_OUT_PIN,&Diag_Infor);
                }
                
            break;
                
            case OverTemperature:
                
				if(Flag_AllFaultSend.Fault_OverTemperature.FaultSendFlag)
				{
                	Duty_DiagInfor(T_HIGH_OT,T_LOW_OT,PWM_OUT_PORT,PWM_OUT_PIN,&Diag_Infor);
				}
				
            break;
                
            case LossSpeed:
				
                if(Flag_AllFaultSend.Fault_LossSpeed.FaultSendFlag)
                {
                    Duty_DiagInfor(T_HIGH_LossSpeed,T_LOW_LossSpeed,PWM_OUT_PORT,PWM_OUT_PIN,&Diag_Infor);
                }
                
            break;
                
            case NoFault:
				
                Duty_DiagInfor(T_HIGH_NoFault,T_LOW_NoFault,PWM_OUT_PORT,PWM_OUT_PIN,&Diag_Infor);
                
            break;                                                        
        }
    }  
}

/**
 * @brief      Send_DiagInfor Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Duty_DiagInfor(uint16_t HighLeverTime,uint16_t LowLeverTime,PORT_ID_t port,PORT_GPIONO_t gpioNo,DIAG_INFOR_DEF *pDiag_Infor)
{
	static uint16_t HighLeverCnt;
	static uint16_t LowLeverCnt;
	
	if(HighLeverCnt <= HighLeverTime)
	{
		HighLeverCnt++;
		GPIO_WritePinOutput(port,gpioNo,GPIO_LOW);//output opposite Polarity due to triode
	}
	else
	{
		if(LowLeverCnt <= LowLeverTime)
		{
			LowLeverCnt++;
			GPIO_WritePinOutput(port,gpioNo,GPIO_HIGH);
		}
		else
		{
			HighLeverCnt = 0;
			LowLeverCnt = 0;
			pDiag_Infor->SendDoneFlag = 1;
		}
	}
}

/**
 * @brief      Send_DiagInfor Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

STATUS_CONDITION_NOFAULT Condition_Diag_NoFault(void)
{	
	static STATUS_CONDITION_NOFAULT Status_Condition = Condition_Meet;
    if((((Fault_All_Type.all & 0x7c) != 0) && ((Fault_All_Type.all & 0x03) == 0))\
		|| (Time_To.OverFreqFlag == 1))
    {
        Status_Condition = Condition_NoMeet;
        Fault_All_Recovery.Fault_Recovery.NormalRunCnt = 0;
    }
    else
    {
		if(ctrlObj.state == CTRL_State_CloseLoop2)
		{
			if(Fault_All_Recovery.Fault_Recovery.NormalRunCnt++ >= 5000)
			{
				Fault_All_Recovery.Fault_Recovery.NormalRunCnt = 0;
				Status_Condition = Condition_Meet;
			}
		}
		
		if((Fault_All_Type.all & 0x03) != 0)
		{
			Status_Condition = Condition_Meet;
		}
    }
    
    return  (Status_Condition);
}

/**
 * @brief      Send_DiagInfor Function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

STATUS_DIAG_OUTPUT Get_Status_DiagOutput(void)
{
	static uint8_t Status_DiagVoltage = 1;
	static STATUS_DIAG_OUTPUT Status_DiagOutput = Enable_DiagOutput;
	Status_DiagVoltage = Detection_DiagVoltage(adcData.Vdc_pu,&Fault_DiagVoltage);
	
	if(Status_DiagVoltage == 0)
	{
		Status_DiagOutput = Disable_DiagOutput;
	}
	else
	{
		Status_DiagOutput = Enable_DiagOutput;
	}
	
	return (Status_DiagOutput);
}