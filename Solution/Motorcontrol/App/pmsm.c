/**************************************************************************************************/
/**
 * @file      : pmsm.c
 * @brief     : pmsm demo main file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Global_Structure.h"
void On_Off_test(void);

q15 voltageValue;
int32_t voltageFactor = 0;
q15 currentFactor = 0;
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup PMSM 
 *  @brief PMSM driver modules
 *  @{
 */
/** @defgroup PMSM_Public_Functions
 *  @{
 */
/**
 * @brief      Main function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void main(void)
{
    uint8_t McuFaultResult;
    HAL_setupClock();
    disable_all_interrupt();
    
    /* Z20A8300A Init */
    while (0U == Z20A8300A_Init())
    {
    }
    
    HAL_setupPwms();
    Hal_SetupTmu();
    HAL_setupStim();
    HAL_setupHwdiv();
    Hal_SetupNvic();
	//Init_LIN();
    McuFaultResult = Hal_SetupAdcs();
	ADC_InstallCallBackFunc(ADC0_ID,ADC_FWM_INT,Pmsm_IRQHandleCurrentLoopIsr);
    HAL_setAdcUserScale((float)USER_IQ_FULL_SCALE_CURRENT_A,
        (float)USER_IQ_FULL_SCALE_VOLTAGE_V);
    // Error initial
    ERR_init(&g_errObj);
    
    // FOC control
    Foc_ParamsType focParams;
    focParams.VsMaxScale = USER_MAX_VS_MAG;
    focParams.VsLimitSacle = USER_VS_SF;    
    focParams.VdMaxScale = USER_VD_SF;
    focParams.Rs_Ohm = USER_MOTOR_Rs;
    focParams.Ls_mH = (USER_MOTOR_Ls_d + USER_MOTOR_Ls_q) / 2.0f;
    focParams.runFreq_kHz = CURRENT_LOOP_FREQ_kHz;
    focParams.VoltageBase_V = USER_IQ_FULL_SCALE_VOLTAGE_V;
    focParams.fBase_Hz = USER_IQ_FULL_SCALE_FREQ_Hz;
    focParams.ratedVdc_V = NORMAL_VDC_V;
    focParams.IBase_A = USER_IQ_FULL_SCALE_CURRENT_A;
    focParams.PwmLimitScale = PWM_LIMIT_SCALE;
    focParams.PwmMod = PWM_MOD_COUNTER;
    focParams.PwmCycle = 1000.0f/PWM_FREQ_kHz;
    focParams.MinSampeWidthTime = HAL_SAMPE_WIDTH_TIME_US;
    focParams.SampeDelayTime = HAL_SAMPE_DELAY_TIME_US;  
    focParams.GainKp = FOC_KP_GAIN;
    focParams.GainKi = FOC_KI_GAIN;
    focParams.StdQ15Max = Q15_STDMAX;
    Foc_Init(&focObj, &focParams);

    // Torque&Speed control
    TC_Params tcParams;
    tcParams.polePairs = USER_MOTOR_NUM_POLE_PAIRS;
    tcParams.runFreq_kHz = 1.0 / SLOW_TIMER_MS;
    tcParams.fBase_Hz = USER_IQ_FULL_SCALE_FREQ_Hz;
    tcParams.IBase_A = USER_IQ_FULL_SCALE_CURRENT_A;
    tcParams.inertia_kgm2 = USER_SYSTEM_INERTIA;
    tcParams.rotorFlux_Wb = USER_MOTOR_ROTOR_FLUX;
    tcParams.maxCurrent_A = USER_MOTOR_MAX_CURRENT;
    TC_init(&tcObj, &tcParams);

    // Estimation control
    Est_ParamsType estParams;
    estParams.RunFreqKHz = CURRENT_LOOP_FREQ_kHz;
    estParams.BaseFreqHz = USER_IQ_FULL_SCALE_FREQ_Hz;
    estParams.RsOhm = USER_MOTOR_Rs;
    estParams.LdmH = USER_MOTOR_Ls_d;
    estParams.LqmH = USER_MOTOR_Ls_q;
    estParams.BaseCurrentA = USER_IQ_FULL_SCALE_CURRENT_A;
    estParams.BaseVoltageV = USER_IQ_FULL_SCALE_VOLTAGE_V; 
    estParams.BaseFluxWb = USER_IQ_FULL_SCALE_FLUX_WB;
    estParams.GainAngleKi = USER_KI_ANGLE;
