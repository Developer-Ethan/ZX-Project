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
DETECT_TIME_DEF Detect_Time[FaultSum] = {
	{0,0,0,0},\
	{0,0,0,0},\
	{0,0,0,0},\
	{0,0,0,0},\
	{0,0,0,0},\

};

FAULT_ALL_TYPE_DEF Fault_All_Type;
MC_PARAM_DEF MC_Param;

FAULT_ALL_RECOVERY_DEF Fault_All_Recovery;

FAULT_ALL_DEF Fault_All;
FLAG_ALLFAULTSEND Flag_AllFaultSend;

FAULT_DIAGVOLTAGE_DEF Fault_DiagVoltage;
NOFAULT_CONDITION_DEF NoFault_Condition;
RECOVERY_FAULT_ENABLE_DEF Recovery_Fault_Enable;

PROTECTION_PARAM_DEF Protection_Param[FaultSum] = \
{
	{VoltageDetect,OVERVOLTAGE_VALUE,OVERVOLTAGE_DETECT_TIME,UNDERVOLTAGE_VALUE,UNDERVOLTAGE_DETECT_TIME,\
		OVREC_VALUE,OVREC_DETECT_TIME,UVREC_VALUE,UVREC_DETECT_TIME},\
	{CurrentDetect,0,0,0,0,0,0,0,0},\
	{TemperatureDetect,0,0,0,0,0,0,0,0},\
	{StallRotorDetect,0,0,0,0,0,0,0,0},\
	{DryRotorDetect,0,0,0,0,0,0,0,0},\
};

/**
 * @brief      Recovery_Fault function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Recovery_Fault(FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_RECOVERY_DEF *pFaultRecovery,FLAG_FAULTSEND *pFlag_FaultSend,uint16_t FaultType)
{
	if(pFault_All_Type->all & FaultType)
	{
        pFaultRecovery->NormalRunCnt = 0;
        if(pFaultRecovery->ProtectRecCnt == 0)
        {
            if(pFaultRecovery->ProtectRecRelay1++ >= RECOVERY_TIME_1st)  //~one time reovery
            {
                pFaultRecovery->ProtectRecRelay1 = 0;
                pFaultRecovery->ProtectRecCnt++;
                pFault_All_Type->all = pFault_All_Type->all & (~FaultType);
                ctrlObj.state = CTRL_State_Idle;
            }
        }
        else if(pFaultRecovery->ProtectRecCnt == 1) 
        {
            if(pFaultRecovery->ProtectRecRelay2++ >= RECOVERY_TIME_2nd)
            {
                pFaultRecovery->ProtectRecRelay2 = 0;
                pFaultRecovery->ProtectRecCnt++;//~ 2 time reovery
                pFault_All_Type->all = pFault_All_Type->all & (~FaultType);
                ctrlObj.state = CTRL_State_Idle;
            }
        }
        else if(pFaultRecovery->ProtectRecCnt <= 9) //~ 3 time reovery
        {
			if((pFaultRecovery->ProtectRecCnt == 3) && \
				((pFault_All_Type->bit.DryRotor == 1) ||\
				 (pFault_All_Type->bit.LossSpeedRotor == 1) ||\
				 (pFault_All_Type->bit.StallRotor == 1)))
				{
					pFlag_FaultSend->FaultSendFlag = 1;
				}
			
            if(pFaultRecovery->ProtectRecRelay3++ >= RECOVERY_TIME_3rd_10th)
            {
                pFaultRecovery->ProtectRecRelay3 = 0;
                pFaultRecovery->ProtectRecCnt++;
                pFault_All_Type->all = pFault_All_Type->all & (~FaultType);
                ctrlObj.state = CTRL_State_Idle;
            }
        }
        else if(pFaultRecovery->ProtectRecCnt <= 19)//~ 20 time reovery
        {
            if(pFaultRecovery->ProtectRecRelay4++ >= RECOVERY_TIME_11th_20th)
            {
                pFaultRecovery->ProtectRecRelay4 = 0;
                pFaultRecovery->ProtectRecCnt++;
                pFault_All_Type->all = pFault_All_Type->all & (~FaultType);
                ctrlObj.state = CTRL_State_Idle;
            }
        }
        else
        {
            ctrlObj.state = CTRL_State_Error;
            ctrlObj.startMotor = 0;
            
            if(Speed_Ctrl.Duty == 0)
            {
                if(pFaultRecovery->ProtectRecRelay4++ >= 4000)
                {
                    pFaultRecovery->ProtectRecRelay4 = 0;
                    ctrlObj.state = CTRL_State_Idle;
                    pFault_All_Type->all = pFault_All_Type->all & (~FaultType);
                }
            }
            else
            {
                pFaultRecovery->ProtectRecRelay4 = 0;
            }   
        }
    }
}

/**
 * @brief      Detection_Fault function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */ 

