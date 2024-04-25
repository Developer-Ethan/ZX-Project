/**************************************************************************************************/
/**
 * @file      : user_pmsm.h
 * @brief     : user_pmsm demo main header file.
 *              - Platform: Z20K11xM
 * @version   : Demo
 * @date      : 27-Feb-2023
 * @author    : Zhixin Semiconductor
 * @note      : None
 *
 * @copyright : Copyright (c) 2022-2023 Zhixin Semiconductor Ltd. All rights reserved.
 **************************************************************************************************/
#ifndef USER_PMSM_H
#define USER_PMSM_H



//! \brief BASE VALUE FOR PER UNIT CALCULATION
// **************************************************************************
//! \brief Defines the full scale frequency for IQ variable, Hz
//! \brief All frequencies are converted into (pu) based on the ratio to this value
//! \brief this value MUST be larger than the maximum speed that you are expecting from the motor
#define USER_IQ_FULL_SCALE_FREQ_Hz            (400.0f)

//! \brief Defines full scale value for the IQ30 variable of Voltage inside the system
//! \brief All voltages are converted into (pu) based on the ratio to this value
#define USER_IQ_FULL_SCALE_VOLTAGE_V          (40.0f)

//! \brief Defines the full scale current for the IQ variables, A
//! \brief All currents are converted into (pu) based on the ratio to this value
//! \brief WARNING: this value MUST be larger than the maximum current readings that you are expecting from the motor or the reading will roll over to 0, creating a control issue
#define USER_IQ_FULL_SCALE_CURRENT_A          (30.0f)

  
#define USER_IQ_FULL_SCALE_FLUX_WB            (0.02f)




//! \brief POLES
// **************************************************************************
//! \brief Defines the analog voltage filter pole location, rad/s
//! \brief Must match the hardware filter
#define USER_VOLTAGE_FILTER_POLE_rps    (2340.66f)

//! \brief Defines the software pole location for the DC bus filter, rad/s
//! \brief Normally it should not be changed
#define USER_DCBUS_POLE_rps             (120.0f)   // 100.0 Default

//! \brief Defines the software pole location for the direction filter, rad/s
//! \brief Normally it should not be changed
#define USER_DIRECTION_POLE_rps         (6.0f)     // 6.0 Default

//! \brief Defines the software pole location for the temperature filter, rad/s
//! \brief Normally it should not be changed
#define USER_TEMPERATURE_POLE_rps       (100.0f)     // 6.0 Default


//! \brief LIMITS
// **************************************************************************
//! \brief Defines the maximum current allowed for this solution
//! \brief If the current is greater than this value, the system should shut down to prevent damage.
#define USER_MAX_ALLOWED_CURRENT_PU           (ZxMath_F32toQ15(0.8))

//! \brief Defines the lowest bus voltage allowed for this solution
//! \brief If the voltage is lower than this value, the system should shut down to prevent damage.
#define USER_LOWEST_ALLOWED_VOLTAGE_PU        (ZxMath_F32toQ15(0.7))

//! \brief Defines the highest bus voltage allowed for this solution
//! \brief If the voltage is higher than this value, the system should shut down to prevent damage.
#define USER_HIGHEST_ALLOWED_VOLTAGE_PU       (ZxMath_F32toQ15(0.9))

//! \brief Defines the maximum Voltage vector (Vs) magnitude allowed.  This value sets the maximum magnitude for the output of the
//! \brief Id and Iq PI current controllers.  The Id and Iq current controller outputs are Vd and Vq.
//! \brief The relationship between Vs, Vd, and Vq is:  Vs = sqrt(Vd^2 + Vq^2).  In this FOC controller, the
//! \brief Vd value is set equal to USER_MAX_VS_MAG*USER_VD_MAG_FACTOR.  Vq = sqrt(USER_MAX_VS_MAG^2 - Vd^2).
//! \brief Set USER_MAX_VS_MAG = 0.5 for a pure sine-wave with a peak at SQRT(3)/2 = 86.6% duty cycle.  No current reconstruction is needed for this condition.
//! \brief Set USER_MAX_VS_MAG = 1/SQRT(3) = 0.5774 for a pure sine-wave with a peak at 100% duty cycle.  Current reconstruction will be needed for this condition.
//! \brief Set USER_MAX_VS_MAG = 2/3 = 0.6666 to create a trapezoidal voltage waveform.  Current reconstruction will be needed for this condition.
#define USER_MAX_VS_MAG                    (0.5774)

//! \brief Defines the total vector scale factor 
//! \brief The 0.97 scale is used for One shunt resistor sample,0.98 is used for three shunts

#define USER_VS_SF                            (0.98f) 


//! \brief Defines the direct voltage (Vd) scale factor
#define USER_VD_SF                         (0.97f) 

//! \brief Defines the pll out limit value
//! \brief Est max output value.
#define USER_MAX_PLLOUT_PU       (ZxMath_F32toQ15(0.98f))
//! \brief Ext min output value.
#define USER_MIN_PLLOUT_PU       (ZxMath_F32toQ15(-0.98f))

//! \brief MOTOR RELATED
// **************************************************************************
#define USER_MOTOR_TYPE                   MOTOR_Type_Pm
#define USER_MOTOR_NUM_POLE_PAIRS         (2)
#define USER_MOTOR_Rr                     (NULL)
#define USER_MOTOR_Rs                     (0.2f)          // Ohm one phase to center resistance 0.2055f
#define USER_MOTOR_Ls_d                   (0.383*0.8f)         // mH 3.75
#define USER_MOTOR_Ls_q                   (0.383*0.8f)         // mH  6.51
#define USER_MOTOR_ROTOR_FLUX             (0.00938f)       // Wb or V*s/rad      Kt=p*phi*3/2
#define USER_MOTOR_MAX_CURRENT            (18.0f)          // A peak 8.5A
#define USER_SYSTEM_INERTIA               (0.000028f)      // kg*m2
#define USER_MAX_NEGATIVE_ID_CURRENT_A    (-15)            // Defines the maximum negative current to be applied in Id reference

#endif  /* USER_PMSM_H */