#if (EST == SMO)    
    estParams.GainKp = EST_SMO_KP_GAIN;
    estParams.GainKi = EST_SMO_KI_GAIN;
    estParams.MaxNegIdA = USER_MAX_NEGATIVE_ID_CURRENT_A;
    estParams.MaxCurrentA = USER_MOTOR_MAX_CURRENT;    
#else
    estParams.GainKp = EST_FSO_KP_GAIN;
    estParams.GainKi = EST_FSO_KI_GAIN; 
#endif
    estParams.StdQ15Max = Q15_STDMAX; 
    estParams.StdQ15Min = Q15_STDMIN;
    estParams.PllOutMax = USER_MAX_PLLOUT_PU;
    estParams.PllOutMin = USER_MIN_PLLOUT_PU; 
    estParams.FluxFilterCoeff = COEFF_FLUX_FILTER;
    estParams.SpdFilterCoeff = COEFF_SPD_FILTER;     
    estParams.MaxCompAngle = 1000; //910 400
    estParams.MinCompAngle = 500;//450 100
    estParams.MaxCompFreq = 8192;
    estParams.MinCompFreq = 2577;
    
    estParams.FagStep = OPENLOOPSTEP_FREQ;
    estParams.FagTarget = OPENLOOPSPEED_TARGET;    
    Est_Init(&estObj, &estParams);

    // Status initial
    
    CTRL_ParamsType ctrlParams;
    ctrlParams.scmBaseAddr = SCM_BASE_ADDR;
    ctrlParams.steadytime = STEADY_TIME_MS;
    ctrlParams.rotorAlignCurrentA = Align_CURRENT_PU;
    ctrlParams.alignCurrentStep = ROTORALIGN_CURRENTRAMP_STEP;
    ctrlParams.IFCurrentStep = IF_CURRENTRAMP_STEP;
    ctrlParams.calitime = CALIBRATION_TIME_MS;
    ctrlParams.biasThreshold = ZERO_CURRENT_THRESHOLD;
    ctrlParams.rotorAlignTimeMs = ROTOR_ALIGN_TIME_MS;
    ctrlParams.openloopMaxtimeMs = MAX_OPENLOOP_TIME_MS;
    ctrlParams.loop1TimeMs = LOOP1_TIME_MS;
    ctrlParams.cntThreshold = ANGLE_ERROTHRESHOLD_COUNT;
    ctrlParams.LFreqThresholdHz = SPEED_LWTHRESHOLD;
    ctrlParams.finalFreqHz = FINAL_SPEED_PU;

    CTRL_init(&ctrlObj , &ctrlParams);

    // DC bus voltage filter initial
    IIR_FO_init(&Pmsm_VdcFilter, ZxMath_F32toQ15(USER_DCBUS_POLE_rps / CURRENT_LOOP_FREQ_kHz / 1000));
	IIR_FO_init(&Pmsm_TemperFilter, ZxMath_F32toQ15(USER_DCBUS_POLE_rps / CURRENT_LOOP_FREQ_kHz / 1000));
	IIR_FO_init(&Pmsm_BusCurrentFilter, ZxMath_F32toQ15(USER_DCBUS_POLE_rps / CURRENT_LOOP_FREQ_kHz / 1000));
	TIM_InstallCallBackFunc(TIM2_ID, TIM_INT_TO, TIM_ToIsr); 
    Hal_SetupInterrupts(Pmsm_IRQHandleBkinIsr, EventHandle_1ms);
		
    if (McuFaultResult ==1)
    {
     g_errObj.faultCode = ERR_MCU_Fault;
    }
    // Enable global interrupts
    enable_all_interrupt();

    for (;;)
    {
        // Apply control
        if (ctrlObj.state < CTRL_State_Idle)
        {
            ctrlObj.Ia_pu = OneShuntObj.Ia_pu;
            ctrlObj.Ib_pu = OneShuntObj.Ib_pu;
            ctrlObj.Ic_pu = OneShuntObj.Ic_pu;
        }

        voltageValue = AdcRawResults[0];
        adcData.Vdc_pu = ZxMath_MpyQ15((voltageValue << 3), voltageFactor);
        IIR_FO_pushInput(&Pmsm_VdcFilter, adcData.Vdc_pu);
        focObj.RealVdc = Pmsm_VdcFilter.output;
		
		HWSample.TemperSample = AdcRawResults[3];
		HWSample.TemperSample = HWSample.TemperSample << 3;
		IIR_FO_pushInput(&Pmsm_TemperFilter,HWSample.TemperSample);
		HWSample.TemperSample_LPF = Pmsm_TemperFilter.output;
		
        if (g_errObj.faultCode != 0)
        {
            ctrlObj.enablePwm = false;
        }
        CTRL_updateState(&ctrlObj);
		//ctrlObj.freqRef = 15000;		
    }
}

