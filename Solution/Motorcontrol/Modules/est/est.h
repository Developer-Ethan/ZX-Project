/**************************************************************************************************/
/**
 * @file      : Est.h
 * @brief     : Estimation module header file
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/

#ifndef EST_H
#define EST_H

#include <stdint.h>
#include "zxmathfp.h"
#include "iir_fo.h"
#include "ramp.h"
#include "user_coeff.h"
#include "user_pmsm.h"
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  EST
 *  @{
 */

/** @defgroup EST_Public_Types 
 *  @{
 */


/**
 *  @brief EST filter variable type definition
 */
typedef struct
{
    q15   Input;
    q15   Output;
    q15   FilterCoeff; 

} Est_FilterType;

/**
 *  @brief EST pi variable type definition
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
} Est_PiType;
/**
 *  @brief EST input parameter type definition
 */
typedef struct
{
  float     RunFreqKHz;        //!< Frequency to run EST
  float     BaseFreqHz;
  float     GainAngleKi; 
  float     RsOhm;             //!< Stationary resistance in Ohm
  float     LdmH;              //!< d-axis inductance in mH
  float     LqmH;              //!< q-axis inductance in mH
  float     MaxNegIdA;
  float     MaxCurrentA;  
  float     BaseCurrentA;
  float     BaseVoltageV;
  float     BaseFluxWb;  
  int32_t   GainKp;
  int32_t   GainKi;  
  q15       StdQ15Max;
  q15       StdQ15Min;
  q15       PllOutMax;
  q15       PllOutMin;
  q15       FluxFilterCoeff;
  q15       SpdFilterCoeff; 
  q15       MinCompFreq;
  q15       MaxCompFreq;
  q15       MinCompAngle;
  q15       MaxCompAngle;
  uint16_t       FagStep;
  uint16_t       FagTarget;  
} Est_ParamsType;

/**
 *  @brief EST parameter of calculation type definition
 */
typedef struct
{
    q15             Ia_pu;
    q15             Ib_pu; 
    q15             Ic_pu;
    q15             Ialpha;
    q15             Ibeta;  
    q15             Valpha;               //!< Input: The Stationary alpha-axis voltage
    q15             Vbeta;                //!< Input: The Stationary beta-axis voltage
    q15             SinTheta;
    q15             CosTheta;
    q15             ThetaIndex;    
    q15             FactorRsFlux;        //!< Parameter: Factor of Rs in flux caculation
    q15             FactorLsFlux;        //!< Parameter: Factor of Ls in flux caculation
    q15             FrAlpha;
    q15             FrBeta;
    int32_t         FluxAlpha;
    int32_t         FluxBeta; 
    q15             FluxAlphaSat;
    q15             FluxBetaSat;    
    q15             RawElecFreq;
    int32_t         RawAngle_pu;
    q15             ElecFreq;
    q15             EstTheta;
    q15             AngleFactor; 
    q15             StdQ15Max;
    q15             StdQ15Min;
    q15             MinCompFreq;
    q15             MaxCompFreq;
    q15             MinCompAngle;
    q15             MaxCompAngle;
    q15             CompAngle;    
    q15             CompAngleFactor;
    int32_t         FinalAngle;
    q15             FluxFilterCoeff;
    q15             SpdFilterCoeff;      
    q15             FagTheta;
    int32_t         Fagangle_pu;    
    uint16_t        FakeFreqStep;
    uint16_t        FakeFreqTarget;
    uint16_t        FakeFreqOutput;
#if (EST == SMO)    
    q15             Fdsmo;          //!< Parameter: Motor dependent control gain
    q15             Fqsmo;          //!< Parameter: Motor dependent control gain
    q15             Gdsmo;          //!< Parameter: Motor dependent control gain
    q15             Gqsmo;          //!< Parameter: Motor dependent control gain
    q15             Kdsmo;          //!< Parameter: Motor dependent control gain
    q15             Kqsmo;          //!< Parameter: Motor dependent control gain
    q15             Kslide;         //!< Parameter: Sliding control gain
    q15             MaxError;       //!< Parameter: Maximum current error for linear SMC
    q15             IdFb;           //!< Internal Variable: The d-axis current
    q15             IqFb;           //!< Internal Variable: The q-axis current
    q15             IdEst;          //!< Internal Variable: Estimated d-axis current
    q15             IqEst;          //!< Internal Variable: Estimated q-axis current
    q15             IdError;        //!< Internal Variable: The d-axis current error
    q15             IqError;        //!< Internal Variable: The q-axis current error
    q15             Vd;             //!< Internal Variable: The d-axis voltage
    q15             Vq;             //!< Internal Variable: The q-axis voltage
    q15             Zd;             //!< Internal Variable: Stationary d-axis sliding control
    q15             Zq;             //!< Internal Variable: Stationary q-axis sliding control
    q15             Eq;             //!< Internal Variable: Stationary q-axis back EMF after saturation
    int32_t         SumEq;          //!< Internal Variable: Stationary q-axis back EMF before saturation
    q15             IdNegmax;
    q15             Iqmax; 
#endif
    Est_PiType      EstPidPll;
    Est_FilterType  EstFilterAlphaFlux;
    Est_FilterType  EstFilterBetaFlux;  
    Est_FilterType  EstFilterSpd;
} Est_ObjType;  