void Detection_Fault(PROTECTION_PARAM_DEF *pDet,uint16_t SampleValue,FAULT_ALL_TYPE_DEF *Fault_All_Type,DETECT_TIME_DEF *pTimer)
{	
	/*OverProtect_1st logic*/
	if(SampleValue >= pDet->OvProtectTH)
	{
		if(pTimer->OvProtectCnt++ >= pDet->OvProtectTimeTH)
		{
			pTimer->OvProtectCnt = 0;
			Hal_PwmOutputDis();
			ctrlObj.state = CTRL_State_Error;
			Fault_All_Type->all = 1 << 0;
		}	
	}
	else
	{
		if(pTimer->OvProtectCnt > 0)
		{
			pTimer->OvProtectCnt--;
		}
	}
	/*UnderProtect_1st logic*/
	if(SampleValue <= pDet->UdProtectTH)
	{
		if(pTimer->UdProtectCnt++ >= pDet->UdProtectTimeTH)
		{
			pTimer->UdProtectCnt = 0;
			Hal_PwmOutputDis();
			ctrlObj.state = CTRL_State_Error;
			Fault_All_Type->all = 1 << 1;
		}
	}
	else
	{
		if(pTimer->UdProtectCnt > 0)
		{
			pTimer->UdProtectCnt--;
		}
	}
	
	if(Recovery_Fault_Enable.Recovery_VoltFault_Enable == 1)
	{
		/*Recovery logic*/
		if(Fault_All_Type->all & 0x0001)//OverVoltage Recovery
		{
			if((SampleValue <= pDet->OvRecTH))
			{
				if(pTimer->OvRecCnt++ >= pDet->OvRecTimeTH)
				{
					pTimer->OvRecCnt = 0;
					ctrlObj.state = CTRL_State_Powerup;
					Fault_All_Type->all &= 0xfffe;
				}
			}
			else
			{
				if(pTimer->OvRecCnt > 0)
				{
					pTimer->OvRecCnt--;
				}
			}
		}
		
		if(Fault_All_Type->all & 0x0002)//UnderVoltage Recovery
		{
			if(SampleValue > pDet->UdRecTH)
			{
				if(pTimer->UdRecCnt++ >= pDet->UdRecTimeTH)
				{
					pTimer->UdRecCnt = 0;
					ctrlObj.state = CTRL_State_Powerup;
					Fault_All_Type->all &= 0xfffc;
				}
			}
			else
			{
				if(pTimer->UdRecCnt > 0)
				{
					pTimer->UdRecCnt--;
				}
			}
		}
	}
}

/**
 * @brief      Detection_StallRotor function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */ 