uint8_t On_OffEnable = 0;
void On_Off_test(void)
{
	static uint16_t Cnt = 0;
	if(On_OffEnable == 1)
	{
		if(Cnt++ >= 3000)
		{
			Cnt = 0;
			if(ctrlObj.startMotor == 0)
			{
				ctrlObj.startMotor = 1;
			}
			else if(ctrlObj.startMotor == 1)
			{
				ctrlObj.startMotor = 0;
			}		
		}
	}
}

/**
 * @brief      Currentloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
__ramfunc void Pmsm_IRQHandleCurrentLoopIsr(void)
{	 
	uint16_t i = 0;
	uint32_t Read_Fcount;
	
	adc_reg_t * ADCx = (adc_reg_t *)(ADC0_BASE_ADDR);
    
    Read_Fcount = ADCx->ADC_FCTRL.FCOUNT; 
	
    if((VALUE_WM + 1u) == Read_Fcount)
    {
      for(i = 0;i < (VALUE_WM + 1);i++)
      {
        IRQn_AdcMapData[i] = ADCx->ADC_DATA_RD.ADC_DATA_RD;
      }
    }
    /* initial current offset value */
    if(ctrlObj.needCalibI)
    {
        Hal_InitialCurrentOffset(&adcData);
    }  
    Hal_ReadAdcData(&adcData);
    OneShunt_CalcSampeCurrent(&OneShuntObj, adcData.Current1_pu, adcData.Current2_pu, focObj.Sector); 
	
	OneShuntObj.Ia_Sum += ZxMath_Abs(OneShuntObj.Ia_pu);
	OneShuntObj.Ib_Sum += ZxMath_Abs(OneShuntObj.Ib_pu);
	OneShuntObj.Ic_Sum += ZxMath_Abs(OneShuntObj.Ic_pu);
		
	estObj.Valpha = focObj.Valpha;
    estObj.Vbeta = focObj.Vbeta;
	
	estObj.Ia_pu = (OneShuntObj.Ia_pu);
    estObj.Ib_pu = (OneShuntObj.Ib_pu);
    estObj.Ic_pu = (OneShuntObj.Ic_pu);
#if (EST == SMO) 
    Est_SmoRun(&estObj);
#else
    Est_FluxRun(&estObj);
#endif    
    
    if (ctrlObj.enablePwm)
    {   
        switch (ctrlObj.state)
        {  
        case CTRL_State_RotorAlign:
            focObj.Theta = ROTOR_ALIGN_ANGLE;
            break;
        case CTRL_State_Openloop:
            Est_FakeAngle(&estObj);
            focObj.Theta = estObj.FagTheta;
            ctrlObj.OPangle_pu = estObj.FagTheta;
            ctrlObj.freqfb = estObj.FakeFreqOutput;
            ctrlObj.estangle_pu = estObj.EstTheta;
            break;
        default:
            if (ctrlObj.state > CTRL_State_Openloop)
            {
                focObj.Theta = estObj.EstTheta;
            }
            break;
        }
		
        focObj.Ialpha = estObj.Ialpha;
        focObj.Ibeta = estObj.Ibeta;
				
        // FOC process
        Foc_Run(&focObj);
        Hal_UpdateAsymmetricOutput(&focObj.FocCv);
        Hal_UpdateTdgDelayOut(focObj.OffsetTime1,focObj.OffsetTime2);
        Hal_PwmOutputEn();
        if(ctrlObj.state == CTRL_State_Openloop)
        {
            estObj.ThetaIndex = estObj.EstTheta >> 5u;
            estObj.SinTheta = ZxMath_SineTableQ15[estObj.ThetaIndex];
            estObj.CosTheta = ZxMath_cosTableQ15Ptr[estObj.ThetaIndex];  
        }
        else
        {
            estObj.SinTheta = focObj.SinTheta;
            estObj.CosTheta = focObj.CosTheta;
        }
    }
    else 
    {
        Foc_ResetParams(&focObj);
        Hal_UpdateAsymmetricOutput(&focObj.FocCv);
        Hal_UpdateTdgDelayOut(focObj.OffsetTime1,focObj.OffsetTime2);
        Hal_PwmOutputDis();
    }
    
    Hal_ClearDMAINT();
}

