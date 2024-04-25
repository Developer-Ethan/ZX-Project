/**************************************************************************************************/
/**
 * @file      : hal.c
 * @brief     : Hardware abstraction layer setup and action file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#include "Global_Structure.h"
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @defgroup HAL 
 *  @brief HAL driver modules
 *  @{
 */

/** @defgroup HAL_Global_Variables
 *  @{
 */
static uint32_t Hal_AdcMapData[5] = {0,0,0,0,0};
uint32_t IRQn_AdcMapData[5] = {0,0,0,0,0};
uint32_t AdcRawResults[5] = {0,0,0,0,0};
/** @} end of group HAL_Global_Variables */

/** @defgroup HAL_Public_Functions
 *  @{
 */
/**
 * @brief      PWM config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void HAL_setupPwms()
{
    /*TIM module enable*/
    SYSCTRL_ResetModule(SYSCTRL_TIM0);
    SYSCTRL_EnableModule(SYSCTRL_TIM0);
	
	SYSCTRL_ResetModule(SYSCTRL_TIM2);
    SYSCTRL_EnableModule(SYSCTRL_TIM2);
  
    /* TIM complementary PWM output channel 4 and channel 5 config struct :disable fault ctrl and disable ccv sync function */
    TIM_CompPwmChannelConfig_t  cPwmChConfig0 =
    {
        TIM_PAIR_CHANNEL_1,
        TIM_PWM_HIGH_TRUE_PULSE, 
        TIM_POL_HIGH,
        // 0.5*(1-a)*PWM_MAX a is the ratio of caculated ccv
        (PWM_MOD_COUNTER * 50 / 100),
        // 0.5*(1+a)*PWM_MAX a is the ratio of caculated ccv
        (PWM_MOD_COUNTER * 50 / 100),
        ENABLE,
        ENABLE,
        ENABLE
    };
    /* TIM complementary PWM output channel 6 and channel 7 config struct*/
    TIM_CompPwmChannelConfig_t  cPwmChConfig1 =
    {
        TIM_PAIR_CHANNEL_0,
        TIM_PWM_HIGH_TRUE_PULSE,
        TIM_POL_HIGH,
        (PWM_MOD_COUNTER * 50 / 100),
        (PWM_MOD_COUNTER * 50 / 100),
        ENABLE,
        ENABLE,
        ENABLE
    };
    /* TIM complementary PWM output channel 0 and channel 1 config struct*/
    TIM_CompPwmChannelConfig_t  cPwmChConfig2 =
    {
        TIM_PAIR_CHANNEL_2,
        TIM_PWM_HIGH_TRUE_PULSE,
        TIM_POL_HIGH,
        (PWM_MOD_COUNTER * 50 / 100),
        (PWM_MOD_COUNTER * 50 / 100),
        ENABLE,
        ENABLE,
        ENABLE
    };

    /* TIM complementary PWM output config array*/
    TIM_CompPwmChannelConfig_t cPwmChConfig[3] = { cPwmChConfig0,cPwmChConfig1,cPwmChConfig2 };


    /* TIM complementary PWM output Config*/
    TIM_CompPwmConfig_t  cPwmConfig =
    {
        3,                  /*pair number*/
        0,                  /*counter init value*/
        PWM_MOD_COUNTER - 1,  /*counter max value*/
        TIM_PRESCALER_4,    /*dead PRESCALER*/
        0,                  /*dead clock, dead time = TIM_PRESCALER_1 * 128 set 2us was 32 for 4910 has this funciton*/
        cPwmChConfig        /*channel config pointer*/
    };

    /*match relaod*/
    TIM_ChannelMatchConfig_t channelMatchConfig[6] =
    {
      {TIM_CHANNEL_4, DISABLE},
      {TIM_CHANNEL_5, DISABLE},
      {TIM_CHANNEL_2, DISABLE},
      {TIM_CHANNEL_3, DISABLE},
      {TIM_CHANNEL_0, DISABLE},
      {TIM_CHANNEL_1, DISABLE}
    };


    /*TIM reload config*/
    TIM_ReloadConfig_t   reloadConfig =
    {
        TIM_RELOAD_FULL_CYCLE,
        1,
        6,
        channelMatchConfig
    };

    /*TIM sync config for update ccv*/
    TIM_PwmSyncConfig_t  syncConfig =
    {
        TIM_UPDATE_PWM_SYN,
        TIM_UPDATE_PWM_SYN,
        DISABLE,
        ENABLE,
        &reloadConfig
    };

        /* GPIO config */
    PORT_PinmuxConfig(PORT_A, GPIO_2, PTA2_TIM0_CH0);    // AHI
    PORT_PinmuxConfig(PORT_C, GPIO_1, PTC1_TIM0_CH1);    // ALO

    PORT_PinmuxConfig(PORT_C, GPIO_2, PTC2_TIM0_CH2);    // BHI
    PORT_PinmuxConfig(PORT_C, GPIO_3, PTC3_TIM0_CH3);    // BLO

    PORT_PinmuxConfig(PORT_B, GPIO_4, PTB4_TIM0_CH4);    // CHI
    PORT_PinmuxConfig(PORT_B, GPIO_5, PTB5_TIM0_CH5);    // CLO
    
    /* set up-counting mode */
    TIM_CountingModeConfig(PWM_TIM_ID, TIM_COUNTING_UP);

    /* output complementary init*/
    TIM_OutputComplementaryPwmConfig(PWM_TIM_ID, &cPwmConfig);

    /* reload config*/
    TIM_SyncConfig(PWM_TIM_ID, &syncConfig);

    // Disable the pwm output
    Hal_PwmOutputDis();

    TIM_StopCounter(PWM_TIM_ID);

    /* TIM fault control config struct definition */
    const TIM_PwmFaultCtrlConfig_t  config =
    {
      ENABLE, TIM_INPUT_FILTER_15, TIM_LOW_STATE,
      TIM_Fault_MANUAL_CLEAR,{
      {DISABLE, DISABLE, TIM_POL_LOW},             /* TIM_POL_LOW: 0 is active fault0 input*/
      {ENABLE, ENABLE, TIM_POL_LOW},               /* TIM_FAULT_CHANNEL_1 */
      }
    };
    /* TIM initialise fault control config */
    TIM_FaultControlConfig(FAULT_TIM_ID, &config);
    //PORT_PinmuxConfig(PORT_E, GPIO_9, PTE9_TIM2_FLT0); 
    PORT_PinmuxConfig(Z20A8300A_DIAG_PORT, Z20A8300A_DIAG_PIN, Z20A8300A_DIAG_PINMUX);
    /* enable function */
    TIM_IntClear(FAULT_TIM_ID, TIM_INT_FAULT);
    TIM_InitTriggerCmd(PWM_TIM_ID, ENABLE);
    //TIM_IntMask(PWM_TIM_ID,TIM_INT_TO,UNMASK);

