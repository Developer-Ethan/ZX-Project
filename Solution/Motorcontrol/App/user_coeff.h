/**************************************************************************************************/
/**
 * @file      : user_coeff.h
 * @brief     : User coeff design header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef USER_COEFF_H
#define USER_COEFF_H

#include "zxmathfp.h"

#define EST  (1)

#define SMO  (1)
#define FSO  (2)

/* These parameters are related for control status machine */
#define STEADY_TIME_MS           (200U)                       //!< Precharging time  
#define CALIBRATION_TIME_MS      (200U)                        //!< Calibration time for sensors
#define ZERO_CURRENT_THRESHOLD   ZxMath_F32toQ15(0.025)        //!< Current sensor threshold value in zero point
  
/* brief These parameters are Coeffs for PID control */
#define TC_KP_GAIN               (4000U)       //!< KP coeff in TC&speed control Range(0~32767)
#define TC_KI_GAIN               (100)        //!< KI coeff in TC&speed control Range(0~32767) 
#define TC_KD_GAIN               (0U)          //!< KD coeff in TC&speed control Range(0~32767)
#define POW_KP_GAIN              (5000U)       //!< KP coeff in TC&speed control Range(0~32767)
#define POW_KI_GAIN              (200)        //!< KI coeff in TC&speed control Range(0~32767) 
#define POW_KD_GAIN              (0U)          //!< KD coeff in TC&speed control Range(0~32767)
#define FOC_KP_GAIN              (20000U)       //18000!< KP coeff in FOC control Range(0~32767) 
#define FOC_KI_GAIN              (30U)        //!< KI coeff in FOC control Range(0~32767)
#define EST_FSO_KP_GAIN          (700U)        //!< KP coeff in FSO control Range(0~32767) 700U
#define EST_FSO_KI_GAIN          (150U)        //!< KI coeff in FSO control Range(0~32767)
#define EST_SMO_KP_GAIN          (7000U)       //!< KP coeff in SMO control Range(0~32767) 1600U 2000U
#define EST_SMO_KI_GAIN          (50U)        //!< KI coeff in SMO control Range(0~32767) 140U 200U

/* brief This parameter is lated for angle itegration */
#define USER_KI_ANGLE            (1.0f)   

/* brief These parameters are lated for observer filters */
#define COEFF_FLUX_FILTER      	 (600U)
#define COEFF_SPD_FILTER         (160U)

/* brief These parameters are lated for SMO coeff */
#define SMC_GAIN                 (0.7)          //!< Slide Mode Controller Gain (0.0 to 0.9999)
#define MAX_LINEAR_SMC           (0.2)           //!< Slide Mode Controller Max allowed linear error (0.0 to 0.9999)0.5

/* brief These parameters are lated for start up performance */

#define ROTORALIGN_CURRENTRAMP_STEP 				ZxMath_F32toQ15(0.0003)   //!< Current ramp step value in rotor algin control
#define ROTOR_ALIGN_ANGLE 							ZxMath_F32toQ15(0.25)               //!< Rotor align angle
#define ROTOR_ALIGN_TIME_MS   						(200)                          //!< Rotor align duration time
#define Align_CURRENT_PU  							ZxMath_F32toQ15(0.05)                //!< Align current for rotor

/*motor openloop param*/
#define STARTUP_CURRENT_PU   						ZxMath_F32toQ15(0.05)             //!< Peak current for the startup stage 
#define OPENLOOPSTEP_FREQ   						ZxMath_F32toQ16(0.000016)         //!< Frequency ramp step value in open loop 0.000035
#define OPENLOOPSPEED_TARGET  						ZxMath_F32toQ16(0.14)            //!< Openloop Target speed
#define FINAL_SPEED_PU  							ZxMath_F32toQ15(0.12)                 //!< Final speed in the close loop
#define LOOP1_TIME_MS  								(200)                                 //!< Loop1 duration time

/*condition of switching to IF from Start phase*/
#define SPEED_LWTHRESHOLD  							ZxMath_F32toQ15(0.06)              //!< Define the basic condition for switch to close loop and when degrade the current for IF

/*IF param*/
#define IF_CURRENT_PU  								ZxMath_F32toQ15(0.008)                  //!< IF control target current,smoothly switch from startup current
#define IF_CURRENTRAMP_STEP 						ZxMath_F32toQ15(0.000031)         //!< Current ramp step value in open loop
#define MAX_OPENLOOP_TIME_MS   						(200)                                   //!< IF stage duration time

