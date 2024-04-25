/**************************************************************************************************/
/**
 * @file      : Motor_Protection.h
 * @brief     : communication demo main header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef LOOPCONTROL_H
#define LOOPCONTROL_H

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
#include "user_pmsm.h"


typedef struct
{
	uint16_t SpeedLoopCnt;
	uint16_t PowerLoopCnt;
	uint8_t SpeedLoopFlag;
	uint8_t PowerLoopFlag;
	uint16_t LoopSwitchCnt;
}LOOPCTRL_DEF;

typedef enum
{
	SpeedLoop = 0,
	PowerLoop,
}LOOPTYPE_DEF;

#define SPEED_LIMIT_DETECT_TIME		(200)
#define SPEED_LIMITCLR_DETECT_TIME	(200)
extern LOOPCTRL_DEF	LoopCtrl;
extern LOOPTYPE_DEF LoopType;
extern q15 Loop_Control_Ref(q15 SpeedRef,q15 PowerRef);
extern q15 Loop_Control_Feedback(q15 SpeedBack,q15 PowerBack);
extern void LoopControl(HWSAMPLE_PARAM *HwSample,LOOPCTRL_DEF *LoopCtrl);
#endif