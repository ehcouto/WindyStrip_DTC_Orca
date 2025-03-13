/*
 * MclConfig.h
 *
 *  Created on: 13 apr 2018
 *      Author: lab.motori
 */

#ifndef MCL_MCLCONF_H_
#define MCL_MCLCONF_H_

#ifndef PI
    #define PI 3.1415926535897932384626433832795f
#endif


/* Mcl Configuration File */
/* Input Processing macroblock configuration -begin */
#define INPUTPROC__VDCFILTER_MINIMUM        DISABLED
#define INPUTPROC__VDC_RMS                  ENABLED
/* Input Processing macroblock configuration -end */


/* Observer macroblock configuration -begin */
#define OBS__VOLTAGE_PHASE_SHIFT                DISABLED
#define OBS__INVERTER_DISTORSION_COMPENSATION   DISABLED     // DISABLED --> inverter compensation feed-forward approach
                                                             // ENABLED  --> inverter compensation feedback approach)

#define OBS__RESISTANCE_ADAPTATION_RECOVERY     0.00008333f  // 0.08 ohm per second
#define OBS__RESISTANCE_ADAPTATION_INIT_VALUE   0.0f

#ifdef COMPILE_4_SIMULINK
#define OBS__ROTOR_MECH_POSITION        ENABLED
#define OBS__LOAD_ANGLE_CALCULATION     ENABLED
#else
#define OBS__ROTOR_MECH_POSITION        DISABLED
#define OBS__LOAD_ANGLE_CALCULATION     DISABLED
#endif


#define OBS__PLL_FILTERED_OMEGA         DISABLED
//#define OBS__PLL_FILTERED_OMEGA_COEF    0.25f       // value used in DD
#define OBS__PLL_ENGAGE_ERROR_THR       16.747f

/* Observer macroblock configuration -end */

/* DqRef macroblock configuration -begin */
#define DQREF__MIN_FLUX                 0.0f            //expressed as a percentage of the nominal flux
#define DQREF__OVERFLUX_CHARGING        0.001f         // flux increment rate (every control loop)  [percentage of the nominal]

#define DQREF__FLUX_ADAPTATION          DISABLED         // Enabled --> magnet flux adaptation during braking
#if (DQREF__FLUX_ADAPTATION == ENABLED)
#define DQREF__G_FLUX_ADAPT_GAIN        0.0002f*0.125f
#define DQREF__FLUX_PERC                0.3f
#define DQREF__FLUX_ADAPT_SAT_LOW       (1.0f-DQREF__FLUX_PERC)
#define DQREF__FLUX_ADAPT_SAT_HI        (1.0f+DQREF__FLUX_PERC)
#endif
/* DqRef macroblock configuration -end */



/* SpeedCtrl macroblock configuration -begin */
#define SPEEDCTRL__START_TORQUE_LIM             DISABLED
#define SPEEDCTRL__TORQUE_LIM_OUTPUT_FILTER     DISABLED
#define SPEEDCTRL__CURR_LIM_IDS_FILTER          DISABLED
#define SPEEDCTRL__SPEED_REGIONS				4
#define SPEEDCTRL__DEFAULT_TRANSITION_TIME_S   	0.25f	// seconds
#define SPEEDCTRL__DEFAULT_TRANSITION_TIME   	(sint32) (SPEEDCTRL__DEFAULT_TRANSITION_TIME_S/0.025f)
/* SpeedCtrl macroblock configuration -end */


/* Braking configuration -begin */
#define BRAKING_RESISTANCE_PERC         0.95f   //  [%]
#define ID_BRAKE_STEP                   0.05f   //  [A]
#define MIN_BRAKE_TORQUE                0.1f    //  [Nm]
#define MIN_BRAKE_TORQUE_THR            0.8f    //  [%] hysteresis threshold for minimum brake torque saturation
#define TMAX_BRAKING_NO_REQ_LOW_SPEED   0.2f    //  [Nm]
#define TMAX_BRAKING_NO_REQ_HIGH_SPEED  0.0f    //  [Nm]
#define BRAKING_SPEED_THR               1900.0f/60.0f*2.0f*PI //  [rad/s]
#define ID_BRAKING_PROP_GAIN            0.02f

#define BRAKING__CONSERVATIVE_VARIANT      // define it in case of conservative braking variant
/* Braking configuration -end */



/* Resistance estimation configuration -begin */
#define FAST_RESISTANCE_CALC    DISABLED  // alternative resistance estimation procedure -- it requires specific parameters
#define RESET_RESISTANCE_MANAGEMENT DISABLED
#define RESISTANCE_EST_ALIGNMENT_STEP_VALUE   2.0f*PI/3.0f
#define RESISTANCE_EST_ALIGNMENT_STEPS        3

#if (FAST_RESISTANCE_CALC == ENABLED)
    #ifndef COMPILE_4_SIMULINK
    #define R_COMP_FACTOR           0.15f   // it must be adjusted according to the inverter compensation strategy and resistance estimation procedure
    #else
    #define R_COMP_FACTOR           0.0f
    #endif
#endif
/* Resistance estimation configuration -end */

#define STOP_RAMP_DURING_INVERSION  ENABLED

#ifdef COMPILE_4_SIMULINK
    #ifndef PWM_DOUBLE_SLOT
        #define PWM_DOUBLE_SLOT
    #endif

#endif




/* Filter defines for floating point low pass filter implementation */
#define FILTER_FS_1KHZ_FC_3HZ   1.0f/(float32) (1<<6) //FILTER_SHIFT_FS_1KHZ_FC_3HZ
#define FILTER_FS_8KHZ_FC_956HZ 1.0f/(float32) (1<<1) //FILTER_SHIFT_FS_8KHZ_FC_956HZ
#define FILTER_FS_8KHZ_FC_10HZ  1.0f/(float32) (1<<7) //FILTER_SHIFT_FS_8KHZ_FC_10HZ
#define FILTER_FS_1KHZ_FC_5HZ   1.0f/(float32) (1<<5) //FILTER_SHIFT_FS_1KHZ_FC_5HZ


// Low Pass Filter coefficients //
#define EM_TORQUE_LPF_COEF          FILTER_FS_8KHZ_FC_956HZ
#define DC_BUS_LPF_COEF             FILTER_FS_1KHZ_FC_3HZ
#define OBS_SPEED_LPF_COEF          FILTER_FS_8KHZ_FC_10HZ
#define BRAKING_SPEED_LPF_COEF      FILTER_FS_1KHZ_FC_5HZ
#define CURR_LIM_IDS_LPF_COEF       0.125f
#define MAX_TORQUE_LIM_OUT_LPF_COEF 0.125f

// define it to enable the Electrical Power calculation
#define MCL__ELECTRICAL_POWER   ENABLED
#define MCL__EL_POWER_LPF 0.02f


// Thermal Model Support and operation //
#define   THERMAL_MODEL_EXE     ENABLED
//#define DISABLE_RESISTANCE_UPDATE     // uncomment to operate in open-loop mode
#endif /* MCL_MCLCONF_H_ */