/**
 * @brief      Faultin brake function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Pmsm_IRQHandleBkinIsr(void)
{
	tdg_reg_t * TDGx = (tdg_reg_t *)(TDG0_BASE_ADDR);

	static Z20A8300A_SpiStatusType Result_FaultRead = Z20A8300A_ERR_OK;
    static Z20A8300A_StatusRegType Status;
    static Z20A8300A_AllDiagType   Diag;
	
	TIM_StopCounter(PWM_TIM_ID);
	TIM_LoadCounterInitialVal(PWM_TIM_ID);
	
	TDGx->TDG_CNT.CNT = 0;
	
	Hal_PwmOutputDis();
	ctrlObj.enablePwm = false;
	
	//g_errObj.faultCode = ERR_Drive_fault;
	ctrlObj.state = CTRL_State_Error;
	Fault_All_Type.bit.Predriver = 1;
	Fault_All_Type.bit.Oc = 1;
	
	Result_FaultRead = Z20A8300A_Diag_ReadClearDiag(&Z20A8300AIf1,&Status,&Diag);
	(void)(Result_FaultRead);
	
    TIM_IntClear(FAULT_TIM_ID, TIM_INT_FAULT);
	TIM_StartCounter(PWM_TIM_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1);
}
/**
 * @brief      Speedloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Pmsm_IRQHandleSpeedLoopIsr(void)
{
	On_Off_test();
    CTRL_tick(&ctrlObj); 
	
    switch (ctrlObj.state)
    {
    case CTRL_State_RotorAlign:
        focObj.IdRef = 0;
        RAMP_calc(&(ctrlObj.rotorAlignCurrentRamp));
        focObj.IqRef = ctrlObj.rotorAlignCurrentRamp.output;
        break;

    case CTRL_State_Openloop:
        if (ZxMath_Abs(ctrlObj.freqfb) < SPEED_LWTHRESHOLD) 
        {
            focObj.IdRef = 0;
            focObj.IqRef = STARTUP_CURRENT_PU;
        }
        else 
        {
            focObj.IdRef = 0;
            RAMP_calc(&(ctrlObj.IFCurrentRamp));
            focObj.IqRef = ctrlObj.IFCurrentRamp.output;
            if (ZxMath_Abs(ctrlObj.estangle_pu - ctrlObj.OPangle_pu) < DELTA_THETA_THRESHOLD)
            {
                ctrlObj.Cntr++;
            }
            else
            {
                ctrlObj.Cntr = 0U;
            }            
        }
        break;

    case CTRL_State_CloseLoop1:
        tcObj.speedRef = Loop_Control_Ref(Speed_Ctrl.SpeedRef,Get_BusCurrentLimit(&focObj));//ctrlObj.freqRef;
        tcObj.speed = Loop_Control_Feedback(ElecFreq_LPF,Get_CurrentRealtime(&HWSample));//estObj.ElecFreq;
        TC_run(&tcObj);
        focObj.IdRef = tcObj.IdRef;
        focObj.IqRef = tcObj.IqRef;
        break;

    case CTRL_State_CloseLoop2:
        tcObj.speedRef = Loop_Control_Ref(Speed_Ctrl.SpeedRef,Get_BusCurrentLimit(&focObj));//ctrlObj.freqRef;
        tcObj.speed = Loop_Control_Feedback(ElecFreq_LPF,Get_CurrentRealtime(&HWSample));//estObj.ElecFreq;
        TC_run(&tcObj);
        focObj.IdRef = tcObj.IdRef;
        focObj.IqRef = tcObj.IqRef;
        break;

    case CTRL_State_CloseLoop3:
        if (estObj.ElecFreq < SHUTDOWN_SPEED_THRESHOLD_PU)
        {
            focObj.IdRef = 0;
            focObj.IqRef = 0;
            Hal_PwmOutputDis();
            ctrlObj.enablePwm = false;
        }
        else
        {
            tcObj.speedRef = SHUTDOWN_SPEED_PU;
            tcObj.speed = estObj.ElecFreq;
            TC_run(&tcObj);
            focObj.IdRef = tcObj.IdRef;
            focObj.IqRef = tcObj.IqRef;
        }
        break;

    default:
        focObj.IdRef = 0;
        focObj.IqRef = 0;
        break;
    }
    STIM_ClearInt(STIM_0);
}
/**
 * @brief      Switch to closeloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void CTRL_switchToCloseLoop()
{
    tcObj.speedRef = ctrlObj.freqRef;
    tcObj.pidSpd.Ui = focObj.IqRef;
    tcObj.spdRamp.output = ((OPENLOOPSPEED_TARGET >> 1)+ SPEED_LWTHRESHOLD)>>1;
}
/**
 * @brief      Reset openloop data function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void CTRL_Resetopenloopdata()
{
    ctrlObj.freqfb = 0;
    ctrlObj.estangle_pu = 0;
    ctrlObj.OPangle_pu = 0;
    ctrlObj.Cntr = 0U;
    ctrlObj.IFCurrentRamp.target = IF_CURRENT_PU;
    ctrlObj.IFCurrentRamp.output = STARTUP_CURRENT_PU;

    focObj.IdRef = 0;
    focObj.IqRef = STARTUP_CURRENT_PU;
#if (EST == SMO)
    estObj.Eq = 0;
    estObj.SumEq = 0;
#else
    estObj.EstFilterAlphaFlux.Output = 0U;
    estObj.EstFilterBetaFlux.Output = 0U;  
#endif 
    estObj.EstPidPll.Ui = 0.0f;    
    estObj.EstFilterSpd.Output = 0.0f;    
    estObj.FinalAngle = 0;   
    estObj.RawAngle_pu = 2*ROTOR_ALIGN_ANGLE;
    estObj.FakeFreqTarget = OPENLOOPSPEED_TARGET;
    estObj.FakeFreqOutput = 0;
    estObj.Fagangle_pu = 2*ROTOR_ALIGN_ANGLE;
	
	LoopType = SpeedLoop;
	LoopCtrl.PowerLoopFlag = 0;
	LoopCtrl.SpeedLoopFlag = 1;
	
	memset(&Fault_All,0,sizeof(FAULT_ALL_DEF));
}

 /**
 *  @brief set value for communication
 */