//PWM_IN about duty capure	
	PORT_PinmuxConfig(PORT_A, GPIO_12, PTA12_TIM2_CH2);
	/* input config, the TIM input channel should be in low level if it is set to capture rising edge first when TIM is in input capture mode*/
	/* TIM input capture channel config struct  initial:channelId, falling,rising, disable filter, continue capture */
	TIM_DualEdgeChannelConfig_t chconfig1 =
	{
		TIM_PAIR_CHANNEL_1, TIM_INPUT_EDGE_RISING, TIM_INPUT_EDGE_FAILING,TIM_INPUT_FILTER_DISABLED, TIM_CONTINUOUS_PULSE_CAPTURE
	};

	/* TIM input capture global config struct initial:  number 1,mod value 0xFFFF, input capture channel config struct  */ 
	const TIM_DualEdgeCaptureConfig_t  config1 = 
	{
		1, 0xFFFF,  &chconfig1
	};	
	/*input capture init*/
    TIM_DualEdgeCaptureInit(TIM2_ID, &config1);
    TIM_SetCounterInitialVal(TIM2_ID, 0x0000);
	/* counting mode: up counting mode */
	TIM_CountingModeConfig(TIM2_ID, TIM_COUNTING_UP);
	/*start counter*/  
    TIM_StartCounter(TIM2_ID, TIM_CLK_SOURCE_FUNCTION, TIM_CLK_DIVIDE_32);
	/* reload flag clear */
    TIM_IntClear(TIM2_ID, TIM_INT_RLD);
	/* clear CHF before dual edge capture */
    TIM_IntClear(TIM2_ID, TIM_INT_CH2);
    TIM_IntClear(TIM2_ID, TIM_INT_CH3);
	/* enable the interrupt */
    TIM_IntMask(TIM2_ID, TIM_INT_CH3, UNMASK);
	TIM_IntMask(TIM2_ID, TIM_INT_TO, UNMASK);

