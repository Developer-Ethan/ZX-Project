/**************************************************************************************************/
/**
 * @file      : zxmathfp.h
 * @brief     : math function head file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef ZXMATHFP_H
#define ZXMATHFP_H


#include <stdlib.h>
#include <stdint.h>
#include "Z20K116M.h"

// **************************************************************************
// the defines

#define MATH_PI                         (3.1415926535897932384626433832795)
#define MATH_2PI                        (6.283185307179586476925286766559)
#define MATH_ONE_OVER_THREE             (0.3333333333)
#define MATH_ONE_OVER_SQRT_THREE        (0.5773502692)
#define MATH_SQRT_THREE_OVER_TWO        (0.8660254038)
#define Q15MATH_ONE                     (32768)
#define Q15MATH_ONE_OVER_THREE          (10922)
#define Q15MATH_ONE_OVER_SQRT_THREE     (18918) 
#define Q15MATH_SQRT_THREE_OVER_TWO     (28378) 
#define Q15MATH_PI                      (102943)
#define Q15MATH_2PI                     (205887)
#define Q15MATH_PI_OVER_TWO             (51472)
 
#define Q15_MAX                         ((int16_t)32766)
#define Q15_MIN                         ((int16_t)-32766)
#define Q15_STDMAX                      ((int16_t)32767)
#define Q15_STDMIN                      ((int16_t)-32767)  
// **************************************************************************
// the typedefs

typedef   int16_t   q15;
typedef   int32_t   q24;

// **************************************************************************
// the globals


// **************************************************************************
// the function prototypes

extern  int16_t ZxMath_SineTableQ15[1280];
extern  int16_t *ZxMath_cosTableQ15Ptr;
extern int16_t zx_arctanQ15(int16_t A);

#define     ZxMath_F32toQ15(A)              (int16_t)((A) * 32767)
#define     ZxMath_Q15toF32(A)              ((float)(A) * 0.000030517578)
#define     ZxMath_F32toQ16(A)              (uint16_t)((A) * 65535)

#define     ZxMath_Max(A, B)                (A>B ? A : B)
#define     ZxMath_Min(A, B)                (A<B ? A : B)

#define     ZxMath_Sat(A, Pos, Neg)         (ZxMath_Max(((ZxMath_Min((A),(Pos)))),(Neg)))


#define     zx_Q15toQ14(A)              ((int16_t) (A) >> 1)
#define     zx_Q15toQ13(A)              ((int16_t) (A) >> 2)
#define     zx_Q15toQ12(A)              ((int16_t) (A) >> 3)
#define     zx_Q15toQ11(A)              ((int16_t) (A) >> 4)
#define     zx_Q15toQ10(A)              ((int16_t) (A) >> 5)
#define     zx_Q15toQ9(A)               ((int16_t) (A) >> 6)
#define     zx_Q15toQ8(A)               ((int16_t) (A) >> 7)
#define     zx_Q15toQ7(A)               ((int16_t) (A) >> 8)
#define     zx_Q15toQ6(A)               ((int16_t) (A) >> 9)
#define     zx_Q15toQ5(A)               ((int16_t) (A) >> 10)
#define     zx_Q15toQ4(A)               ((int16_t) (A) >> 11)
#define     zx_Q15toQ3(A)               ((int16_t) (A) >> 12)
#define     zx_Q15toQ2(A)               ((int16_t) (A) >> 13)
#define     zx_Q15toQ1(A)               ((int16_t) (A) >> 14)

#define     zx_Q14toQ15(A)              ((int16_t) (A) << 1)
#define     zx_Q13toQ15(A)              ((int16_t) (A) << 2)
#define     zx_Q12toQ15(A)              ((int16_t) (A) << 3)
#define     zx_Q11toQ15(A)              ((int16_t) (A) << 4)
#define     zx_Q10toQ15(A)              ((int16_t) (A) << 5)
#define     zx_Q9toQ15(A)               ((int16_t) (A) << 6)
#define     zx_Q8toQ15(A)               ((int16_t) (A) << 7)
#define     zx_Q7toQ15(A)               ((int16_t) (A) << 8)
#define     zx_Q6toQ15(A)               ((int16_t) (A) << 9)
#define     zx_Q5toQ15(A)               ((int16_t) (A) << 10)
#define     zx_Q4toQ15(A)               ((int16_t) (A) << 11)
#define     zx_Q3toQ15(A)               ((int16_t) (A) << 12)
#define     zx_Q2toQ15(A)               ((int16_t) (A) << 13)
#define     zx_Q1toQ15(A)               ((int16_t) (A) << 14)

#define HWDIV     ((hwdiv_reg_t *) HWDIV_BASE_ADDR)      /*!< HWDIV Register */
#define HWDIV_W   ((hwdiv_reg_w_t *) HWDIV_BASE_ADDR)    /*!< HWDIV Register */
__ramfunc static inline int16_t HWDIVQ15(int16_t dividend, int16_t divisor)
{
   int16_t div_result;
    while( HWDIV->HWDIV_CSR.BUSY == 1U ){}; 
    HWDIV->HWDIV_CSR.USGN = 0U;
    HWDIV_W->HWDIV_DEND = (int32_t)(dividend << 15);
    HWDIV_W->HWDIV_DSOR = (int32_t)divisor;
    while( HWDIV->HWDIV_CSR.BUSY == 1U ){}; 
    div_result =  (int16_t)HWDIV_W->HWDIV_RES;
    return div_result;    
}
    
    
__ramfunc static inline uint32_t HWSQR(uint32_t radicand)
{
    while( HWDIV->HWDIV_CSR.BUSY == 1U ){};     
    HWDIV_W->HWDIV_RCND = (uint32_t)radicand;   
    while( HWDIV->HWDIV_CSR.BUSY == 1U ){};     
    return (uint32_t)HWDIV_W->HWDIV_RES;    
}  
    
#define     ZxMath_MpyQ16(A,B)          ((int16_t)(((int32_t)A * (int32_t)B) >> 16))

#define     ZxMath_MpyQ15(A,B)          ((int16_t)(((int32_t)A * (int32_t)B) >> 15))

#define     ZxMath_DivQ15(A,B)              (B==0 ? 0x7FFF : HWDIVQ15(A, B))

#define     ZxMath_Sqrt(A)                 (((A) < 0) ? 0: HWSQR(A))

#define     ZxMath_Abs(A)                  (((A) < 0) ? - (A) : (A))
  
  


#endif /* ZXMATHFP_H */