/*condition of switching to closed loop from IF phase*/
#define DELTA_THETA_THRESHOLD  						ZxMath_F32toQ15(0.03167)       //!< The Delta angle range is for switching to the close loop
#define ANGLE_ERROTHRESHOLD_COUNT      				(2U)                  //!< The count threshold of angle erro

/*closed loop param*/
#define SPDRAMP_LOOP1_STEP 							ZxMath_F32toQ15(0.00008)           //!< Frequency ramp step value in close loop1 control 
#define SPDRAMP_LOOP2_STEP 							ZxMath_F32toQ15(0.0004)          //!< Frequency ramp step value in close loop2 control 
#define SPDRAMP_LOOP3_STEP 							ZxMath_F32toQ15(0.0002)           //!< Frequency ramp step value in close loop3 control

/*brake shut down param*/
#define SHUTDOWN_SPEED_PU  ZxMath_F32toQ15			(0.075)             //!< Shut down target value 
#define SHUTDOWN_SPEED_THRESHOLD_PU  ZxMath_F32toQ15(0.0875)  //!< Will turn off drive output if run speed below this value  
#define BD_THRESHOLD  								(ZxMath_F32toQ15(0.001))                //!< Current for Dead band 0.02A default

/****StallRotor Protection Param****/
#define USER_FULL_SCALE_SPEED_RPM		(60 * USER_IQ_FULL_SCALE_FREQ_Hz/USER_MOTOR_NUM_POLE_PAIRS)
#define Motor_StallRotor_Speed_Max		(6000.0f)
#define MOTOR_STALLROTOR_SPEED_MAX		ZxMath_F32toQ15(Motor_StallRotor_Speed_Max/USER_FULL_SCALE_SPEED_RPM)
#define Motor_StallRotor_Speed_Min		(500.0f)
#define MOTOR_STALLROTOR_SPEED_MIN		ZxMath_F32toQ15(Motor_StallRotor_Speed_Min/USER_FULL_SCALE_SPEED_RPM)
#define ESTI_EMF_SQUARE_MIN				(76)
#define STALLROTOR_DETECT_TIME			(200)
#define STALLROTOR_DETECTDELAY_TIME		(500)
#define STALLROTOR_TYPE					(0X0010)

/****DryRotor Protection Param****/
#define Motor_DryRotor_Speed_Min    	(1500.f)
#define MOTOR_DRYROTOR_SPEED_MIN    	ZxMath_F32toQ15(Motor_DryRotor_Speed_Min/USER_FULL_SCALE_SPEED_RPM)
#define Motor_DryRotor_BusCurrent		(1.0f)
#define MOTOR_DRYROTOR_BUSCURRENT		ZxMath_F32toQ15(Motor_DryRotor_BusCurrent/USER_IQ_FULL_SCALE_CURRENT_A)
#define DRYROTOR_DETECT_TIME			(200)
#define DRYROTOR_DETECTDELAY_TIME		(500)
#define DELAYDETECT_TIME				(5000)
#define DRYROTOR_TYPE					(0X0020)

/****OverCurrent Protection Param****/
#define OverCurrentValue1				(9.0f)
#define OVERCURRENT_VALUE1				ZxMath_F32toQ15(OverCurrentValue1/USER_IQ_FULL_SCALE_CURRENT_A)
#define OverCurrentValue2				(11.0f)
#define OVERCURRENT_VALUE2				ZxMath_F32toQ15(OverCurrentValue2/USER_IQ_FULL_SCALE_CURRENT_A)
#define OVERCURRENT_DETECT_TIME			(200)

#define LimitCurrentValue_ReConstr		(5.1f)
#define LIMITCURRENT_VALUE_RECONSTR		ZxMath_F32toQ15(LimitCurrentValue_ReConstr/USER_IQ_FULL_SCALE_CURRENT_A)
#define LimitCurrentValue_HwSamp		(5.7f)
#define LIMITCURRENT_VALUE_HWSAMP		ZxMath_F32toQ15(LimitCurrentValue_HwSamp/USER_IQ_FULL_SCALE_CURRENT_A)