//PWM_OUT
	PORT_PinmuxConfig(PWM_OUT_PORT, PWM_OUT_PIN, PWM_OUT_PINMUX);
	GPIO_SetPinDir(PWM_OUT_PORT,PWM_OUT_PIN,GPIO_OUTPUT);
}

/**
 * @brief      Tmu config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Hal_SetupTmu(void)
{
    TMU_SetUnlockForModule(HAL_TMU_OUTPUT);
    TMU_SetSourceForModule(HAL_TMU_INPUT,HAL_TMU_OUTPUT);
    TMU_ModuleCmd(HAL_TMU_OUTPUT,ENABLE);
    TMU_SetLockForModule(HAL_TMU_OUTPUT);
}
/**
 * @brief      Stim config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void HAL_setupStim()
{
    /* Enable STIM module */
    SYSCTRL_EnableModule(SYSCTRL_STIM);
    /* STIM configuration */
    const STIM_Config_t stimConfig =
    {
    .workMode = STIM_FREE_COUNT,
    .compareValue = SLOW_TIMER_MS * 64000,    /*counter clock is 64M, compare value =64000,  period = 1ms*/
    .countResetMode = STIM_INCREASE_FROM_0,
    .clockSource = STIM_FUNCTION_CLOCK,
    };

    /* Init STIM_0*/
    STIM_Init(STIM_0, &stimConfig);
    /*Disable STIM*/
    STIM_Disable(STIM_0);
}
/**
 * @brief      Slock config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void HAL_setupClock()
{
    /* Disable wdog */
    SYSCTRL_EnableModule(SYSCTRL_WDOG);
    WDOG_Disable();

    /* Select FIRC64M as system clock*/
    /* Clock init */
    CLK_OSC40MEnable2(CLK_OSC_FREQ_MODE_HIGH, DISABLE, CLK_OSC_XTAL);
    CLK_FIRC64MEnable(ENABLE);
    while (ERR == CLK_SysClkSrc(CLK_SYS_FIRC64M));

    /* set the core&bus clock divider */
    CLK_SetClkDivider(CLK_CORE, CLK_DIV_1);
    CLK_SetClkDivider(CLK_BUS, CLK_DIV_1);
   
    /* Enable TMU module */
    SYSCTRL_EnableModule(SYSCTRL_TMU);    
    
    /* Reset DMA */
    SYSCTRL_ResetModule(SYSCTRL_DMA);
    /* Enable DMA module */
    SYSCTRL_EnableModule(SYSCTRL_DMA);
  
    /* Reset DMAMUX module */
    SYSCTRL_ResetModule(SYSCTRL_DMAMUX);    
    /* Enable DMAMUX module */
    SYSCTRL_EnableModule(SYSCTRL_DMAMUX);
    
    /* config the port clock and enable gpios */
    CLK_ModuleSrc(CLK_PORTA, CLK_SRC_FIRC64M);
    SYSCTRL_EnableModule(SYSCTRL_PORTA);
    CLK_ModuleSrc(CLK_PORTB, CLK_SRC_FIRC64M);
    SYSCTRL_EnableModule(SYSCTRL_PORTB);
    CLK_ModuleSrc(CLK_PORTC, CLK_SRC_FIRC64M);
    SYSCTRL_EnableModule(SYSCTRL_PORTC);
    CLK_ModuleSrc(CLK_PORTD, CLK_SRC_FIRC64M);
    SYSCTRL_EnableModule(SYSCTRL_PORTD);
    CLK_ModuleSrc(CLK_PORTE, CLK_SRC_FIRC64M);
    SYSCTRL_EnableModule(SYSCTRL_PORTE);
    /*Config GPIO module*/
    SYSCTRL_EnableModule(SYSCTRL_GPIO);

    /*TIM clock source select*/
    //SYSCTRL_EnableModule(SYSCTRL_TIM2);
    CLK_ModuleSrc(CLK_TIM0, CLK_SRC_FIRC64M);
    CLK_SetClkDivider(CLK_TIM0, CLK_DIV_1);
	
    CLK_ModuleSrc(CLK_TIM2, CLK_SRC_OSC40M);
    CLK_SetClkDivider(CLK_TIM2, CLK_DIV_1);

    /* Configure STIM function clock*/
    CLK_ModuleSrc(CLK_STIM, CLK_SRC_FIRC64M);

    /* Select clock source for ADC */
    CLK_ModuleSrc(CLK_ADC0, CLK_SRC_FIRC64M);
    /*set clock for ADC , make sure it is not greater than bus clock*/
    CLK_SetClkDivider(CLK_ADC0, CLK_DIV_1);

    /* Select clock source for TDG */
    CLK_ModuleSrc(CLK_TDG0, CLK_SRC_FIRC64M);
    /* set divide ratio of TDG's function clock*/
    CLK_SetClkDivider(CLK_TDG0, CLK_DIV_1);

    /* choose osc clock as function clock of uart1 */
    CLK_ModuleSrc(CLK_UART1, CLK_SRC_FIRC64M);
    /* set divide ratio of uart1's function clock*/
    CLK_SetClkDivider(CLK_UART1, CLK_DIV_1);
}
/**
 * @brief      NVIC config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Hal_SetupNvic(void)
{
    /* Enable STIM & TIM2 NVIC IRQ*/
	NVIC_SetPriority (HAL_SPEEDLOOPIRQ_ID, 2);
	NVIC_SetPriority(ADC0_IRQn,1);
    NVIC_SetPriority (HAL_FAULTINIRQ_ID, 0);
	NVIC_SetPriority (TIM2_IRQn, 3);
	
	NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_EnableIRQ(HAL_SPEEDLOOPIRQ_ID);
    NVIC_EnableIRQ(HAL_FAULTINIRQ_ID);
	NVIC_EnableIRQ(TIM2_IRQn);
}
/**
 * @brief      Interrupt config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Hal_SetupInterrupts(isr_cb_t *PwmFaultIntCbf, isr_cb_t *StimIntCbf)
{
    /* Install interrupt callback function */
    TIM_InstallCallBackFunc(FAULT_TIM_ID, TIM_INT_FAULT,PwmFaultIntCbf);  
    STIM_InstallCallBackFunc(STIM_ID,STIM_INT,StimIntCbf);
    
    // Enalbe Fault-in int  fault1 input
    TIM_IntMask(FAULT_TIM_ID, TIM_INT_FAULT, UNMASK);
    // Enable the TIM0 int
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_0, DISABLE);
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_1, DISABLE);    
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_2, DISABLE);   
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_3, DISABLE);
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_4, DISABLE);
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_5, DISABLE);
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_6, DISABLE);
    TIM_DMACtrl(TIM0_ID, TIM_CHANNEL_7, DISABLE);
    
    /*start TIM PWM*/
    TIM_StartCounter(PWM_TIM_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1);   // was TIM_CLK_SOURCE_SYSTEM 
    /*start TIM1 FAULT channel*/
    //TIM_StartCounter(FAULT_TIM_ID, TIM_CLK_SOURCE_SYSTEM, TIM_CLK_DIVIDE_1); 
    
    STIM_Enable(STIM_0);    
    // Enable STIM_0 interrupt
    STIM_IntCmd(STIM_0, ENABLE); 
}  
/**
 * @brief      Fault in enable function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void HAL_breakEnable()
{
    TIM_IntClear(PWM_TIM_ID, TIM_INT_FAULT); 
    // Enalbe Fault-in int  fault0 input
    TIM_IntMask(PWM_TIM_ID, TIM_INT_FAULT, UNMASK);     
}

/**
 * @brief      ADC Config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
uint8_t Hal_SetupAdcs(void)
{
    uint8_t TDGFaultFlag;
    ADC_Config_t SubcaseAdcCfg=
    {
        /* resolution */
        ADC_RESOLUTION_12BIT, 
        /* vref */    
        ADC_VREF_INTERNAL,  
        /* trigger mode */      
        ADC_TDG_TRIGGER,        
        /* conversion mode */  
        ADC_CONVERSION_SINGLE, 
        /* average disabled */   
        ADC_AVGS_DISABLED,     
        /* Set sample interval > 500ns */   
        50,                
    }; 
   
    ADC_ChannelConfig_t AdcChannelCfg0 = 
    {
        ADC_SINGLE_MODE,  /* Single-Ended Mode Selected */
        HAL_ADC_TEMP_CHN,     /* Single mode, channel[12] and vssa */
        ADC_N_NONE,       /* Single mode, N-channel donn't need to configure */                  
    };
    
    ADC_ChannelConfig_t AdcChannelCfg1 = 
    {
        ADC_SINGLE_MODE,  /* Single-Ended Mode Selected */
        HAL_ADC_VMOT_CHN,     /* Single mode, channel[12] and vssa */
        ADC_N_NONE,       /* Single mode, N-channel donn't need to configure */                  
    };
 
    ADC_ChannelConfig_t AdcChannelCfg2 = 
    {
        ADC_SINGLE_MODE,  /* Single-Ended Mode Selected */
        HAL_ADC_IV_CHN,       /* Single mode, channel[12] and vssa */
        ADC_N_NONE,       /* Single mode, N-channel donn't need to configure */ 
    };
	
	ADC_ChannelConfig_t AdcChannelCfg3 = 
    {
        ADC_SINGLE_MODE,  /* Single-Ended Mode Selected */
        HAL_ADC_BUSCURRENT_CHN,       /* Single mode, channel[12] and vssa */
        ADC_N_NONE,       /* Single mode, N-channel donn't need to configure */ 
    };
   
    ADC_TDGTriggerConfig_t  AdcTriggerConfig = 
    {
        /* Loop mode Selected */  
        ADC_MAPPING_MODE, 
        /* CMD0: channel 15; */          
        HAL_ADC_VMOT_CHN,       
        /* CMD1: channel 14; */      
        HAL_ADC_IV_CHN,        
        /* CMD2: channel 13; */       
        HAL_ADC_IV_CHN,     
        /* CMD3: channel 12; */           
        HAL_ADC_TEMP_CHN,         
        /* CMD4: channel 7; */          
        HAL_ADC_BUSCURRENT_CHN,   
        /* CMD5: channel 7; all CMDs can be Configured as different channels */            
        HAL_ADC_IV_CHN,                              
    };
 
    /* mod value, single, divide4, SW trig, clear to mod */
    TDG_InitConfig_t Config=
    {
        PWM_MOD_COUNTER*2U - 1, TDG_COUNT_SINGLE, TDG_CLK_DIVIDE_1, TDG_TRIG_EXTERNAL, TDG_UPDATE_IMMEDIATELY, TDG_CLEAR_MODULATOR
    };
  
    /* 001*1/64Tclock */
    TDG_DelayOutputConfig_t Doconfig =         
    {
        TDG_DO_0, 
        9, 
        ENABLE,
    };
  
    TDG_ChannelConfig_t Chconfig =
    {
        TDG_CHANNEL_0, 
        0U, 
        1U, 
        &Doconfig,
    };

    /* Reset ADC */
    SYSCTRL_ResetModule(HAL_ADC_SYSCTRL_MODULE);
    /* Enable ADC clock */
    SYSCTRL_EnableModule(HAL_ADC_SYSCTRL_MODULE);
    /* adc pinmux */ 
    /* PTB22/ADC0_CH19 for motor V_current */      
    PORT_PinmuxConfig(HAL_ADC_IV_PORT, HAL_ADC_IV_PIN, HAL_ADC_IV_PINMUX);           
    /* PTC17/ADC0_CH15 for motor bus volatage */   
    PORT_PinmuxConfig(HAL_ADC_VMOT_PORT, HAL_ADC_VMOT_PIN, HAL_ADC_VMOT_PINMUX); 
	/* PTA7/ADC0_CH3 for motor temperature */   
    PORT_PinmuxConfig(HAL_ADC_TEMP_PORT, HAL_ADC_TEMP_PIN, HAL_ADC_TEMP_PINMUX);
	/* PTA7/ADC0_CH3 for motor temperature */   
    PORT_PinmuxConfig(HAL_ADC_BUSCURRENT_PORT, HAL_ADC_BUSCURRENT_PIN, HAL_ADC_BUSCURRENT_PINMUX);
    /* Reset software */
    ADC_SoftwareReset(HAL_ADC_ID);
    /* Self calibration */
    //ADC_SelfCalibration(HAL_ADC_ID);
    /* Initialize ADC */
    ADC_Init(HAL_ADC_ID,&SubcaseAdcCfg);
    /* Redefine the depth the function */    
    ADC_FifoDepthRedefine(HAL_ADC_ID,6); 
    /* Set ADC watermark */
    /* fifo WM > 3, that is when WM=4,flag will be set */
    ADC_FifoWatermarkConfig(HAL_ADC_ID,VALUE_WM);                               
    /* Configure input channel */
    ADC_ChannelConfig(HAL_ADC_ID, &AdcChannelCfg0);
    ADC_ChannelConfig(HAL_ADC_ID, &AdcChannelCfg1);
    ADC_ChannelConfig(HAL_ADC_ID, &AdcChannelCfg2);
	ADC_ChannelConfig(HAL_ADC_ID, &AdcChannelCfg3);
    
    /* Mask FIFO watermark interrupt */
    ADC_IntMask(HAL_ADC_ID,ADC_FWM_INT, UNMASK);   
    
    /* Set trigger mode */
    ADC_TDGTriggerConfig(HAL_ADC_ID,&AdcTriggerConfig);
    /* Clear TCOMP interrupt */
    ADC_IntClear(HAL_ADC_ID,ADC_TCOMP_INT);
    /* Enable ADC module */
    ADC_Enable(HAL_ADC_ID);
    /* Enable ADC dma request */
    ADC_DmaRequestCmd(HAL_ADC_ID, DISABLE);
    
    /*Enable TDG module */
    SYSCTRL_EnableModule(HAL_TDG_SYSID);    
    /* Initialize TDG */
    TDG_InitConfig(HAL_TGD_ID, &Config);
    /* Set TDG delay output */
    TDG_ChannelDelayOutputConfig(HAL_TGD_ID, &Chconfig, ENABLE);    

    Chconfig.channelId = TDG_CHANNEL_1;
    Doconfig.offset = (uint16_t)PWM_MOD_COUNTER / 5; 
    TDG_ChannelDelayOutputConfig(HAL_TGD_ID, &Chconfig, ENABLE);

    Chconfig.channelId = TDG_CHANNEL_2;
    Doconfig.offset = (uint16_t)PWM_MOD_COUNTER / 4;
    TDG_ChannelDelayOutputConfig(HAL_TGD_ID, &Chconfig, ENABLE);
      
	Chconfig.channelId = TDG_CHANNEL_3;
    Doconfig.offset = (uint16_t)((PWM_MOD_COUNTER / 2) + 400); 
    TDG_ChannelDelayOutputConfig(HAL_TGD_ID, &Chconfig, ENABLE);
	
	Chconfig.channelId = TDG_CHANNEL_4;
    Doconfig.offset = (uint16_t)((PWM_MOD_COUNTER / 2) + 900); 
    TDG_ChannelDelayOutputConfig(HAL_TGD_ID, &Chconfig, ENABLE);
    /* enable TDG */
    TDG_Enable(HAL_TGD_ID, ENABLE);    
    /* Load channel Configuration */
    TDGFaultFlag = (uint8_t)TDG_LoadCmd(HAL_TGD_ID);
    
    ADC_IntClear(HAL_ADC_ID,ADC_FWM_INT);
    //ADC_IntMask(HAL_ADC_ID,ADC_TCOMP_INT, UNMASK); 
    
    return TDGFaultFlag; 
    
    
}

