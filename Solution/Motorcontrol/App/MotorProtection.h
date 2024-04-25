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
#ifndef MOTOR_PROTECTION_H
#define MOTOR_PROTECTION_H
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

typedef enum
{
	VoltageDetect = 0,
	CurrentDetect,
	TemperatureDetect,
	StallRotorDetect,
	DryRotorDetect,
	FaultSum,	
}DETECT_TYPE_DEF;

typedef struct
{
	uint16_t OvProtectCnt;
	uint16_t UdProtectCnt;
	uint16_t OvRecCnt;
	uint16_t UdRecCnt;

}DETECT_TIME_DEF;
typedef struct
{
	DETECT_TYPE_DEF Detect_Type;
	uint16_t OvProtectTH;
	uint16_t OvProtectTimeTH;
	uint16_t UdProtectTH;
	uint16_t UdProtectTimeTH;
	uint16_t OvRecTH;
	uint16_t OvRecTimeTH;
	uint16_t UdRecTH;
	uint16_t UdRecTimeTH;

}PROTECTION_PARAM_DEF;

typedef union
{
	uint16_t all;
	struct{
		uint16_t Ov:	1;
		uint16_t Uv:	1;		
		uint16_t Ot:	1;
		uint16_t Oc:	1;
		uint16_t StallRotor:	1;
		uint16_t DryRotor:	1;
		uint16_t LossSpeedRotor:	1;
		uint16_t LossPhase:	1;
		uint16_t Predriver:	1;
		uint16_t Reserve:	7;
	}bit;
}FAULT_ALL_TYPE_DEF;

typedef struct
{
	uint8_t	 StallMethod;
	uint16_t DetectCnt;
	uint16_t DetectCnt1;
	uint16_t DetectDelayCnt;
}FAULT_STALL_ROTOR_DEF;

typedef struct
{
	uint16_t DetectCnt;
	uint16_t DetectDelayCnt;
}FAULT_DRY_ROTOR_DEF;

typedef struct
{
	uint16_t ProtectRecCnt;
	uint16_t ProtectRecRelay1;
	uint16_t ProtectRecRelay2;
	uint16_t ProtectRecRelay3;
	uint32_t ProtectRecRelay4;
    uint32_t ProtectRecRelay5;
	uint32_t NormalRunCnt;
	
}FAULT_RECOVERY_DEF;

typedef struct
{
	FAULT_RECOVERY_DEF 	Fault_Recovery_StallRotor;
	FAULT_RECOVERY_DEF 	Fault_Recovery_DryRotor;
	FAULT_RECOVERY_DEF 	Fault_Recovery_LossSpeedRotor;
	FAULT_RECOVERY_DEF 	Fault_Recovery_OverCurrentRotor;
	FAULT_RECOVERY_DEF 	Fault_Recovery;
	
}FAULT_ALL_RECOVERY_DEF;

typedef struct
{
	uint8_t  FaultSendFlag;
}FLAG_FAULTSEND;

typedef struct
{
	FLAG_FAULTSEND 	Fault_StallRotor;
	FLAG_FAULTSEND 	Fault_DryRotor;
	FLAG_FAULTSEND 	Fault_OverCurrent;
	FLAG_FAULTSEND 	Fault_LossSpeed;
	FLAG_FAULTSEND 	Fault_OverTemperature;
	
}FLAG_ALLFAULTSEND;

typedef struct
{
	q15 MotorSpeed;
	q15 MotorBusCurrent;
}MC_PARAM_DEF;

typedef struct
{
	uint16_t DetectCnt_Lim;
	uint16_t DetectCnt_Oc; 
	uint16_t DetectCnt_OcReduceLoad;
	uint16_t DetectCnt_OcReduceLoad1;
	uint16_t DetectCnt_OcRec;
	uint16_t OcDelayTime;
	uint16_t OcClrDelayTime;
	uint8_t	 OcReduceLoadFlag;
}FAULT_OVERCURRENT_DEF;

typedef struct
{
	uint16_t DetectCnt_Ot;
	uint16_t DetectCnt_OtRec;
	uint16_t DetectCnt_Limt;
	uint16_t DetectCnt_LimtRec;
	uint16_t OcDelayTime;
	uint16_t OcClrDelayTime;
	uint8_t  OtReduceLoadFlag;
}FAULT_OVERTEMPERATURE_DEF;