#define LimitCurrentValue1				(2.58f)
#define LIMITCURRENT_VALUE1				ZxMath_F32toQ15(LimitCurrentValue1/USER_IQ_FULL_SCALE_CURRENT_A)
#define	LIMITCURRENT_DETECT_TIME		(200)
#define OVERCURRENT_REC_DETECT_TIME		(200)

#define OC_REDUCELOAD_SPEED				(q15)(0.6 * FREQUENCY_MAX)
#define OVERCURRENT_TYPE				(0X0008)

/****Temperature Protection Param****/
#define R_PULL							(10.0f)
#define LimitTemperature_Value			(3.38f)
#define	LIMITTEMPERATURE_VALUE			ZxMath_F32toQ15((LimitTemperature_Value / (LimitTemperature_Value + R_PULL))*5/5)
#define RecoverTemperature_Value		(3.934f)
#define RECOVERTMPERATURE_VALUE			ZxMath_F32toQ15((RecoverTemperature_Value / (RecoverTemperature_Value + R_PULL))*5/5)

#define OverTemperature_Value			(2.522f)
#define OVERTEMPERATURE_VALUE			ZxMath_F32toQ15((OverTemperature_Value / (OverTemperature_Value + R_PULL))*5/5)
#define RestartTemperature_Value		(2.916f)
#define RESTARTTEMPERATURE_VALUE		ZxMath_F32toQ15((RestartTemperature_Value / (RestartTemperature_Value + R_PULL))*5/5)

#define OT_REDUCELOAD_SPEED				(q15)(0.6 * FREQUENCY_MAX)

#define LIMITTEMPERATURE_DETECT_TIME 	(200)
#define OVERTEMPERATURE_DETECT_TIME		(200)
#define OTREC_DETECT_TIME				(20000 - (STEADY_TIME_MS + CALIBRATION_TIME_MS))
#define LIMTREC_DETECT_TIME				(20000)

/****LossSpeed Protection Param****/
#define	LossSpeed_Value					(1200.0f)
#define	LOSSSPEED_VALUE					ZxMath_F32toQ15((LossSpeed_Value*USER_MOTOR_NUM_POLE_PAIRS)/(60*USER_IQ_FULL_SCALE_FREQ_Hz))
#define	LOSSSPEED_DETECT_TIME			(4000)
#define LOSSSPEED_TYPE					(0X0040)

/****LossPhase Protection Param****/
#define DETECT_TIME_LP					(50)
#define DELAY_TIME_LP					(1000)
#define THRESHOLD_LP					(100)

/****Voltage Protection Param****/
#define UnderVoltage_Value				(17.2f)
#define	UNDERVOLTAGE_VALUE				ZxMath_F32toQ15(UnderVoltage_Value/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define	UNDERVOLTAGE_DETECT_TIME		(750)
#define OverVoltage_Value				(32.6f)
#define	OVERVOLTAGE_VALUE				ZxMath_F32toQ15(OverVoltage_Value/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define	OVERVOLTAGE_DETECT_TIME			(20)
#define UvRec_Value						(18.2f)
#define	UVREC_VALUE						ZxMath_F32toQ15(UvRec_Value/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define	UVREC_DETECT_TIME				(2000 - (STEADY_TIME_MS + CALIBRATION_TIME_MS))
#define	OvRec_Value						(31.6f)
#define	OVREC_VALUE						ZxMath_F32toQ15(OvRec_Value/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define	OVREC_DETECT_TIME				(2000 - (STEADY_TIME_MS + CALIBRATION_TIME_MS))

/****Diagnose Voltage Param****/
#define DiagVoltage_Min					(7.0f)
#define DIAGVOLTAGE_MIN					ZxMath_F32toQ15(DiagVoltage_Min/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define DiagVoltage_Max					(35.0f)
#define DIAGVOLTAGE_MAX					ZxMath_F32toQ15(DiagVoltage_Max/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define	DiagVoltage_Halt				(36.0f)
#define DIAGVOLTAGE_HALT				ZxMath_F32toQ15(DiagVoltage_Halt/USER_IQ_FULL_SCALE_VOLTAGE_V)
#define DIAGVOLTAGE_MAX_TIME			(2000)
#define DIAGVOLTAGE_HALT_TIME			(20)
#define REC_OVER_VOLTAG_TIME			(2000)

/****Fault Enable and Disable Param****/
#define FAULT_DETECT_ENABLE				(1)