/**
 * @brief      DMA Config function.
 *
 * @param[in]  DMADoneCbf: done int Callback that will be registered for this channel.
 *
 * @return     None.
 *
 */
uint8_t Hal_SetupDma(isr_cb_t *DMADoneCbf)
{
    uint8_t McuFaultFlag;
    DMA_TransferConfig_t GtDMATransferConfig;
    /* Select DMA channel 0 transfer */
    GtDMATransferConfig.channel= DMA_CHANNEL0;
    /* Select ADC as the DMA request for channel 0 */
    GtDMATransferConfig.source = DMA_REQ_ADC0;
    /* DMA channel priority */
    GtDMATransferConfig.channelPriority= DMA_CHN_PRIORITY0;
    /* Unmask done interrupt */
    GtDMATransferConfig.doneIntMask = UNMASK;
    /* Mask error interrupt */
    GtDMATransferConfig.errorIntMask = MASK;

    /* Address pointing to the source data */
    GtDMATransferConfig.srcAddr = (uint32_t)HAL_ADC_DATA_ADDRESS;
    /* Address pointing to the destination data */
    GtDMATransferConfig.destAddr  = (uint32_t)Hal_AdcMapData;
    /* Number of minor loop in a major loop: 1 */
    GtDMATransferConfig.minorLoopNum = 1U;
    /* Source address offset in minor loop */
    GtDMATransferConfig.minorLoopSrcOffset = 0;
    /* Destination address offset in minor loop */
    GtDMATransferConfig.minorLoopDestOffset = 4;
    /* Source address offset after a major loop done */
    GtDMATransferConfig.majorLoopSrcOffset = 0;
    /* Destination address offset after a major loop done */
    GtDMATransferConfig.majorLoopDestOffset = -20;
    
    /* Source data transfer size */
    GtDMATransferConfig.srcTransferSize = DMA_TRANSFER_SIZE_4B;
    /* Destination data transfer size */
    GtDMATransferConfig.destTransferSize = DMA_TRANSFER_SIZE_4B;
    /* Transfer bytes number */
    GtDMATransferConfig.transferByteNum = 20;
    /* Disable request after done control */
    GtDMATransferConfig.disableRequestAfterDoneCmd = DISABLE;
    /* DMA transfer Configure */
    if(ERR == DMA_ConfigTransfer(&GtDMATransferConfig))
    {
        McuFaultFlag = 1U;
    }
    else
    {
        McuFaultFlag = 0U;
    }    
    
    DMA_InstallCallBackFunc(DMA_CHANNEL0, DMA_INT_DONE, DMADoneCbf);
    /* Enable dma channel request */
    DMA_ChannelRequestEnable(DMA_CHANNEL0);

    return McuFaultFlag;    
}
/**
 * @brief      HWDIV Config function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void HAL_setupHwdiv()
{
    /* Enable HWDIV module */
    SYSCTRL_EnableModule(SYSCTRL_HWDIV);
    /* Init HWDIV module set signed divide DZE=0*/
    HWDIV_Init();
    /* Enable the Divider Fast Start Mode */
    HWDIV_DisableDivFastStart(DISABLE); 
}    
/**
 * @brief      Adc coeff Config function.
 *
 * @param[in]  iqFullScaleCurrent_A: base current value.
 * @param[in]  iqFullScaleVoltage_V: base voltage value.
 *
 * @return     None.
 *
 */
