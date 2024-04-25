/**************************************************************************************************/
/**
 * @file      : foc.h
 * @brief     : foc module header file .
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#ifndef FOC_H
#define FOC_H

#include <stdint.h>
#include "zxmathfp.h"
#include "pid.h"
#include "user_coeff.h"
#include "hal.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  FOC
 *  @{
 */

/** @defgroup FOC_Public_Types 
 *  @{
 */

/**
 *  @brief FOC paramter of input type definition
 */
typedef struct
{
  float     VsMaxScale;       //!< Max allowed Vs
  float     VdMaxScale;       //!< Max allowed Vd scale factor based on Vs.
  float     Rs_Ohm;           //!< Stationary resistance in Ohm
  float     Ls_mH;            //!< Inductance in mH
  float     runFreq_kHz;      //!< Frequency to run EST
  float     fBase_Hz;         //!< Base value of frequency used for P.U.
  float     VoltageBase_V;    //!< Base value of voltage used for P.U.
  float     ratedVdc_V;       //!< Rated dc bus voltage
  float     IBase_A;          //!< Base value of current used for P.U.
  float     MinSampeWidthTime;
  float     SampeDelayTime;
  float     PwmCycle;
  float     PwmLimitScale; 
  float     VsLimitSacle;
  uint32_t  PwmMod;
  int32_t   GainKp;
  int32_t   GainKi;
  q15       StdQ15Max;
} Foc_ParamsType;  
/**
 *  @brief FOC pi regulator paramters of input type definition
 */  
typedef struct 
{  
    int32_t  Ref;
    int32_t  Fdb; 
    int32_t  Err;
    int32_t  Kp;
    int32_t  Ki; 
    int32_t  Ui;                               
    int32_t  Up;
    int32_t  UiOutMax;
    int32_t  UiOutMin;
    int32_t  PreOutput;                                  
    int32_t  OutMax;
    int32_t  OutMin;
    int32_t  Output; 
} Foc_PiType;
/**
 *  @brief FOC paramter of calculation type definition
 */
typedef struct
{
    q15  ThetaIndex;
    q15  Theta;
    q15  IdFb;
    q15  IqFb;
    q15  Ialpha;  
    q15  Ibeta;   
    q15  SinTheta;   	
    q15  CosTheta;   		
    q15  Vd; 
    q15  Vq;    
    q15  Valpha;
    q15  Vbeta;
    q15  Valpha_pu;
    q15  Vbeta_pu;
    q15  RefTemp;
    q15  Ref1;
    q15  Ref2;
    q15  Ref3;
    q15  VsMax;
    q15  RealVdc;
    q15  VdMax;
    q15  VsMaxScaleQ15;
    q15  VsMaxLimitSacle;
    q15  VsMaxLimit;      
    q15  IdRef;
    q15  IqRef;
    q15  StdQ15Max;
    Hal_CvType  FocCv; 
    uint8_t     Sector;
    uint32_t    PwmMod;
    uint16_t    Ta;
    uint16_t    Tb;
    uint16_t    Tc;
    uint16_t    T1;
    uint16_t    T2;
    uint16_t    MinSampeDuty;
    uint16_t    ShiftDuty1;
    uint16_t    ShiftDuty2; 
    uint16_t    DutyU;
    uint16_t    DutyV;
    uint16_t    DutyW;  
    uint32_t    OffsetTime1;
    uint32_t    OffsetTime2;  
    uint32_t    SampeDelayDuty;
    uint32_t    PwmLimitMax;
    uint32_t    PwmLimitMin;    
    Foc_PiType  FocPidIdObj;
    Foc_PiType  FocPidIqObj;  
} Foc_ObjType;
/** @} end of group FOC_Public_Types */

extern    Foc_ObjType focObj;

/** @defgroup FOC_Private_Defines
 *  @{
 */

/**
* @brief   FOC park transform
*/
#define FOC_PARK(A)	\
A.IdFb = (A.Ialpha * A.CosTheta + A.Ibeta * A.SinTheta)>> 15U;\
A.IqFb = (A.Ibeta * A.CosTheta - A.Ialpha * A.SinTheta)>> 15U;       
/**
* @brief   FOC ipark transform
*/
#define FOC_IPARK(A)	\
A.Valpha = ((A.Vd*A.CosTheta) - (A.Vq*A.SinTheta)) >> 15U;\
A.Vbeta  = ((A.Vq*A.CosTheta) + (A.Vd*A.SinTheta)) >> 15U;      
/**
* @brief   FOC iclark transform
*/
#define FOC_ICLARKE(A)			     \
A.Valpha_pu = ZxMath_DivQ15(A.Valpha,A.VsMax);\
A.Vbeta_pu = ZxMath_DivQ15(A.Vbeta,A.VsMax);\
A.RefTemp = ((28377U * A.Valpha_pu) >> 15U); \
A.Ref3 = -(A.Vbeta_pu >> 1U) - A.RefTemp; \
A.Ref2 = -(A.Vbeta_pu >> 1U ) + A.RefTemp; \
A.Ref1 = A.Vbeta_pu;                           
/**
* @brief   FOC sincos value caculation
*/
#define FOC_SINCOS_VALUE(A)\
A.ThetaIndex = A.Theta >> 5U;\
A.SinTheta = ZxMath_SineTableQ15[A.ThetaIndex];\
A.CosTheta = ZxMath_cosTableQ15Ptr[A.ThetaIndex];
/**
* @brief   FOC pid regulator
*/
#define FOC_PIDRUN(A)	\
A.Err = A.Ref - A.Fdb; \
A.Up = A.Kp*A.Err;\
A.Ui = A.Ui + A.Ki*A.Err; \
A.Ui = ZxMath_Sat(A.Ui,A.UiOutMax,A.UiOutMin);\
A.PreOutput = (A.Up + A.Ui) >>15U; \
A.Output = ZxMath_Sat(A.PreOutput,A.OutMax,A.OutMin);      

/** @} end of group FOC_Private_Defines */

/** @defgroup FOC_Public_FunctionDeclaration
 *  @brief FOC functions statement
 *  @{
 */
/**
 * @brief       Foc function.
 *
 * @param[inout]  FocPtr: foc variabes of Id Iq ,uvw voltage ,duty ,cv and offset.
 *
 * @return     None.
 *
 */
__ramfunc void Foc_Run(Foc_ObjType * FocPtr);
/**
 * @brief       Foc reset  function.
 *
 * @param[inout]  FocPtr: foc variabes of Id Iq ,uvw voltage ,duty ,cv and offset.
 *
 * @return     None.
 *
 */
__ramfunc void Foc_ResetParams(Foc_ObjType * FocPtr);
/**
 * @brief      Foc initial  function.
 *
 * @param[inout]  ObjPtr: KP KI KD and Id,Iq PID parameters and CV initial value.
 * @param[in]  ParamsPtr: motor paramters and limits data address.
 *
 * @return     None.
 *
 */
void Foc_Init(Foc_ObjType* ObjPtr, const Foc_ParamsType* ParamsPtr);
/**
 * @brief      Foc reset  function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
void Foc_Reset(void);


/** @} end of group FOC_Public_FunctionDeclaration */

/** @} end of group FOC */

/** @} end of group Z20K11XM_Foc */

#endif /* FOC_H */