void Pc_Speed_Required(uint16_t spd_comp)
{
    ctrlObj.freqRef = ZxMath_F32toQ15(spd_comp * USER_MOTOR_NUM_POLE_PAIRS / USER_IQ_FULL_SCALE_FREQ_Hz);
}

uint16_t for_Get_Status()
{
    uint16_t drive_status = 0;
    if (0 != ctrlObj.enablePwm && ctrlObj.startMotor)
    {
        drive_status = 0x05;
    }
    else
    {
        drive_status = 0x04;
    }
    if (0 != g_errObj.faultCode)
    {
        drive_status = drive_status + 0x18;
    }
    return drive_status;
}

uint16_t for_Get_Alarm1()
{
    return g_errObj.faultCode;
}

uint16_t for_Get_Speed()
{
    uint16_t drive_speed = 0;
    //rps
    drive_speed = (uint16_t)((zx_Q15toQ6(estObj.ElecFreq)) * USER_IQ_FULL_SCALE_FREQ_Hz / USER_MOTOR_NUM_POLE_PAIRS);
    return drive_speed;
}


//Get the dcbus voltage
uint16_t for_Get_Voltage_Bus()
{
    uint16_t drive_voltage_bus = 0;
    // not rms value
    drive_voltage_bus = (uint16_t)(Pmsm_VdcFilter.output);
    return drive_voltage_bus;
}
/** @} end of group PMSM_Public_Functions */

/** @} end of group PMSM driver modules */

/** @} end of group Z20K11XM_Foc */