void HAL_setAdcUserScale(float iqFullScaleCurrent_A, float iqFullScaleVoltage_V)
{
    currentFactor = ZxMath_F32toQ15((float)ADC_FULL_SCALE_CURRENT_A / (float)iqFullScaleCurrent_A);
    voltageFactor = (int32_t)((float)ADC_FULL_SCALE_VOLTAGE_V / (float)iqFullScaleVoltage_V * 32767);
}

/**
 * @brief      Initial current offset value function.
 *
 * @param[in]  AdcPtr:  Offset ,current and voltage variable address.
 *
 * @return     None.
 *
 */
__ramfunc void Hal_InitialCurrentOffset(Hal_AdcType *AdcPtr)
{
	adc_reg_t * ADCx = (adc_reg_t *)(ADC0_BASE_ADDR);
    uint32_t TempValue;

    while(0U != ADCx->ADC_FCTRL.FCOUNT)
    {
        (void)ADCx->ADC_DATA_RD.ADC_DATA_RD;
    }
	
    AdcRawResults[0] = IRQn_AdcMapData[0] & (uint32_t)0xFFF;
	AdcRawResults[3] = IRQn_AdcMapData[3] & (uint32_t)0xFFF;
	AdcRawResults[4] = IRQn_AdcMapData[4] & (uint32_t)0xFFF;
	
	TempValue = IRQn_AdcMapData[1] & (uint32_t)0xFFF;
	AdcPtr->OffsetCurrent = (q15)TempValue;

	TempValue = IRQn_AdcMapData[2] & (uint32_t)0xFFF;
	AdcPtr->OffsetCurrent = (q15)TempValue;	
	
}

