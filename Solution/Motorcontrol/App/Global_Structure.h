
#ifndef GLOBAL_STRUCTURE_H
#define GLOBAL_STRUCTURE_H

#include <stdbool.h>
#include <stdint.h>
#include "hal.h"
#include "foc.h"
#include "Oneshunt.h"
#include "timer.h"
#include "tc.h"
#include "err.h"
#include "est.h"
#include "ramp.h"
#include "ctrl.h"
#include "Z20K11xM_gpio.h"
#include "Z20A8300A_Init.h"
#include "Z20K11xM_adc.h"
#include "hal_cfg.h"
#include "pmsm.h"
#include "user_pmsm.h"
#include "user_coeff.h"
#include "Communication.h"
#include "EventHandle.h"
#include "MotorProtection.h"
#include "math.h"
#include "Diagnose.h"
#include "LoopControl.h"
#include "string.h"
#include "zxmathfp.h"
#include "Z20K11xM_tim.h"

extern Hal_AdcType     adcData;
extern TIMER_Obj       uartTxDelay;
extern TIMER_Obj       ledInterval;
extern Foc_ObjType     focObj;
extern OneShunt_ObjType OneShuntObj;
extern TC_Obj          tcObj;
extern Est_ObjType     estObj;
extern CTRL_Obj        ctrlObj;
extern IIR_FO_Obj      Pmsm_VdcFilter;
extern IIR_FO_Obj      Pmsm_TemperFilter;
extern IIR_FO_Obj 		Pmsm_BusCurrentFilter;
extern IIR_FO_Obj      ipmTempFilter;


#endif