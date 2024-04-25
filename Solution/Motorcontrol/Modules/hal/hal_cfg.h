/**************************************************************************************************/
/**
 * @file      : hal_cfg.h
 * @brief     : Hardware abstraction layer macro definition module header file
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 28-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef HAL_CFG_H
#define HAL_CFG_H

/* mcu device configuration */
#include <stdbool.h>
#include <stdint.h>
#include "Z20K116M.h"
#include "Z20K11xM_drv.h"
#include "Z20K11xM_clock.h"
#include "Z20K11xM_sysctrl.h"
#include "Z20K11xM_wdog.h"
#include "Z20K11xM_gpio.h"
#include "Z20K11xM_uart.h"
#include "Z20K11xM_tim.h"
#include "Z20K11xM_stim.h"
#include "Z20K11xM_tmu.h"
#include "Z20K11xM_tdg.h"
#include "Z20K11xM_adc.h"
#include "Z20K11xM_dma.h"
#include "Z20K11xM_crc.h"
#include "Z20K11xM_hwdiv.h"
#include "Z20K11xM_spi.h"
/** @addtogroup  Z20K14XM_Foc
 *  @{
 */

/** @addtogroup  HAL_CFG
 *  @{
 */

/** @defgroup HAL_CFG_Private_Defines
 *  @{
 */
/* brief Defines the system clock frequency, kHz */
#define SYSTEM_FREQ_kHz                 (64000U)

/**
* @brief   Defines the maximum voltage at the input to the AD converter 
* @note    The value that will be represented by the maximum ADC input (5.0V) and conversion (0FFFh)
*          Hardware dependent, this should be based on the voltage sensing and scaling to the ADC input
*          Caculated by 5*(32.4/3)  (32.4 is total resistance of Vsense,3 is divider resistance to GND)
*/
#define ADC_FULL_SCALE_VOLTAGE_V        (64.0277f)

/**
* @brief   Defines the maximum current at the AD converter 
* @note    The value that will be represented by the maximum ADC input 5.0V - 2.5V(zero-current)
*          Hardware dependent, this should be based on the current sensing and scaling to the ADC input
*          Caculated by (2.5/0.015/10)=16.6667(0.015 is shunt resistance,10 is the loop gain of OP) 
*/   
#define ADC_FULL_SCALE_CURRENT_A        (25.0f)//16.6667
  
/* brief PWM frequency in kHZ */
#define PWM_FREQ_kHz            (20.0f)

/**
* @brief   Current loop frequency in kHZ 
* @note    Normally it will be equal to HAL_PWM_FREQ_KHZ, unless the PWM frequency it too high for the current control.
*          it should be 1/n*HAL_PWM_FREQ_KHZ, n=1,2,3...
*/
#define CURRENT_LOOP_FREQ_kHz   (10.0f)

/**
* @brief   Low frequency timer coeff for mechanical time-constant control such as speed loop, in ms 
* @note    Normally it will be 1-5ms
*/
#define SLOW_TIMER_MS           (1.0f)

/**
* @brief   Defines the rated DC bus voltage, V
*/
#define NORMAL_VDC_V  (24U)  

/**
* @brief   Defines the time2 for PWM timer source
*/
#define PWM_TIM_ID     TIM0_ID          

#define FAULT_TIM_ID   TIM0_ID
/**
* @brief   Defines the stim0 for the low freq control
*/
#define STIM_ID     STIM_0  

/**
* @brief   Defines the PWM Counter value
*/
#define PWM_MOD_COUNTER  (uint32_t)(SYSTEM_FREQ_kHz/PWM_FREQ_kHz)

/**
* @brief   Defines the PWM limit scal
*/
#define PWM_LIMIT_SCALE  (0.95f)

/* brief Defines the sample width time and delay time */
#define HAL_PWM_DEAD_TIME_US        (0.6f)     /*!< us */
#define HAL_PWM_DELAY_US            (0.5f)    /*!< us 0.75f */
#define HAL_PWM_TURN_ON_TIME_US     (0.3f)     /*!< us */
#define HAL_SAMPE_HOLD_TIME_US      (1.3f)     /*!< us 2.0f*/
#define HAL_SAMPE_WIDTH_TIME_US     (HAL_PWM_TURN_ON_TIME_US + HAL_SAMPE_HOLD_TIME_US )
#define HAL_SAMPE_DELAY_TIME_US     (HAL_PWM_DEAD_TIME_US + HAL_PWM_TURN_ON_TIME_US + HAL_PWM_DELAY_US)

/**
* @brief   Defines the PWM CHN Enable value
*/
#define PWM_U_ENABLE         (0x30U)
#define PWM_V_ENABLE         (0xC0U)  
#define PWM_W_ENABLE         (0x03U)