typedef struct
{
	uint16_t DetectCnt;
}FAULT_LOSSSPEED_DEF;

typedef struct
{
	uint16_t DetectCnt;
	uint16_t DetectDelay;
}FAULT_LOSSPHASE_DEF;

typedef struct
{
	FAULT_STALL_ROTOR_DEF 		Fault_StallRotor;
	FAULT_DRY_ROTOR_DEF 		Fault_DryRotor;
	FAULT_OVERCURRENT_DEF 		Fault_OverCurrent;
	FAULT_OVERTEMPERATURE_DEF 	Fault_OverTemperature;
	FAULT_LOSSSPEED_DEF 		Fault_LossSpeed;
	FAULT_LOSSPHASE_DEF			Fault_LossPhase;
}FAULT_ALL_DEF;

typedef struct
{
	uint16_t DetectCnt;
}NOFAULT_CONDITION_DEF;

typedef struct
{
	uint16_t DetectCnt;
	uint16_t DetectCnt1;
	uint16_t DetectCnt2;
	uint8_t  FailureFlag_1st;
	uint8_t  FailureFlag_2nd;
}FAULT_DIAGVOLTAGE_DEF;

typedef enum
{
	NoFault = 0,
	DryRotor,
	StallOrOc,
	OverTemperature,
	LossSpeed,

}PRIOR_FAULTCODE_DEF;

typedef struct
{
	uint8_t Recovery_VoltFault_Enable;
	uint8_t Recovery_CurrFault_Enable;
	uint8_t Recovert_LossSpdFault_Enable;
	uint8_t Recovery_TempFault_Enable;
	uint8_t Recovery_StallFault_Enable;
	uint8_t Recovery_DryRotorFault_Enable;
}RECOVERY_FAULT_ENABLE_DEF;

extern CTRL_Obj ctrlObj;
extern FAULT_ALL_TYPE_DEF Fault_All_Type;
extern DETECT_TIME_DEF Detect_Time[FaultSum];
extern PROTECTION_PARAM_DEF Protection_Param[FaultSum];

extern FAULT_ALL_RECOVERY_DEF Fault_All_Recovery;
extern FAULT_ALL_DEF Fault_All;
extern FLAG_ALLFAULTSEND Flag_AllFaultSend;
extern MC_PARAM_DEF MC_Param;

extern FAULT_DIAGVOLTAGE_DEF Fault_DiagVoltage;
extern NOFAULT_CONDITION_DEF NoFault_Condition;
extern RECOVERY_FAULT_ENABLE_DEF Recovery_Fault_Enable;

extern void Detection_Fault(PROTECTION_PARAM_DEF *pDet,uint16_t SampleValue,FAULT_ALL_TYPE_DEF *Fault_All_Type,DETECT_TIME_DEF *pTimer);
extern void Detection_StallRotor(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_STALL_ROTOR_DEF *pFault);
extern void Detection_DryRotor(q15 SampleValue1,q15 SampleValue2,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_DRY_ROTOR_DEF *pFault);
extern void Detection_OverCurrent(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_OVERCURRENT_DEF *pFault);
extern void Recovery_Fault(FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_RECOVERY_DEF *pFaultRecovery,FLAG_FAULTSEND *pFlag_FaultSend,uint16_t FaultType);
extern void Detection_Temperature(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_OVERTEMPERATURE_DEF *pFault);
extern void Detection_LossSpeed(q15 SampleValue,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_LOSSSPEED_DEF *pFault);
extern void Detection_LossPhase(OneShunt_ObjType *pSample,FAULT_ALL_TYPE_DEF *pFault_All_Type,FAULT_LOSSPHASE_DEF *pFault);
extern uint8_t Detection_DiagVoltage(uint16_t SampleValue,FAULT_DIAGVOLTAGE_DEF *pFault);
extern void Detection_FundamentalFault(void);
extern void RecoveryFault_Enable(RECOVERY_FAULT_ENABLE_DEF *pRecov_Fault_Enable);

#endif