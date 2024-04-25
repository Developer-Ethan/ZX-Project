/**************************************************************************************************/
/**
 * @file       ctrl.h
 * @brief      Control state machine module header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef CTRL_H
#define CTRL_H

#include <stdint.h>
#include <stdbool.h>
#include "zxmathfp.h"
#include "timer.h"
#include "ramp.h"
#include "err.h"
#include "Z20K11xM_tim.h"
#include "hal.h"
#include "user_coeff.h"
#include "Z20K116M.h"
#include "user_coeff.h"
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  CTRL
 *  @{
 */

/** @defgroup CTRL_Public_Types 
 *  @{
 */

/**
 *  @brief CTRL state type definition
 */

typedef enum
{
  CTRL_State_Error = 0,        //!< Some error occurred
  CTRL_State_Powerup = 1,          //!< This should be the default state when power up
  CTRL_State_Precharging = 2,      //!< The controller is pre-charging
  CTRL_State_CalibratingI = 3,     //!< The device is calibrating current
  CTRL_State_Idle = 4,             //!< The control is idle
  CTRL_State_RotorAlign = 5,       //!< Align the rotor
  CTRL_State_Openloop = 6,         //!< Open loop startup 
  CTRL_State_CloseLoop1 = 7,       //!< Close loop1 control for startup
  CTRL_State_CloseLoop2 = 8,       //!< Close loop2 control for normal running 
  CTRL_State_CloseLoop3 = 9,       //!< Close loop3 control for shutdown
} CTRL_State_e;

/**
 *  @brief CTRL paramters of input type definition
 */

typedef struct
{  
  uint32_t     steadytime;
  uint32_t     calitime;
  uint32_t     scmBaseAddr;
  q15          biasThreshold;  
  q15          rotorAlignCurrentA; 
  uint32_t     rotorAlignTimeMs;   
  uint32_t     openloopMaxtimeMs;
  uint32_t     loop1TimeMs;
  q15          LFreqThresholdHz; 
  q15          finalFreqHz;     
  uint16_t     cntThreshold;
  q15          alignCurrentStep;
  q15          IFCurrentStep;    
} CTRL_ParamsType;

typedef struct _CTRL_Obj_
{
  bool            startMotor;           //!< Input: start command of the motor
  uint32_t        steadytime;
  uint32_t        calitime;
  q15             freqRef;              //!< Input: Reference electric frequency
  q15             freqfb;               //!< Input: Est electric frequency
  q15             estangle_pu;          //!< Input: SMO angle output in P.U.
  q15             OPangle_pu;           //!< Input: openloop angle output in P.U.
  q15             dcbusVoltage_pu;      //!< Input: Voltage of DC bus in P.U.
  q15             Ia_pu;                //!< Input: Phase A current
  q15             Ib_pu;                //!< Input: Phase B current
  q15             Ic_pu;                //!< Input: Phase C current
  q24             Ia_pu_Sum;                //!< Input: Phase A current
  q24             Ib_pu_Sum;                //!< Input: Phase B current
  q24             Ic_pu_Sum;                //!< Input: Phase C current
  q15             biasThreshold; 
  uint16_t		  biasFilterCnt;
  uint32_t        rotorAlignTimeMs;
  uint32_t        openloopMaxtimeMs;
  uint32_t        loop1TimeMs;  
  uint16_t        cntThreshold;  
  q15             LFreqThresholdHz; 
  q15             finalFreqHz;   
  bool            needStartOnFly;        //!< Parameter: Whether start-on-fly is required
  bool            needCalibI;            //!< Parameter: Whether Calibration is required
  bool            needCalibV;            //!< Parameter: Whether Calibration is required
  bool            needRotorAlign;        //!< Parameter: Need rotor alignment
  q15             rotorAlignCurrent_pu;  //!< Parameter: The rotor alignment current in P.U.
                  
  RAMP_Obj        rotorAlignCurrentRamp;   //!< Internal Variable: The ramp for the rotor alignment current
  RAMP_Obj        IFCurrentRamp;           //!< Internal Variable: The ramp for I/F control current 
  TIMER_Obj       steadyTimer;             //!< Internal Variable: The timer for power steady
  TIMER_Obj       calibTimer;              //!< Internal Variable: The timer for calibration
  TIMER_Obj       rotorAlignTimer;         //!< Internal Variable: The timer for rotor alignment
  TIMER_Obj       IFTimer;                 //!< Internal Variable: The timer for IF
  TIMER_Obj       closeLoop1Timer;         //!< Internal Variable: The timer for closeLoop1  
  uint16_t        Cntr;                    //!< Output: Counter for angle erro */
  CTRL_State_e    state;                //!< Output: Current state
  bool            enablePwm;            //!< Output: Flag of enable the PWM output
  bool            enableCurrentCalib;   //!< Output: Command of enable the calibration of current sensor
  bool            enableVoltageCalib;   //!< Output: Command of enable the calibration of voltage sensor

} CTRL_Obj;

/** @} end of group CTRL_Public_Types*/

extern void Foc_Reset();

extern void Tc_Reset();

extern void TC_setloop2_step(); 

extern void TC_setloop3_step(); 

extern void CTRL_switchToCloseLoop();

extern void CTRL_Resetopenloopdata();


void CTRL_init(CTRL_Obj* ObjPtr , CTRL_ParamsType* ParamsPtr);

extern void CTRL_updateState(CTRL_Obj* obj);

static inline void CTRL_tick(CTRL_Obj* obj)
{

    TIMER_tick(&obj->steadyTimer);  
    TIMER_tick(&obj->calibTimer);
    TIMER_tick(&obj->rotorAlignTimer);
    TIMER_tick(&obj->IFTimer);
    TIMER_tick(&obj->closeLoop1Timer);
}


/** @} end of group CTRL_Public_FunctionDeclaration */

/** @} end of group CTRL */

/** @} end of group Z20K11XM_Foc */


#endif /* CTRL_H */