/* Defines function module and port */
#define HAL_TDG_SYSID            SYSCTRL_TDG0
#define HAL_ADC_ID               ADC0_ID
#define HAL_TGD_ID	         TDG0_ID   
#define HAL_CURRENTLOOPIRQ_ID    DMA0TO3_IRQn  
#define HAL_SPEEDLOOPIRQ_ID      STIM_IRQn
#define HAL_FAULTINIRQ_ID        TIM0_IRQn
#define HAL_ADC_DATA_ADDRESS     (ADC0_BASE_ADDR + 0x20U)
#define HAL_TMU_INPUT            TMU_SOURCE_TIMER0_INITIAL
#define HAL_TMU_OUTPUT           TMU_MODULE_TDG0_TRIG_IN  

#define HAL_PWM_UH_PORT          PORT_C
#define HAL_PWM_UH_PIN           GPIO_0
#define HAL_PWM_UH_PINMUX        PTC0_TIM0_CH0
#define HAL_PWM_UH_CHN           TIM_CHANNEL_0
#define HAL_PWM_UL_PORT          PORT_C
#define HAL_PWM_UL_PIN           GPIO_1
#define HAL_PWM_UL_PINMUX        PTC1_TIM0_CH1
#define HAL_PWM_UL_CHN           TIM_CHANNEL_1
#define HAL_PWM_U_PAIR           TIM_PAIR_CHANNEL_0
#define HAL_PWM_U_ENABLE         (0x30U)

#define HAL_PWM_VH_PORT          PORT_C
#define HAL_PWM_VH_PIN           GPIO_2
#define HAL_PWM_VH_PINMUX        PTC2_TIM0_CH2
#define HAL_PWM_VH_CHN           TIM_CHANNEL_2
#define HAL_PWM_VL_PORT          PORT_C
#define HAL_PWM_VL_PIN           GPIO_3
#define HAL_PWM_VL_PINMUX        PTC3_TIM0_CH3
#define HAL_PWM_VL_CHN           TIM_CHANNEL_3
#define HAL_PWM_V_PAIR           TIM_PAIR_CHANNEL_1
#define HAL_PWM_V_ENABLE         (0xC0U)

#define HAL_PWM_WH_PORT          PORT_B
#define HAL_PWM_WH_PIN           GPIO_4
#define HAL_PWM_WH_PINMUX        PTB4_TIM0_CH4
#define HAL_PWM_WH_CHN           TIM_CHANNEL_4
#define HAL_PWM_WL_PORT          PORT_B
#define HAL_PWM_WL_PIN           GPIO_5
#define HAL_PWM_WL_PINMUX        PTB5_TIM0_CH5
#define HAL_PWM_WL_CHN           TIM_CHANNEL_5
#define HAL_PWM_W_PAIR           TIM_PAIR_CHANNEL_2
#define HAL_PWM_W_ENABLE         (0x03U)

#define HAL_KEY_START_PORT    PORT_A
#define HAL_KEY_START_PIN     GPIO_5

#define PWM_OUT_PORT			PORT_A
#define	PWM_OUT_PIN				GPIO_11
#define PWM_OUT_PINMUX			PTA11_GPIO

#define HAL_ADC_TEMP_PORT        PORT_C
#define HAL_ADC_TEMP_PIN         GPIO_0
#define HAL_ADC_TEMP_PINMUX      PTC0_ADC0_CH8
#define HAL_ADC_TEMP_CHN         ADC_P_CH8

#define HAL_ADC_VMOT_PORT        PORT_A
#define HAL_ADC_VMOT_PIN         GPIO_7
#define HAL_ADC_VMOT_PINMUX      PTA7_ADC0_CH3
#define HAL_ADC_VMOT_CHN         ADC_P_CH3
   
#define HAL_ADC_IV_PORT          PORT_A
#define HAL_ADC_IV_PIN           GPIO_0
#define HAL_ADC_IV_PINMUX        PTA0_ADC0_CH0
#define HAL_ADC_IV_CHN           ADC_P_CH0 

#define HAL_ADC_BUSCURRENT_PORT          PORT_A
#define HAL_ADC_BUSCURRENT_PIN           GPIO_1
#define HAL_ADC_BUSCURRENT_PINMUX        PTA1_ADC0_CH1
#define HAL_ADC_BUSCURRENT_CHN           ADC_P_CH1 

#define HAL_LIN_SLP_N_PORT		 PORT_C
#define HAL_LIN_SLP_N_PIN		 GPIO_5
#define HAL_LIN_SLP_N_PINMUX	 PTC5_GPIO

#define HAL_PWM_CLK_MODULE       CLK_MCPWM1
#define HAL_PWM_SYSCTRL_MODULE   SYSCTRL_MCPWM1
#define HAL_ADC_CLK_MODULE       CLK_ADC0
#define HAL_ADC_SYSCTRL_MODULE   SYSCTRL_ADC0  
#define HAL_TDG_CLK_MODULE       CLK_TDG0
#define HAL_TDG_SYSCTRL_MODULE   SYSCTRL_TDG0
#define HAL_UART_CLK_MODULE      CLK_UART2
#define HAL_UART_SYSCTRL_MODULE  SYSCTRL_UART2

/** @} end of group HAL_CFG_Private_Defines */

/** @} end of group HAL_CFG */

/** @} end of group Z20K11XM_Foc */

#endif /* HAL_CFG_H */