void Detection_StallRotor(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_STALL_ROTOR_DEF *pFault)
{
	static q15 Coeff_EmfLPF = 30800;
	static q15 Esti_Emf_Square = 0;
	static q15 Esti_Emf_Square_LPF = 0;
	
	Esti_Emf_Square = (estObj.Valpha * estObj.Valpha + estObj.Vbeta*estObj.Vbeta) >> 15U;
	
	Esti_Emf_Square_LPF = (Esti_Emf_Square * (32767 - Coeff_EmfLPF) + Esti_Emf_Square_LPF * Coeff_EmfLPF) >> 15U;
    
	if (ctrlObj.state == CTRL_State_CloseLoop2)
    {
		if(pFault->DetectDelayCnt < STALLROTOR_DETECTDELAY_TIME)
		{
			pFault->DetectDelayCnt++;
		}
		else
		{
			if(SampleValue < MOTOR_STALLROTOR_SPEED_MIN)
			{
				if(pFault->DetectCnt++ >= STALLROTOR_DETECT_TIME)
				{
					pFault->DetectCnt = 0;
					ctrlObj.state = CTRL_State_Error;
					pFault_All_Type->bit.StallRotor = 1;
					pFault->StallMethod = 1;
				}	
			}
			else
			{
				if(pFault->DetectCnt > 0)
				{
					pFault->DetectCnt--;
				}
			}
			
			if(Esti_Emf_Square_LPF < (q15)(ESTI_EMF_SQUARE_MIN / 2))
			{
				if(pFault->DetectCnt1++ >= STALLROTOR_DETECT_TIME)
				{
					pFault->DetectCnt1 = 0;
					ctrlObj.state = CTRL_State_Error;
					pFault_All_Type->bit.StallRotor = 1;
					pFault->StallMethod = 2;
				}			
			}
			else
			{
				if(pFault->DetectCnt1 > 0)
				{
					pFault->DetectCnt1--;
				}
			}
		}
    }
	
	if(Recovery_Fault_Enable.Recovery_StallFault_Enable == 1)
	{
		Recovery_Fault(&Fault_All_Type,&Fault_All_Recovery.Fault_Recovery_StallRotor,&Flag_AllFaultSend.Fault_StallRotor,STALLROTOR_TYPE);
	}
}