#define DRYROTOR_DETECT_ENABLE			(1)
#define STALLROTOR_DETECT_ENABLE		(1)
#define BUSCURRENT_DETECT_ENABLE		(1)
#define TEMPERATURE_DETECT_ENABLE		(1)
#define LOSSSPEED_DETECT_ENABLE			(1)
#define LOSSPHASE_DETECT_ENABLE			(1)
#define VOLTAGE_DETECT_ENABLE  			(1)
#define DIAGVOLTAGE_DETECT_ENABLE		(1)
#define RECOVERY_FAULT_ENABLE			(1)
#define SWOc_ENABLE						(1)
/*Fault Recovery*/
#define RECOVERY_TIME_1st       		(1000)
#define RECOVERY_TIME_2nd       		(5000)
#define RECOVERY_TIME_3rd_10th  		(20000)
#define RECOVERY_TIME_11th_20th 		(120000)

/*Speed Adjustment*/

#define Duty_0							(0)
#define Duty_7							(0.07)
#define Duty_12							(0.12)
#define Duty_17							(0.17)
#define Duty_93							(0.93)
#define Duty_100						(1.0)
#define Hysteresis						(0.008)//0.005*32767

#define DUTY_0							(uint16_t)(Duty_0*32767)//0*32767
#define DUTY_7							(uint16_t)(Duty_7*32767)//0.07*32767
#define DUTY_12							(uint16_t)(Duty_12*32767)
#define DUTY_17							(uint16_t)(Duty_17*32767)
#define DUTY_93							(uint16_t)(Duty_93*32767)
#define DUTY_100						(uint16_t)(Duty_100*32767)
#define HYSTERESIS						(uint16_t)(Hysteresis*32767)//0.005*32767

#define SPEED_MAX						(5200.0f)
#define SPEED_MIN						(1500.0f)
#define FREQUENCY_MAX					ZxMath_F32toQ15(SPEED_MAX / USER_FULL_SCALE_SPEED_RPM)
#define FREQUENCY_MIN					ZxMath_F32toQ15(SPEED_MIN / USER_FULL_SCALE_SPEED_RPM)
#define FREQUENCY_K						((float)(FREQUENCY_MAX - FREQUENCY_MIN)/(DUTY_93 - DUTY_17))

#define CurrentThreshold_DryRotor		(1.0f)
#define CURRENT_DRYROTOR				ZxMath_F32toQ15(CurrentThreshold_DryRotor/USER_IQ_FULL_SCALE_CURRENT_A)
#define Speed_LoopSwitch   				(5147.0f)
#define SPEED_LOOPSWITCH    			ZxMath_F32toQ15(Speed_LoopSwitch/USER_FULL_SCALE_SPEED_RPM)
#define Speed_Return  			 		(50.0f)
#define SPEED_RETURN    				ZxMath_F32toQ15(Speed_Return/USER_FULL_SCALE_SPEED_RPM)
#define Speed_Limit   					(5600.0f)
#define SPEED_LIMIT    					ZxMath_F32toQ15(Speed_Limit/USER_FULL_SCALE_SPEED_RPM)
#define Current_LoopSwitch				(4.8f)
#define CURRENT_LOOPSWITCH				ZxMath_F32toQ15(Current_LoopSwitch / USER_IQ_FULL_SCALE_CURRENT_A)
#define ReturnCurrent					(1.0f)
#define RETURNCURRENT					ZxMath_F32toQ15(ReturnCurrent / USER_IQ_FULL_SCALE_CURRENT_A)

#define Current_Target					(6.0f)
#define Voltage_Rate					(24.0f)
#define POWER_TARGET					ZxMath_F32toQ15((Current_Target * Voltage_Rate) / (USER_IQ_FULL_SCALE_CURRENT_A * USER_IQ_FULL_SCALE_VOLTAGE_V))

/*Diagnose*/

#define SEND_TIME						(5)

#define T_HIGH_DRYROTOR					(1000)
#define T_LOW_DRYROTOR					(1000)

#define T_HIGH_StallOrOc				(1000)
#define T_LOW_StallOrOc					(1500)

#define T_HIGH_OT						(1000)
#define	T_LOW_OT						(2000)

#define T_HIGH_LossSpeed				(1000)
#define	T_LOW_LossSpeed					(2500)

#define T_HIGH_NoFault					(4500)
#define	T_LOW_NoFault					(500)

#endif  /* USER_COEFF_H */

