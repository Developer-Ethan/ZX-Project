/**************************************************************************************************/
/**
 * @file      : hal.h
 * @brief     : Hardware abstraction layer module header file
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef HAL_H
#define HAL_H

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
//#include "Global_Structure.h"
/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  HAL
 *  @{
 */

/** @defgroup HAL_Public_Types 
 *  @{
 */

static bool isPwmEnabled = false;
static tim_reg_w_t * TIMx_w = (tim_reg_w_t *)(TIM0_BASE_ADDR);
/**
 *  @brief CCV elements type definition
 */
typedef struct 
{  
    uint32_t  CCV0;   			// Output: Timx_ccv0 
    uint32_t  CCV1;   			// Output: Timx_ccv1 
    uint32_t  CCV2;   			// Output: Timx_ccv0 
    uint32_t  CCV3;   			// Output: Timx_ccv1 
    uint32_t  CCV4;   			// Output: Timx_ccv0 
    uint32_t  CCV5;   			// Output: Timx_ccv1    
} Hal_CvType;
/**
 *  @brief Adc elements type definition
 */
typedef struct
{
  uint16_t   OffsetCurrent;
  uint16_t   OffsetCurrent1;
  uint16_t   OffsetCurrent2;
  uint32_t   OffsetCurrent1_Sum;
  uint32_t   OffsetCurrent2_Sum;
  q15   Current1_pu;
  q15   Current2_pu;
  q15	BusCurrent;
  q15   Vdc_pu;
  uint16_t biasFilterCnt;
  
} Hal_AdcType;

/** @} end of group HAL_Public_Types */
static inline void enable_all_interrupt()      { __asm volatile ("cpsie i"); }
static inline void disable_all_interrupt()     { __asm volatile ("cpsid i"); }

/** @defgroup HAL_Public_FunctionDeclaration
 *  @brief HAL functions statement
 *  @{
 */
#define	CW	(1)
#define	CCW	(0)
/**
 * @brief      Update the offset data of TDG function.
 *
 * @param[in]  Ch1Delay: TDG chn1 offset value.
 * @param[in]  Ch2Delay: TDG chn2 offset value.
 *
 * @return     None.
 *
 */

__ramfunc static inline void Hal_UpdateTdgDelayOut(uint32_t Ch1Delay,uint32_t Ch2Delay)
{   
    tdg_reg_t * TDGx = (tdg_reg_t *)(TDG0_BASE_ADDR);  
    tdg_reg_w_t * TDGxw = (tdg_reg_w_t *)(TDG0_BASE_ADDR); 
    TDGxw->TDG_CHCFG[TDG_CHANNEL_1].TDG_CHDOOFS[0] = (uint16_t)Ch1Delay; 
    TDGxw->TDG_CHCFG[TDG_CHANNEL_1].TDG_CHCTRL = 0x00000100;    
    TDGxw->TDG_CHCFG[TDG_CHANNEL_2].TDG_CHDOOFS[0] = (uint16_t)Ch2Delay; 
     TDGxw->TDG_CHCFG[TDG_CHANNEL_2].TDG_CHCTRL = 0x00000100;      
    TDGx->TDG_CTRL1.CFGUP = 1U;   
}
/**
 * @brief      Update the CCV of pwm function.
 *
 * @param[in]  ObjPtr: PWM CCV value.
 *
 * @return     None.
 *
 */
__ramfunc static inline void Hal_UpdateAsymmetricOutput(Hal_CvType * ObjPtr)
{
#if	CW
	TIMx_w->TIM_CCVn[HAL_PWM_UH_CHN]= ObjPtr->CCV2;
	TIMx_w->TIM_CCVn[HAL_PWM_UL_CHN]= ObjPtr->CCV3;
	TIMx_w->TIM_CCVn[HAL_PWM_VH_CHN]= ObjPtr->CCV0;
	TIMx_w->TIM_CCVn[HAL_PWM_VL_CHN]= ObjPtr->CCV1;
	TIMx_w->TIM_CCVn[HAL_PWM_WH_CHN]= ObjPtr->CCV4;
	TIMx_w->TIM_CCVn[HAL_PWM_WL_CHN]= ObjPtr->CCV5;
#elif CCW
	TIMx_w->TIM_CCVn[HAL_PWM_UH_CHN]= ObjPtr->CCV0;
	TIMx_w->TIM_CCVn[HAL_PWM_UL_CHN]= ObjPtr->CCV1;
	TIMx_w->TIM_CCVn[HAL_PWM_VH_CHN]= ObjPtr->CCV2;
	TIMx_w->TIM_CCVn[HAL_PWM_VL_CHN]= ObjPtr->CCV3;
	TIMx_w->TIM_CCVn[HAL_PWM_WH_CHN]= ObjPtr->CCV4;
	TIMx_w->TIM_CCVn[HAL_PWM_WL_CHN]= ObjPtr->CCV5;
#endif
	TIMx_w->TIM_RELOAD |= 0x0100U;
}
/**
 * @brief      PWM output disable function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
__ramfunc static inline void Hal_PwmOutputDis(void)
{
    TIMx_w->TIM_GLBCR &= 0xffffff00U;
    isPwmEnabled = false;
}

/**
 * @brief      PWM output enable function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
__ramfunc static inline void Hal_PwmOutputEn(void)
{
      if (!isPwmEnabled)
    {
    TIMx_w->TIM_GLBCR |= 0x0000003fU;
    isPwmEnabled = true;
    }
}
/**
 * @brief      Clear DMA int flag function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
__ramfunc static inline void Hal_ClearDMAINT(void)
{
    dma_reg_w_t * DMAW = (dma_reg_w_t *)(DMA_BASE_ADDR); 
    DMAW->DMA_GCC = 0x00808000U;      // todo 0x00808080U
}
/** @defgroup HAL_Public_FunctionDeclaration
 *  @brief HAL functions statement
 *  @{
 */
void Hal_SetupNvic(void);

uint8_t Hal_SetupDma(isr_cb_t *DMADoneCbf);

void Hal_SetupInterrupts(isr_cb_t *PwmFaultIntCbf, isr_cb_t *StimIntCbf);

void Hal_SetupTmu(void);

void HAL_setupPwms();

void HAL_breakEnable();

uint8_t Hal_SetupAdcs(void);

void HAL_setupHwdiv();

void HAL_setupDebugSerialPort();

void HAL_setupLowFreqTimerIsr();

void HAL_writePwmData(q15 Ta, q15 Tb, q15 Tc);

void HAL_setAdcUserScale(float iqFullScaleCurrent_A, float iqFullScaleVoltage_V);

__ramfunc  void Hal_InitialCurrentOffset(Hal_AdcType *AdcPtr);

__ramfunc void Hal_ReadAdcData(Hal_AdcType *AdcPtr);

bool HAL_debugSerialPortReadReady();

void HAL_setupStim();

void HAL_setupClock();


#define VALUE_WM      (4)
extern uint32_t AdcRawResults[5];
extern uint32_t IRQn_AdcMapData[5];
/** @} end of group HAL_Public_FunctionDeclaration */

/** @} end of group HAL */

/** @} end of group Z20K11XM_Foc */
#endif /* HAL_H */