/**
 * @brief      Detection_DryRotor function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Detection_DryRotor(q15 SampleValue1,q15 SampleValue2,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_DRY_ROTOR_DEF *pFault)
{	
	if (ctrlObj.state == CTRL_State_CloseLoop2)
	{
		if(pFault->DetectDelayCnt < DRYROTOR_DETECTDELAY_TIME)
		{
			pFault->DetectDelayCnt++;
		}
		else
		{
			if((SampleValue1 < MOTOR_DRYROTOR_BUSCURRENT) && (SampleValue2 >= MOTOR_DRYROTOR_SPEED_MIN))
			{
				if(pFault->DetectCnt++ >= DRYROTOR_DETECT_TIME)
				{
					pFault->DetectCnt = 0;
					ctrlObj.state = CTRL_State_Error;
					pFault_All_Type->bit.DryRotor = 1;
				}	
			}
			else
			{
				if(pFault->DetectCnt > 0)
				{
					pFault->DetectCnt--;
				}
			}
		}
	}

	if(Recovery_Fault_Enable.Recovery_DryRotorFault_Enable == 1)
	{
		Recovery_Fault(&Fault_All_Type,&Fault_All_Recovery.Fault_Recovery_DryRotor,&Flag_AllFaultSend.Fault_DryRotor,DRYROTOR_TYPE);
	}
}

/**
 * @brief      Detection_OverCurrent function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Detection_OverCurrent(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_OVERCURRENT_DEF *pFault)
{
	if(SWOc_ENABLE)
	{
		if (ctrlObj.state == CTRL_State_CloseLoop2)
		{
			if(SampleValue > OVERCURRENT_VALUE2)
			{
				pFault->DetectCnt_OcReduceLoad = 0;
				pFault->DetectCnt_OcRec = 0;
				if(pFault->DetectCnt_Oc++ >= OVERCURRENT_DETECT_TIME)
				{
					pFault->DetectCnt_Oc = 0;
					ctrlObj.state = CTRL_State_Error;
					pFault_All_Type->bit.Oc = 1;
				}
			
			}
			else if(SampleValue > OVERCURRENT_VALUE1)
			{
				pFault->DetectCnt_Oc = 0;
				pFault->DetectCnt_OcRec = 0;
				if(pFault->DetectCnt_OcReduceLoad++ >= OVERCURRENT_DETECT_TIME)
				{
					pFault->DetectCnt_OcReduceLoad = 0;
					pFault->OcReduceLoadFlag = 1;
				}	
			}
			else
			{
				pFault->DetectCnt_Oc = 0;
				pFault->DetectCnt_OcReduceLoad = 0;
				if(pFault->DetectCnt_OcRec++ >= OVERCURRENT_REC_DETECT_TIME)
				{
					pFault->DetectCnt_OcRec = 0;
					pFault->OcReduceLoadFlag = 0;
				}
			}
		}
	}
	
	if(Recovery_Fault_Enable.Recovery_CurrFault_Enable == 1)
	{		
		Recovery_Fault(&Fault_All_Type,&Fault_All_Recovery.Fault_Recovery_OverCurrentRotor,&Flag_AllFaultSend.Fault_OverCurrent,OVERCURRENT_TYPE);
	}
	
	if(pFault_All_Type->bit.Oc == 1)
	{
		pFault->OcClrDelayTime = 0;
		if(pFault->OcDelayTime++ >= 500)
		{
			pFault->OcDelayTime = 0;
			Flag_AllFaultSend.Fault_OverCurrent.FaultSendFlag = 1;
		}
	}
}

/**
 * @brief      Detection_Temperature function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Detection_Temperature(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_OVERTEMPERATURE_DEF *pFault)
{
	if(SampleValue < OVERTEMPERATURE_VALUE)
	{
		if(pFault->DetectCnt_Ot++ >= OVERTEMPERATURE_DETECT_TIME)
		{
			pFault->DetectCnt_Ot = 0;
			ctrlObj.state = CTRL_State_Error;
			pFault_All_Type->bit.Ot = 1;
			pFault->OtReduceLoadFlag = 0;
		}
	}
	else if(SampleValue < LIMITTEMPERATURE_VALUE)
	{
		pFault->DetectCnt_Ot = 0;
		pFault->DetectCnt_LimtRec = 0;
		if(pFault->DetectCnt_Limt++ >= LIMITTEMPERATURE_DETECT_TIME)
		{
			pFault->DetectCnt_Limt = 0;
			pFault->OtReduceLoadFlag = 1;
		}
	}
	else if(SampleValue > RECOVERTMPERATURE_VALUE)
	{
		pFault->DetectCnt_Ot = 0;
		pFault->DetectCnt_Limt = 0;
		if(pFault->DetectCnt_LimtRec++ >= LIMTREC_DETECT_TIME)
		{
			pFault->DetectCnt_LimtRec = 0;
			pFault->OtReduceLoadFlag = 0;
		}
	}
	
	if(Recovery_Fault_Enable.Recovery_TempFault_Enable == 1)
	{
		if(pFault_All_Type->bit.Ot == 1)
		{
			if(SampleValue > RESTARTTEMPERATURE_VALUE)
			{
				if(pFault->DetectCnt_OtRec++ >= OTREC_DETECT_TIME)
				{
					pFault->DetectCnt_OtRec = 0;
					pFault_All_Type->bit.Ot = 0;
					ctrlObj.state = CTRL_State_Powerup;
				}	
			}
			else
			{
				if(pFault->DetectCnt_OtRec > 0)
				{
					pFault->DetectCnt_OtRec--;
				}
			}
		}
	}
	
	if(pFault_All_Type->bit.Ot == 1)
	{
		pFault->OcClrDelayTime = 0;
		if(pFault->OcDelayTime++ >= 1000)
		{
			pFault->OcDelayTime = 0;
			Flag_AllFaultSend.Fault_OverTemperature.FaultSendFlag = 1;
		}
	}
}

/**
 * @brief      Detection_LossSpeed function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Detection_LossSpeed(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_LOSSSPEED_DEF *pFault)
{
    if(ctrlObj.state == CTRL_State_CloseLoop2)
    {
        if(SampleValue < LOSSSPEED_VALUE)
        {
            if(pFault->DetectCnt++ >= LOSSSPEED_DETECT_TIME)
            {
                pFault->DetectCnt = 0;
                pFault_All_Type->bit.LossSpeedRotor = 1;
                ctrlObj.state = CTRL_State_Error;
            }
        }
        else
        {
            if(pFault->DetectCnt > 0)
            {
                pFault->DetectCnt--;
            }
        }
    }
	
	if(Recovery_Fault_Enable.Recovert_LossSpdFault_Enable == 1)
	{
		Recovery_Fault(&Fault_All_Type,&Fault_All_Recovery.Fault_Recovery_LossSpeedRotor,&Flag_AllFaultSend.Fault_LossSpeed,LOSSSPEED_TYPE);
	}
}

/**
 * @brief      Detection_LossPhase function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
q24 Diff_Iab,Diff_Iac,Diff_Ibc;
void Detection_LossPhase(OneShunt_ObjType *pSample,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_LOSSPHASE_DEF *pFault)
{	
	if(ctrlObj.state == CTRL_State_CloseLoop2)
	{
		if(pFault->DetectDelay < DELAY_TIME_LP)
		{
			pFault->DetectDelay++;
		}
		else
		{
			if(pFault->DetectCnt < DETECT_TIME_LP)
			{
				pFault->DetectCnt++;
			}
			else
			{
				pFault->DetectCnt = 0;
				
				Diff_Iab = ZxMath_Abs(pSample->Ia_Sum - pSample->Ib_Sum);
				Diff_Iac = ZxMath_Abs(pSample->Ia_Sum - pSample->Ic_Sum);
				Diff_Ibc = ZxMath_Abs(pSample->Ib_Sum - pSample->Ic_Sum);
				
				if((Diff_Iab > THRESHOLD_LP)\
				|| (Diff_Iac > THRESHOLD_LP)\
				|| (Diff_Ibc > THRESHOLD_LP))
				{
					//pFault_All_Type->bit.LossPhase = 1;
					//ctrlObj.state = CTRL_State_Error;			
				}
				pSample->Ia_Sum = 0;
				pSample->Ib_Sum = 0;
				pSample->Ic_Sum = 0;
			}
		}
	}
}
/**
 * @brief      Detection_DiagVoltage function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

uint8_t Detection_DiagVoltage(uint16_t SampleValue,FAULT_DIAGVOLTAGE_DEF *pFault)
{
	static uint8_t DiagEnableFlag = 1;
	
	if((SampleValue < DIAGVOLTAGE_MIN) || ((SampleValue >= DIAGVOLTAGE_MAX) && (SampleValue <= DIAGVOLTAGE_HALT)))
	{
		pFault->DetectCnt1= 0;
		pFault->DetectCnt2= 0;
		if(pFault->DetectCnt++ >= DIAGVOLTAGE_MAX_TIME)
		{
			pFault->DetectCnt = 0;
            pFault->FailureFlag_2nd = 0;
			pFault->FailureFlag_1st = 1;	
		}
	}
	else if(SampleValue > DIAGVOLTAGE_HALT)
	{
		pFault->DetectCnt= 0;
		pFault->DetectCnt2= 0;
		if(pFault->DetectCnt1++ >= DIAGVOLTAGE_HALT_TIME)
		{
			pFault->DetectCnt1 = 0;
			pFault->FailureFlag_1st = 0;
			pFault->FailureFlag_2nd = 1;
		}
	}
	else
	{
		pFault->DetectCnt = 0;
		pFault->DetectCnt1 = 0;
		if(pFault->DetectCnt2++ >= REC_OVER_VOLTAG_TIME)
		{
			pFault->DetectCnt2 = 0;
			DiagEnableFlag = 1;
			pFault->FailureFlag_1st = 0;
			pFault->FailureFlag_2nd = 0;
		}
	}
	
	/* diagnose voltage invalidation logic*/    
    
    if(pFault->FailureFlag_1st == 1)
    {
        if(Diag_Infor.SendDoneFlag == 1)
        {
            pFault->FailureFlag_1st = 0;
            Diag_Infor.SendDoneFlag = 0;
            DiagEnableFlag = 0;
        }
    }
    else if(pFault->FailureFlag_2nd == 1)
    {
        pFault->FailureFlag_2nd = 0;
        DiagEnableFlag = 0;
    }
	return (DiagEnableFlag);
}