/** @} end of group EST_Public_Types */

extern    Est_ObjType estObj;
    
/** @defgroup EST_Private_Defines
 *  @{
 */

/**
* @brief   Est clarke transformer
*/
#define Est_ClarkeI(A)				                         \
A.Ialpha = A.Ia_pu;					                 \
A.Ibeta = ZxMath_MpyQ15((A.Ib_pu -A.Ic_pu),Q15MATH_ONE_OVER_SQRT_THREE); 

#if (EST == SMO)
/**
* @brief   EST park transform
*/

#define EST_ParkI(A)	\
A.IdFb = (A.Ialpha * A.CosTheta + A.Ibeta * A.SinTheta)>> 15U;\
A.IqFb = (A.Ibeta * A.CosTheta - A.Ialpha * A.SinTheta)>> 15U;   

#define EST_ParkV(A)	\
A.Vd = (A.Valpha * A.CosTheta + A.Vbeta * A.SinTheta)>> 15U;\
A.Vq = (A.Vbeta * A.CosTheta - A.Valpha * A.SinTheta)>> 15U;  

#endif

/**
* @brief   Est Filter
*/
#define Est_Filter(A)           \
A.Output = A.Output + ZxMath_MpyQ15((A.Input - A.Output),A.FilterCoeff);	
/**
* @brief   Est PI
*/
#define Est_PiRun(A)       \
A.Err = A.Ref - A.Fdb;    \
A.Up= A.Kp*A.Err;         \
A.Ui = A.Ui + A.Ki*A.Err;  \
A.Ui = ZxMath_Sat(A.Ui,A.UiOutMax,A.UiOutMin);  \
A.PreOutput = (A.Up + A.Ui) >>15;            \
A.Output = ZxMath_Sat(A.PreOutput,A.OutMax,A.OutMin);   

/** @} end of group EST_Private_Defines */

/**
 * @brief      EST module intitial parameters Function.
 *
 * @param[inout]  ObjPtr: fso coeff.
 * @param[in]  ParamsPtr: motor parameters.
 *
 * @return     None.
 *
 */
void Est_Init(Est_ObjType* ObjPtr, const Est_ParamsType* ParamsPtr);

#if (EST == SMO) 
/**
 * @brief      SMO EST function.
 *
 * @param[inout]  EstPtr: Smo input, output and variable data.
 *
 * @return     None.
 *
 */

__ramfunc void Est_SmoRun(Est_ObjType* EstPtr);

#else
/**
 * @brief      Flux EST function.
 *
 * @param[inout]  EstPtr: Fso input, output and variable data.
 *
 * @return     None.
 *
 */
__ramfunc void Est_FluxRun(Est_ObjType* EstPtr);

#endif

/**
 * @brief      Fakeangle function.
 *
 * @param[inout]  EstPtr: Fakeangle output and variable data.
 *
 * @return     None.
 *
 */
__ramfunc void Est_FakeAngle(Est_ObjType* EstPtr);

/** @} end of group EST_Public_FunctionDeclaration  */

/** @} end of group EST */

/** @} end of group Z20K11XM_Foc */
#endif /* EST_H */

