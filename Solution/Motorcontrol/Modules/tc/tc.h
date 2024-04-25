/**************************************************************************************************/
/**
 * @file      : tc.h
 * @brief     : Speed and Torque header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef TC_H
#define TC_H

#include <stdint.h>
#include "zxmathfp.h"
#include "pid.h"
#include "ramp.h"
#include "ctrl.h"
#include "user_coeff.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  TC
 *  @{
 */

/**
 *  @brief TC control params type definition
 */

typedef enum
{
  TC_Speed_Loop,   //!< Speed close-loop
  TC_Torque_Mode   //!< Torque open-loop
} TC_Type_e;

typedef struct _TC_Params_
{
  uint32_t  polePairs;          //!< Pole pairs of the motor
  float     runFreq_kHz;        //!< Frequency to run TC
  float     fBase_Hz;           //!< Base value of frequency used for P.U.
  float     IBase_A;            //!< Base value of current used for P.U.
  float     inertia_kgm2;       //!< The inertia of the rotoring system
  float     rotorFlux_Wb;       //!< The rotor flux, equal to BEMF in v*s/rad
  float     maxCurrent_A;       //!< Max value of speed PID output
} TC_Params;

typedef struct _TC_Obj_
{
  q15         torqueRef;    //!< Input: The torque reference value, not available if in speed loop control
  q15         speed;        //!< Input: The speed feedback
  q15         speedRef;     //!< Input: The speed reference value of speed loop control, not available in torque mode
  q15         VsRef;        //!< Input: The reference value of Vs
  q15         Vs;           //!< Input: The feedback value of Vs
  q15         IsMax;        //!< Input: The max value of Is

  PID_Obj     pidSpd;       //!< Internal Variable: The PID for speed loop
  PID_Obj     pidPow;       //!< Internal Variable: The PID for speed loop
  RAMP_Obj    spdRamp;      //!< Internal Variable: The ramp for speed reference value
  RAMP_Obj    PowRamp;      //!< Internal Variable: The ramp for speed reference value

  q15         torque;       //!< Output: The real-time torque if the output of speed PID is torque instead of Iq
  q15         IdRef;        //!< Output: The Id reference value
  q15         IqRef;        //!< Output: The Iq reference value
} TC_Obj;

typedef struct _TC_Coeff_
{
  q15       polePairs;          //!< Pole pairs of the motor
  float     runFreq_kHz;        //!< Frequency to run TC
  float     fBase_Hz;           //!< Base value of frequency used for P.U.
  float     IBase_A;            //!< Base value of current used for P.U.
  float     inertia_kgm2;       //!< The inertia of the rotoring system
  float     rotorFlux_Wb;       //!< The rotor flux, equal to BEMF in v*s/rad
  float     maxNegId_A;         //!< Max value of Id to prevent demagnetization
  float     maxCurrent_A;       //!< Max value of speed PID output
} TC_Coeff;

/** @} end of group TC control types definitions */

/** @defgroup TC FunctionDeclaration
 *  @{
 */

extern void TC_init(TC_Obj* obj, TC_Params* pParams);

extern void TC_run(TC_Obj* obj);

extern void TC_reset(TC_Obj* obj);

extern void TC_reset_fromPc();

extern void TC_setloop2_step();

extern void TC_setloop3_step();
/** @}end of group TC FunctionDeclaration */

/** @}end of group TC */

/** @}end of group Z20K11XM_Foc */
#endif /* TC_H */