/**
 * @brief      Detection_FundamentalFault function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void RecoveryFault_Enable(RECOVERY_FAULT_ENABLE_DEF *pRecov_Fault_Enable)
{
	if(RECOVERY_FAULT_ENABLE == 1)
	{
		pRecov_Fault_Enable->Recovert_LossSpdFault_Enable = 1;
		pRecov_Fault_Enable->Recovery_CurrFault_Enable = 1;
		pRecov_Fault_Enable->Recovery_DryRotorFault_Enable = 1;
		pRecov_Fault_Enable->Recovery_StallFault_Enable = 1;
		pRecov_Fault_Enable->Recovery_TempFault_Enable = 1;
		pRecov_Fault_Enable->Recovery_VoltFault_Enable = 1;
	}
}
/**
 * @brief      Detection_FundamentalFault function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */

void Detection_FundamentalFault(void)
{
	static uint32_t DetectCnt_NormalRun = 0;
	
	if (ctrlObj.state == CTRL_State_CloseLoop2)
	{
		if(DetectCnt_NormalRun++ > 20000) // meet 5 times fault out(max cycle about 3.5s)
		{
			DetectCnt_NormalRun = 0;
			
			memset(&Fault_All_Recovery,0,sizeof(FAULT_ALL_TYPE_DEF));
			memset(&Flag_AllFaultSend,0,sizeof(FLAG_ALLFAULTSEND));
		}
	}
	else
	{
		DetectCnt_NormalRun = 0;
	}
	
	if(FAULT_DETECT_ENABLE == 1)
	{
		if(VOLTAGE_DETECT_ENABLE == 1)
		{
			Detection_Fault(&Protection_Param[VoltageDetect],adcData.Vdc_pu,&Fault_All_Type,&Detect_Time[VoltageDetect]);
		}
		if(DRYROTOR_DETECT_ENABLE == 1)
		{
			Detection_DryRotor(HWSample.BusCurrent,ElecFreq_LPF,&Fault_All_Type,&Fault_All.Fault_DryRotor);
		}
		if(STALLROTOR_DETECT_ENABLE == 1)
		{
			Detection_StallRotor(ElecFreq_LPF,&Fault_All_Type,&Fault_All.Fault_StallRotor);
		}
		if(BUSCURRENT_DETECT_ENABLE == 1)
		{
			Detection_OverCurrent(HWSample.BusCurrent,&Fault_All_Type,&Fault_All.Fault_OverCurrent);
		}
		if(TEMPERATURE_DETECT_ENABLE == 1)
		{
			Detection_Temperature(HWSample.TemperSample_LPF,&Fault_All_Type,&Fault_All.Fault_OverTemperature);
		}
		if(LOSSSPEED_DETECT_ENABLE == 1)
		{
			Detection_LossSpeed(ElecFreq_LPF,&Fault_All_Type,&Fault_All.Fault_LossSpeed);
		}
		if(LOSSPHASE_DETECT_ENABLE == 1)
		{
			Detection_LossPhase(&OneShuntObj,&Fault_All_Type,&Fault_All.Fault_LossPhase);
		}
	}
	
	RecoveryFault_Enable(&Recovery_Fault_Enable);
}