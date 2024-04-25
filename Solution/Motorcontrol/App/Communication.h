/**************************************************************************************************/
/**
 * @file      : communication.h
 * @brief     : communication demo main header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include <stdint.h>
#include "zxmathfp.h"
#include "Z20K116M.h"
#include "Z20K11xM_drv.h"
#include "Z20K11xM_tim.h"
#include "Z20K11xM_clock.h"
#include "Z20K11xM_sysctrl.h"
#include "Z20K11xM_adc.h"
#include "Z20K11xM_dma.h"
#include "Z20K11xM_wdog.h"
#include "Z20K11xM_gpio.h"
#include "Z20K11xM_tdg.h"
#include "Z20K11xM_uart.h"
#include "Z20K11xM_srmc.h"
#include "Z20K11xM_pmu.h"
#include "Z20K11xM_stim.h"
#include "Z20k11xM_drv.h"
#include "Z20k11xM_hwdiv.h"
#include "hal_cfg.h"
#include "ctrl.h"
#include "Z20A8300A_Spi.h"
#include "Z20A8300A_Init.h"
#include "MotorProtection.h"
#include "EventHandle.h"
	 
typedef enum
{
  Z20A8300A_Fault = 0,
  Z20A8300A_NoFault,
}Status_Z20A8300A;

typedef struct
{
	uint16_t Duty;
	q15		SpeedRef;
  	uint8_t SpeedLimFlag;
  	uint16_t SpeedLimCnt;
	uint16_t SpeedLimClrCnt;
	
}SPEED_CTRL_DEF;

typedef struct
{
	q15 SpeedTarget;
	q15 SpeedRealTime;
}MC_DEF;
typedef struct
{
	uint32_t OverFlowCount;
	uint32_t OverFreqRecovCount;
	uint32_t HighLeverCount;
	uint32_t LowLeverCount;
	uint8_t	 OverFlowFlag;
	uint8_t  OverFreqFlag;
	uint8_t	 LeverState;
}TIM_TO_DEF;

#define TIME_SPEEDLOOP		(500)
#define TIME_POWERLOOP		(1500)

#define T_FREQ_MIN 			(29771)//8HZ->31525 9HZ->28018
#define T_FREQ_MAX 			(2262)//110HZ->2272 111HZ->2251

#define STATUS_OF			(1)
#define STATUS_HL			(2)
#define STATUS_LL			(3)
extern SPEED_CTRL_DEF Speed_Ctrl;
extern TIM_TO_DEF Time_To;
extern MC_DEF MotorCtrl;
extern Status_Z20A8300A GetStatus_Diag(void);
extern void Detection_Key(PORT_ID_t port, PORT_GPIONO_t gpioNo);
//extern void SPI_Communication(void);
extern void PWM_Capture(void);
extern void Speed_Adjust(void);
extern void ON_OFFControl(void);
extern void Init_LIN(void);
extern void TIM_ToIsr(void);

#endif