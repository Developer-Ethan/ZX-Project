/**************************************************************************************************/
/**
 * @file      : pmsm.h
 * @brief     : pmsm demo main header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef PMSM_H
#define PMSM_H

//#include <stdbool.h>
//#include <stdint.h>
//#include "hal.h"
//#include "foc.h"
//#include "Oneshunt.h"
//#include "timer.h"
//#include "tc.h"
//#include "err.h"
//#include "est.h"
//#include "ramp.h"
//#include "ctrl.h"
//#include "Z20K11xM_gpio.h"
//#include "Z20A8300A_Init.h"

/** @addtogroup  Z20K11XM_Foc
 *  @{
 */

/** @addtogroup  PMSM
 *  @{
 */

/** @defgroup PMSM_Public_FunctionDeclaration
 *  @brief PMSM functions statement
 *  @{
 */
/**
 * @brief      Currentloop function.
 *
 * @param[in]  None.
 *
 * @return     None.
 *
 */
#define Ualpha_Coefficient	(q15)((1.0/(3*(PWM_MOD_COUNTER/2)))*32767)
#define Ubeta_Coefficient	(q15)((1.0/(1.7320508*(PWM_MOD_COUNTER/2)))*32767)


void Pmsm_IRQHandleCurrentLoopIsr(void);
void Pmsm_IRQHandleBkinIsr(void);
void Pmsm_IRQHandleSpeedLoopIsr(void);
extern int32_t voltageFactor;
extern q15 currentFactor;
extern uint8_t PredriverFault_PowerOn;
/** @} end of group PMSM_Public_FunctionDeclaration */

/** @}end of group PMSM */

/** @}end of group Z20K11XM_Foc */

#endif /* PMSM_H */