/**
 * @brief      Read adc data function.
 *
* @param[in]  AdcPtr:  Offset ,current and voltage variable address.
 *
 * @return     None.
 *
 */
__ramfunc void Hal_ReadAdcData(Hal_AdcType *AdcPtr)
{
    adc_reg_t * ADCx = (adc_reg_t *)(ADC0_BASE_ADDR);
    q15 TempValue1,TempValue2,TempValue3;
    while(0U != ADCx->ADC_FCTRL.FCOUNT)
    {
        (void)ADCx->ADC_DATA_RD.ADC_DATA_RD;
    }      
    AdcRawResults[0] = IRQn_AdcMapData[0] & (uint32_t)0xFFF;
    AdcRawResults[1] = IRQn_AdcMapData[1] & (uint32_t)0xFFF;
    AdcRawResults[2] = IRQn_AdcMapData[2] & (uint32_t)0xFFF;
	AdcRawResults[3] = IRQn_AdcMapData[3] & (uint32_t)0xFFF;
	AdcRawResults[4] = IRQn_AdcMapData[4] & (uint32_t)0xFFF;
    
    TempValue1 = AdcRawResults[1] - AdcPtr->OffsetCurrent;
    TempValue2 = AdcRawResults[2] - AdcPtr->OffsetCurrent;
	TempValue3 = AdcRawResults[4] - AdcPtr->OffsetCurrent;
	
    AdcPtr->Current1_pu = ZxMath_MpyQ15((TempValue1<<4),1*currentFactor);
    AdcPtr->Current2_pu = ZxMath_MpyQ15((TempValue2<<4),1*currentFactor);
	AdcPtr->BusCurrent = ZxMath_MpyQ15((TempValue3<<4),1*currentFactor);
} 
/** @} end of group HAL_Public_Functions */

/** @} end of group HAL driver modules */

/** @} end of group Z20K11XM_Foc */