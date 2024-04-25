/**************************************************************************************************/
/**
 * @file      : Global_Structure.c
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

Hal_AdcType     adcData;
TIMER_Obj       uartTxDelay;
TIMER_Obj       ledInterval;
Foc_ObjType     focObj;
OneShunt_ObjType OneShuntObj;
TC_Obj          tcObj;
Est_ObjType     estObj;
CTRL_Obj        ctrlObj;
IIR_FO_Obj      Pmsm_VdcFilter;
IIR_FO_Obj      Pmsm_TemperFilter;
IIR_FO_Obj 		Pmsm_BusCurrentFilter;
IIR_FO_Obj      ipmTempFilter;

