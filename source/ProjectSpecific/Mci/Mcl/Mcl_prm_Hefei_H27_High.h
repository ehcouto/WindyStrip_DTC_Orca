/**
 *  @file        Mcl_prm_Hefei_H27_High.h
 *  @brief       DTC Loop Parameters for 3-phase motors.
 *
 *               ==== AUTOMATIC GENERATED FILE! DO NOT TOUCH IT, USE EXCEL MACRO! ====
 *
 *  @details     This module defines all parameters of the DTC control loop for 3-phase motors. Created in 19/9/2018 16:42 
 *               Setting File Parameters for sBPM Hefei H27 High (M600)
 *               Generated from https://docs.google.com/a/whirlpool.com/spreadsheets/d/1b4qremaPN_pIFC-_Mqv2TJDgMwQxYunJ9vjfqv9II3Q/edit
 *               Generated by alessio_beato@whirlpool.com
 *  @copyright   Copyright 2018.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL
 *
 */

#ifndef MCL_PRM_H
#define MCL_PRM_H


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


#include "MciParametersLoader_prv.h"


#if (MCI_CLASS_A_PARAMETERS_LOADER_INTERNAL == ENABLED)
//DTC Phisical Motor Control Parameters

//Motor model parameters
//Stator Resistance = 2.47 Ohm
//Rotor flux  = 0.07756 Wb
//Maximum electromagnetic torque = 4 Nm
//Stator Direct Inductance (Ld) = 17.442376972437078 mH
//Stator Quadrature Inductance (Lq) = 24.129 mH
//Stator Inductance (Ls) = 19.57 mH
//Pole pairs = 4 NA

//Inductance Table
//Current [0] = 0 A
//Current [1] = 1 A
//Current [2] = 2 A
//Current [3] = 3 A
//Current [4] = 4 A
//Current [5] = 5 A
//Current [6] = 6 A
//Current [7] = 6 A
//Ld[0] = 17.5125768789166 mH
//Ld[1] = 17.44584531478684 mH
//Ld[2] = 17.30299959380647 mH
//Ld[3] = 16.674984095493606 mH
//Ld[4] = 15.88723197224117 mH
//Ld[5] = 15.143928718393486 mH
//Ld[6] = 14.449264324265052 mH
//Ld[7] = 14.449264324265052 mH
//Lq[0] = 23.805754368629426 mH
//Lq[1] = 24.132052393862118 mH
//Lq[2] = 24.06173278049456 mH
//Lq[3] = 23.001875680156225 mH
//Lq[4] = 21.704627718808492 mH
//Lq[5] = 20.56470437677534 mH
//Lq[6] = 19.573953184427225 mH
//Lq[7] = 19.573953184427225 mH
//Ldq Cross[0] = 0 mH
//Ldq Cross[1] = 0 mH
//Ldq Cross[2] = 0 mH
//Ldq Cross[3] = 0 mH
//Ldq Cross[4] = 0 mH
//Ldq Cross[5] = 0 mH
//Ldq Cross[6] = 0 mH
//Ldq Cross[7] = 0 mH

//Input Processing Parameters
//Maximum available voltage factor = 99.998 %

//Hardware (inverter) configuration
//Maximum peak phase current = 6 A
//Maximum peak phase current [high speed] = 4.1 A
//High Speed threshold = 6000 rpm

//Observer Parameters
//Stator Resistance adaptation limits = 15 %
//Stator Resistance adaptation gain = 160 Ohm/(Wb*s)
//Current threshold to enable resistance adaptation = 4.5 A
//Speed threshold to enable resistance adaptation = 40 rpm
//Observer G gain = 60 V/Wb
//Observer Integ G gain = 1000 V/Wb/s
//Observer Integ Limits = 0.5 V
//Observer Integ activation speed threshold = 400 rpm
//Minimum flux for zero speed = 0.01 Wb
//Phase advance coefficient (Feedback) = 0 NA
//Starting time threshold = 0.5 s
//PLL - Kp gain = 118 1/s
//PLL - Ki gain = 3410 1/s^2
//Observer call rate (sampling time) = 0.000125 s

//Speed Controller Parameters
//Maximum cosine of load angle = -0.26 NA
//Load angle controller Kp = 0 Nm
//Load angle controller Ki = 8000 Nm/s
//Time to force the use of default gains = 1 s
//Maximum starting torque (speed controller output) = 1.7 Nm
//Time to limit the starting torque = 1 s
//Braking Torque = 1 Nm
//Default Acceleration = 75 rpm/s
//Min Speed Default Acceleration = 15 rpm
//Braking Current = 3 A

//DQ Reference Generator Parameters
//Starting time = 1 s
//Maximum designed flux  = 95 %
//Speed threshold to enable/disable overflux = 375 rpm
//Percentage to be applied of extra flux. = 10 %
//K (torque error compensation) = 0.58 1/Nm
//Torque error compensation saturation = 0.5 NA
//Max Modulation Index(REF) = 98 %

//DQ Controllers Parameters
//Starting time to force gains = 1 s
//Phase advance coefficient = 1 NA
//Max Modulation Index (CTRL) = 115 %
//Pi Volt Margin for Vd_Max = 140 V
//Absolute speed threshold range - region[0] = 1905 rpm
//Absolute speed threshold range - region[1] = 14000 rpm
//Absolute speed threshold range - region[2] = 20000 rpm
//Torque controller Kp - region [0] = 10 V/Nm
//Torque controller Ki - region [0] = 2000 V/(Nm*s)
//Torque controller Kp - region [1] = 150 V/Nm
//Torque controller Ki - region [1] = 12000 V/(Nm*s)
//Torque controller Kp - region [2] = 150 V/Nm
//Torque controller Ki - region [2] = 12000 V/(Nm*s)
//Flux controller Kp - region [0] = 150 V/Wb
//Flux controller Ki - region [0] = 15000 V/(Wb*s)
//Flux controller Kp - region [1] = 8000 V/Wb
//Flux controller Ki - region [1] = 900000 V/(Wb*s)
//Flux controller Kp - region [2] = 8000 V/Wb
//Flux controller Ki - region [2] = 900000 V/(Wb*s)
//Time  to switch gains - region[0] = 2 s
//Time  to switch gains - region[1] = 3 s
//Time  to switch gains - region[2] = 3 s

//Pwm Parameters
//Speed threshold to disable compensation (applied 5% hysteresis) = 6000 rpm

//Starting (build flux) algoritm
//Delay time to keep the flux after ramp = 1 s
//Minimum current to build flux = 0.75 A
//Forced flux reference after build flux = 112 %

//Running algorithm
//Minimum speed (guard condition) to enter stopping = 228 rpm
//Minimum speed to switch off all IGBTs = 684 rpm
//Debouncer of running finished check = 5 NA
//Time to set all devices off below minimum speed = 1.8 s
//Time threshold for locked rotor at startup or in running = 4 s
//Starting debounce time to run with torque at its maximum = 3 s
//Running debounce time to run with torque at its maximum = 10 s
//Max Current for detecting locked rotor (% of max current) = 0.95 %

//Manual injection
//d current (Flux) controller Kp = 60 V/A
//d current (Flux) controller Ki = 26000 V/A/s
//q current (Torque) controller Kp = 60 V/A
//q current (Torque) controller Ki = 26000 V/A/s

//Resistance estimation algorithm
//Estimation timeout = 4 s
//Current level - 1st plateau = 4 A
//Current level - 2nd plateau = 5 A
//Current level - 3rd plateau = 1 A
//Time of 1st plateau = 0.6 s
//Time of 2nd plateau = 0.6 s
//Time of 3rd plateau = 0.4 s
//Voltage step for 1st plateau = 0.078125 V
//Voltage step for 2nd plateau = 0.078125 V
//Voltage step for 3nd plateau = 0.078125 V
//Oscillation check timer = 0.1 s
//Minimum number of samples to average = 300 NA
//Oscillation speed threshold = 30 rpm
//Time to reset resistance (declare resistance as outdated) = 6 min
//Resistance estimation call rate (sampling time) = 0.001 s
//Resistance estimation handler rate (sampling time) = 0.000125 s
//Spin speed to trigger resistance on next start up = 1710 rpm
//Time between measurement after reset = 1 min
//Number of estimation after reset = 3 NA
//Time between measurement under normal operation = 5 min
//Number of estimation in a row after spin = 3 NA
//Phase resistance calibrated at temperature = 2.47 Ohm
//Temperature coefficient = 232.1 °C/Ohm
//Base temperature = 25 °C
//Samples Number = 1024 NA

//MTPA Lut
//MTPATorque_0 = 0.064 Nm
//MTPATorque_1 = 0.296007142857142 Nm
//MTPATorque_2 = 0.528014285714285 Nm
//MTPATorque_3 = 0.760021428571428 Nm
//MTPATorque_4 = 0.992028571428571 Nm
//MTPATorque_5 = 1.22403571428571 Nm
//MTPATorque_6 = 1.45604285714285 Nm
//MTPATorque_7 = 1.68805 Nm
//MTPATorque_8 = 1.92005714285714 Nm
//MTPATorque_9 = 2.15206428571428 Nm
//MTPATorque_10 = 2.38407142857142 Nm
//MTPATorque_11 = 2.61607857142857 Nm
//MTPATorque_12 = 2.84808571428571 Nm
//MTPATorque_13 = 3.08009285714285 Nm
//MTPATorque_14 = 3.3121 Nm
//MTPATorque_15 = 3.3121 Nm
//MTPAFlux_0 = 0.0768 Wb
//MTPAFlux_1 = 0.0777000555144337 Wb
//MTPAFlux_2 = 0.0797001476559616 Wb
//MTPAFlux_3 = 0.0826003322259136 Wb
//MTPAFlux_4 = 0.0863005167958656 Wb
//MTPAFlux_5 = 0.090300645994832 Wb
//MTPAFlux_6 = 0.0946007751937984 Wb
//MTPAFlux_7 = 0.0989009043927648 Wb
//MTPAFlux_8 = 0.103199224806201 Wb
//MTPAFlux_9 = 0.107499261720191 Wb
//MTPAFlux_10 = 0.111699483204134 Wb
//MTPAFlux_11 = 0.1157996124031 Wb
//MTPAFlux_12 = 0.119199667774086 Wb
//MTPAFlux_13 = 0.124499833887043 Wb
//MTPAFlux_14 = 0.1299 Wb
//MTPAFlux_15 = 0.1299 Wb

//MTPV Lut
//MTPVFlux_0 = 0.0075 Wb
//MTPVFlux_1 = 0.0124571428571428 Wb
//MTPVFlux_2 = 0.0174142857142857 Wb
//MTPVFlux_3 = 0.0223714285714285 Wb
//MTPVFlux_4 = 0.0273285714285714 Wb
//MTPVFlux_5 = 0.0322857142857142 Wb
//MTPVFlux_6 = 0.0372428571428571 Wb
//MTPVFlux_7 = 0.0421999999999999 Wb
//MTPVFlux_8 = 0.0471571428571428 Wb
//MTPVFlux_9 = 0.0521142857142857 Wb
//MTPVFlux_10 = 0.0570714285714285 Wb
//MTPVFlux_11 = 0.0620285714285714 Wb
//MTPVFlux_12 = 0.0669857142857142 Wb
//MTPVFlux_13 = 0.0719428571428571 Wb
//MTPVFlux_14 = 0.0769 Wb
//MTPVFlux_15 = 0.0769 Wb
//MTPVTorque_0 = 0.2301 Nm
//MTPVTorque_1 = 0.382626530612244 Nm
//MTPVTorque_2 = 0.538085714285714 Nm
//MTPVTorque_3 = 0.71593469387755 Nm
//MTPVTorque_4 = 0.875879591836734 Nm
//MTPVTorque_5 = 1.03660612244897 Nm
//MTPVTorque_6 = 1.19816734693877 Nm
//MTPVTorque_7 = 1.36069999999999 Nm
//MTPVTorque_8 = 1.52425102040816 Nm
//MTPVTorque_9 = 1.68901836734693 Nm
//MTPVTorque_10 = 1.85506530612244 Nm
//MTPVTorque_11 = 2.02247142857142 Nm
//MTPVTorque_12 = 2.19131224489795 Nm
//MTPVTorque_13 = 2.36177551020408 Nm
//MTPVTorque_14 = 4 Nm
//MTPVTorque_15 = 4 Nm

//Speed Controller Parameters
//Speed threshold - range[0] = 70 rpm_Drum
//Speed threshold - range[1] = 1900 rpm_Drum
//Speed threshold - range[2] = 1950 rpm_Drum
//Speed threshold - range[3] = 0 rpm_Drum
//Speed Kp - range[0] = 4 Nmm/rpm
//Speed Ki - range[0] = 14 Nmm/(rpm s)
//Speed Kp - range[1] = 1.3 Nmm/rpm
//Speed Ki - range[1] = 6 Nmm/(rpm s)
//Speed Kp - range[2] = 0.5 Nmm/rpm
//Speed Ki - range[2] = 1.9 Nmm/(rpm s)
//Speed Kp - range[3] = 0 Nmm/rpm
//Speed Ki - range[3] = 0 Nmm/(rpm s)
//Speed gains time to switch - range [0] = 0.5 s
//Speed gains time to switch - range [1] = 0.5 s
//Speed gains time to switch - range [2] = 0.5 s
//Speed gains time to switch - range [3] = 2 s
//Transmission Ratio = 10 adm




signed long int Mcl_Params_SF[247] =
{
/*Max_Available_Voltage_Factor = = (Max_Available_Voltage_Factor/100)*/                     1065352880L,\
/*Stator_Resistance             = =Rs*/                     1075713147L,\
/*Stator_Resistance_Corr_Limits = =OBS_RESIS_ADAPT_LIM/100*/                     1041865114L,\
/*Stator_Resistance_Gain        = =OBSERVER_RESIS_ADAPT_GAIN*0.001*/                     1042536202L,\
/*Curr_Resistance_Threshold     = =OBS_RESIS_ADAPT_CURRENT_THRES*/                     1083179008L,\
/*Curr_Resistance_Spd_Threshold = =OBS_RESIS_ADAPT_SPEED_THRES/60*2*PI*/                     1082526354L,\
/*Phase_Advance_Coeff_Feedback = =PHASE_ADVANCE_COEF_FDBK *OBSERVER_TS*/                     0L,\
/*Rotor_Flux_Nominal            = =ROTOR_LAMBDAM*/                     1033820103L,\
/*Observer_G_gain = =OBSERVER_G_GAIN*/                     1114636288L,\
/*Observer_G_integ_gain = =OBSERVER_G_INTEG_GAIN*OBSERVER_TS*/                     1040187392L,\
/*Observer_G_integ_limits = =OBSERVER_G_INTEG_LIMITS*/                     1056964608L,\
/*Observer_G_integ_Spd_Threshold = =OBSERVER_G_INTEG_SPEED_THRES/60*2*pi()*/                     1109888310L,\
/*Min_Flux_Zero_Speed = =OBSERVER_MIN_FLUX_ZERO_SPEED*/                     1008981770L,\
/*Pole_Pairs = =(POLE_PAIRS)*/                     1082130432L,\
/*Starting_Threshold            = =(OBSERVER_STARTING_THRESHOLD*1000)*/                     500L,\
/*PLL_Kp                        = =OBSERVER_SPEED_PLL_KP*/                     1122762752L,\
/*PLL_Ki                        = =OBSERVER_SPEED_PLL_KI * (0.5*OBSERVER_TS)*/                     1046101361L,\
/*integ_k                       = =0.5*OBSERVER_TS*/                     948114031L,\
/*Vd_Max = =Rs * 1.5 * MAX_PHASE_CURR + PI_VOLT_MARGIN*/                     1126316769L,\
/*Force_Gains_Starting_Time = =((1000 * TIME_DQ_FORCE_GAINS_STARTING))*/                     1000L,\
/*Phase_Advance_Coeff = =PHASE_ADVANCE_COEFF * OBSERVER_TS*/                     956502639L,\
/*m_index = =M_INDEX_CTRL/100*/                     1066611507L,\
/*torque_kp[0] = =DQ_TORQUE_KP_REGION_0*/                     1092616192L,\
/*torque_ki[0] = =DQ_TORQUE_KI_REGION_0 * OBSERVER_TS*/                     1048576000L,\
/*flux_kp[0] = =DQ_FLUX_KP_REGION_0*/                     1125515264L,\
/*flux_ki[0] = =DQ_FLUX_KI_REGION_0*OBSERVER_TS*/                     1072693248L,\
/*abs_speed[0] = =DQ_SPEED_THRESHOLD_REGION_0*/                     1156456448L,\
/*transition_time[0] = =(DQ_GAINS_SWTICHING_TIME_REGION_0/0.025)*/                     80L,\
/*torque_kp[1] = =DQ_TORQUE_KP_REGION_1*/                     1125515264L,\
/*torque_ki[1] = =DQ_TORQUE_KI_REGION_1 * OBSERVER_TS*/                     1069547520L,\
/*flux_kp[1] = =DQ_FLUX_KP_REGION_1*/                     1174011904L,\
/*flux_ki[1] = =DQ_FLUX_KI_REGION_1 * OBSERVER_TS*/                     1122041856L,\
/*abs_speed[1] = =DQ_SPEED_THRESHOLD_REGION_1*/                     1180352512L,\
/*transition_time[1] = =(DQ_GAINS_SWTICHING_TIME_REGION_1/0.025)*/                     120L,\
/*torque_kp[2] = =DQ_TORQUE_KP_REGION_2*/                     1125515264L,\
/*torque_ki[2] = =DQ_TORQUE_KI_REGION_2*OBSERVER_TS*/                     1069547520L,\
/*flux_kp[2] = =DQ_FLUX_KP_REGION_2*/                     1174011904L,\
/*flux_ki[2] = =DQ_FLUX_KI_REGION_2*OBSERVER_TS*/                     1122041856L,\
/*abs_speed[2] = =DQ_SPEED_THRESHOLD_REGION_2*/                     1184645120L,\
/*transition_time[2] = =(DQ_GAINS_SWTICHING_TIME_REGION_2/0.025)*/                     120L,\
/*Rotor_Lambda_M = =ROTOR_LAMBDAM*/                     1033820103L,\
/*Max_Flux = =DQ_CONTROLLER_MAX_FLUX/100*((Ls/1000) * MAX_PHASE_CURR + ROTOR_LAMBDAM)*/                     1044229426L,\
/*Starting_Threshold = =((DQ_STARTING_TIME * 1000))*/                     1000L,\
/*Forced_Flux_Decrement = =2/32768*((Ls/1000) * MAX_PHASE_CURR + ROTOR_LAMBDAM+ 0.02)*/                     928785336L,\
/*Speed_Over_Flux_Threshold = =SPEED_OVERFULX_THRESHOLD*2*PI()/60*/                     1109202019L,\
/*Flux_Ref_Factor = =OVERFLUX_FACTOR/100*/                     1036831949L,\
/*K_Torque_Err = =K_TORQUE_ERROR*/                     1058306785L,\
/*Torque_Error_Sat = =TORQUE_ERROR_SAT*/                     1056964608L,\
/*m_index = =M_INDEX_REF/100*/                     1065017672L,\
/*K_torque_inv = =1/(1.5*POLE_PAIRS * ROTOR_LAMBDAM)*/                     1074366247L,\
/*Running.Min_Speed_Before_All_Devices_Off = =ALGO_RUNNING_MIN_SPEED_2_IGBTS_OFF/60*2*PI()*/                     1116684722L,\
/*Running.Min_Speed_Before_Stopping = =ALGO_RUNNING_MIN_SPEED_BEFORE_STOPPING/60*2*PI()*/                     1103036995L,\
/*Running.Debounce_Off_Running_Finished = =(ALGO_RUNNING_DEBOUNCER_2_IGBTS_OFF)*/                     5L,\
/*Running.Time_All_Dev_Below_Min_Speed = =(ALGO_RUNNING_TIME_IGBTS_OFF_BELOW_MIN_SPEED/0.025)*/                     72L,\
/*Locked_Rotor_Starting_Or_Running = =ROUND((ALGO_RUNNING_LOCKED_ROTOR_START_OR_RUNNING * 1000), 0)*/                     4000L,\
/*Locked_Rotor_Starting_Threshold = =ROUND((ALGO_RUNNING_LOCKED_ROTOR_STARTING_THRESHOLD / 0.005), 0)*/                     600L,\
/*Locked_Rotor_Running_Threshold = =ROUND((ALGO_RUNNING_LOCKED_ROTOR_RUNNING_THRESHOLD / 0.005), 0)*/                     2000L,\
/*Locked_Rotor_Current_Threshold = =(ALGO_RUNNING_LOCKED_ROTOR_CURRENT_THRESHOLD*MAX_PHASE_CURR)^2*3/2*/                     1111683236L,\
/*Build_Flux.Forced_Flux_Plateau_After_Ramp = =ROUND((ALGO_BUILD_FLUX_STARTING_TIME/OBSERVER_TS),0)*/                     8000L,\
/*Build_Flux.Min_Current_Build_Flux = =ALGO_BUILD_FLUX_MIN_CURRENT*/                     1061158912L,\
/*Build_Flux.Forced_Flux_Ref_After_Build_Flux = =(ALGO_BUILD_FLUX_RATIO_2_FORCE_FLUX/100)*ROTOR_LAMBDAM*/                     1035069294L,\
/*Build_Flux.Build_Flux_Increment = =2*((Ls/1000) * MAX_PHASE_CURR + ROTOR_LAMBDAM+ 0.02)/32768*/                     928785336L,\
/*timeout_threshold = =ROUND((ALGO_RES_EST_TIMEOUT_THRESHOLD * (1/ALGO_RES_EST_TS)),0)*/                     4000L,\
/*current_level_1st = =ALGO_RES_EST_CURR_1ST_PLATEAU*/                     1082130432L,\
/*current_level_2nd = =ALGO_RES_EST_CURR_2ND_PLATEAU*/                     1084227584L,\
/*current_level_3rd = =ALGO_RES_EST_CURR_3RD_PLATEAU*/                     1065353216L,\
/*time_1st_step = =ROUND(ALGO_RES_EST_1ST_PLATEAU_TIME/ALGO_RES_EST_TS,0)*/                     600L,\
/*time_2nd_step = =ROUND(ALGO_RES_EST_2ND_PLATEAU_TIME/ALGO_RES_EST_TS,0)*/                     600L,\
/*time_3rd_step = =ROUND(ALGO_RES_EST_3RD_PLATEAU_TIME/ALGO_RES_EST_TS,0)*/                     400L,\
/*delta_voltage_to_1st_step = =ALGO_RES_EST_VOLTAGE_STEP_1ST_PLATEAU*/                     1033895936L,\
/*delta_voltage_to_2nd_step = =ALGO_RES_EST_VOLTAGE_STEP_2ND_PLATEAU*/                     1033895936L,\
/*delta_voltage_to_3rd_step = =ALGO_RES_EST_VOLTAGE_STEP_3RD_PLATEAU*/                     1033895936L,\
/*oscillation_check_timer = =ROUND(ALGO_RES_EST_OSCILLATION_CHECK_TIME/ALGO_RES_EST_TS,0)*/                     100L,\
/*min_number_of_samples = =ROUND(ALGO_RES_EST_NUMBER_OF_SAMPLES,0)*/                     1133903872L,\
/*nominal_resistance = =Rs*/                     1075713147L,\
/*oscillation_speed_threshold = =ALGO_RES_EST_OSCILLATION_SPEED_THRESHOLD/60*2*PI()*/                     1078530011L,\
/*timer_to_reset_resistance = =ROUND((ALGO_RES_EST_TIME_2_OUTDATE_RESISTANCE * 60 / 0.025),0)*/                     14400L,\
/*timer_prescaler = =ROUND(ALGO_RES_EST_TS/ALGO_RES_EST_HANDLER_TS,0)-1*/                     7L,\
/*speed_threshold_to_estimate_resistance = =ROUND((ALGO_RES_EST_SPEED_TO_MEAS_AGAIN),0)*/                     1154859008L,\
/*time_estimate_resistance_after_reset = =ROUND((ALGO_RES_EST_TIME_BETWEEN_MEAS_AFTER_RESET*60)/0.025,0)*/                     2400L,\
/*number_estimation_after_reset = =ROUND(ALGO_RES_EST_NR_OF_ESTIMATION_AFTER_RESET,0)*/                     3L,\
/*time_estimate_resistance_normal = =ROUND((ALGO_RES_EST_TIME_BETWEEN_MEAS_NORMAL_OPERATION*60)/0.025,0)*/                     12000L,\
/*number_estimation_after_spin = =ROUND(ALGO_RES_EST_NR_OF_ESTIMATION_IN_ROW_AFTER_SPIN,0)*/                     3L,\
/*ResTempCalc.Rs0 = =ALGO_RES_EST_RS_FROM_BASE_TEMP*/                     1075713147L,\
/*ResTempCalc.Temp_Coeff = =ALGO_RES_EST_TEMP_COEFF*/                     1130895770L,\
/*ResTempCalc.Rs0_inv = =1/ALGO_RES_EST_RS_FROM_BASE_TEMP*/                     1053772182L,\
/*ResTempCalc.Temp_Coeff_inv = =1/ALGO_RES_EST_TEMP_COEFF*/                     999108151L,\
/*ResTempCalc.T0 = =ALGO_RES_EST_BASE_TEMP*/                     1103626240L,\
/*ResTempCalc.Samples_Number = =SAMPLES_NUMBER*/                     1024L,\
/*SpeedRef_Default_Accel = =SPEED_REF_DEFAULT_ACCEL*/                     1117126656L,\
/*SpeedRef_Min_Speed_Default_Accel = =SPEED_REF_MIN_SPEED_DEFAULT_ACCEL*/                     1097859072L,\
/*Manual_Injection.Vd_Max = =Rs * 1.5 * MAX_PHASE_CURR + PI_VOLT_MARGIN*/                     1126316769L,\
/*Force_Gains_Starting_Time = 0*/                     0L,\
/*Phase_Advance_Coeff = 0*/                     0L,\
/*m_index = =M_INDEX_CTRL/100*/                     1066611507L,\
/*Manual_Injection.gains[0].torque_kp = =Q_CURR_CONTROL_KP*/                     1114636288L,\
/*Manual_Injection.gains[0].torque_ki = =D_CURR_CONTROL_KI * OBSERVER_TS*/                     1078984704L,\
/*Manual_Injection.gains[0].flux_kp = =D_CURR_CONTROL_KP*/                     1114636288L,\
/*Manual_Injection.gains[0].flux_Ki = =D_CURR_CONTROL_KI * OBSERVER_TS*/                     1078984704L,\
/*abs_speed[0] = 0*/                     0L,\
/*transition_time[0] = 0*/                     0L,\
/*torque_kp[1] = 0*/                     0L,\
/*torque_ki[1] = 0*/                     0L,\
/*flux_kp[1] = 0*/                     0L,\
/*flux_ki[1] = 0*/                     0L,\
/*abs_speed[1] = 0*/                     0L,\
/*transition_time[1] = 0*/                     0L,\
/*torque_kp[2] = 0*/                     0L,\
/*torque_ki[2] = 0*/                     0L,\
/*flux_kp[2] = 0*/                     0L,\
/*flux_ki[2] = 0*/                     0L,\
/*abs_speed[2] = 0*/                     0L,\
/*transition_time[2] = 0*/                     0L,\
/*Speed_Threshold_InvComp = 6000*/                     1169915904L,\
/*sizeof_Ld_Lq_Lut = =7*/                     7L,\
/*step_inv = =1/(CURRENT_1-CURRENT_0)*/                     1065353216L,\
/*Ld_Lut_Current[0] = =CURRENT_0*/                     0L,\
/*Ld_Lut_[0] = =Ld_0/1000*/                     1016034953L,\
/*Ld_Lut_Current[1] = =CURRENT_1*/                     1065353216L,\
/*Ld_Lut_[1] = =Ld_1/1000*/                     1015999127L,\
/*Ld_Lut_Current[2] = =CURRENT_2*/                     1073741824L,\
/*Ld_Lut_[2] = =Ld_2/1000*/                     1015922437L,\
/*Ld_Lut_Current[3] = =CURRENT_3*/                     1077936128L,\
/*Ld_Lut_[3] = =Ld_3/1000*/                     1015585274L,\
/*Ld_Lut_Current[4] = =CURRENT_4*/                     1082130432L,\
/*Ld_Lut_[4] = =Ld_4/1000*/                     1015162353L,\
/*Ld_Lut_Current[5] = =CURRENT_5*/                     1084227584L,\
/*Ld_Lut_[5] = =Ld_5/1000*/                     1014505022L,\
/*Ld_Lut_Current[6] = =CURRENT_6*/                     1086324736L,\
/*Ld_Lut_[6] = =Ld_6/1000*/                     1013759131L,\
/*Ld_Lut_Current[7] = =CURRENT_7*/                     1086324736L,\
/*Ld_Lut_[7] = =Ld_7/1000*/                     1013759131L,\
/*Lq_Lut_Current[0] = =CURRENT_0*/                     0L,\
/*Lq_Lut_[0] = =Lq_0/1000*/                     1019413577L,\
/*Lq_Lut_Current[1] = =CURRENT_1*/                     1065353216L,\
/*Lq_Lut_[1] = =Lq_1/1000*/                     1019588757L,\
/*Lq_Lut_Current[2] = =CURRENT_2*/                     1073741824L,\
/*Lq_Lut_[2] = =Lq_2/1000*/                     1019551004L,\
/*Lq_Lut_Current[3] = =CURRENT_3*/                     1077936128L,\
/*Lq_Lut_[3] = =Lq_3/1000*/                     1018981998L,\
/*Lq_Lut_Current[4] = =CURRENT_4*/                     1082130432L,\
/*Lq_Lut_[4] = =Lq_4/1000*/                     1018285543L,\
/*Lq_Lut_Current[5] = =CURRENT_5*/                     1084227584L,\
/*Lq_Lut_[5] = =Lq_5/1000*/                     1017673552L,\
/*Lq_Lut_Current[6] = =CURRENT_6*/                     1086324736L,\
/*Lq_Lut_[6] = =Lq_6/1000*/                     1017141646L,\
/*Lq_Lut_Current[7] = =CURRENT_7*/                     1086324736L,\
/*Lq_Lut_[7] = =Lq_7/1000*/                     1017141646L,\
/*Max_Squared_Inv_Current  = =MAX_PHASE_CURR*MAX_PHASE_CURR*/                     1108344832L,\
/*Max_Design_Motor_Torque  = =TE_MAX*/                     1082130432L,\
/*Max_Starting_Torque      = =FORCE_MAX_STARTING_TORQUE*/                     1071225242L,\
/*Max_Starting_Torque_Time = =ROUND(((1000) * FORCE_MAX_STARTING_TIME),0)*/                     1000L,\
/*Rotor_Flux_Nominal = =ROTOR_LAMBDAM*/                     1033820103L,\
/*Ld                       = =Ld/1000*/                     1015997265L,\
/*Force_Default_Gains_Time = =ROUND(((1000) * TIME_FORCE_USE_DEFAULT_SPEED_GAINS),0)*/                     1000L,\
/*load_angle.Max_Cos = =LOAD_ANGLE_COS_MAX*/                     3196395192L,\
/*load_angle.Kp = =LOAD_ANGLE_KP*/                     0L,\
/*load_angle.Ki = =LOAD_ANGLE_KI * OBSERVER_TS*/                     1065353216L,\
/*speed_gains_table[0].kp = =SPEED_KP_REGION_0*60/(2*PI())/1000*/                     1025275047L,\
/*speed_gains_table[0].ki = =SPEED_KI_REGION_0 * OBSERVER_TS*60/(2*PI())/1000*/                     931933997L,\
/*speed_gains_table[0].speed_abs = =SPEED_THRESHOLD_REGION_0*TRANSMISSION RATIO*/                     1143930880L,\
/*speed_gains_table[0].transtion_time = =ROUND(SPEED_TIME_TO_SWITCH_REGION_0/0.025,0)*/                     20L,\
/*speed_gains_table[1].kp = =SPEED_KP_REGION_1*60/(2*PI())/1000*/                     1011573875L,\
/*speed_gains_table[1].ki = =SPEED_KI_REGION_1 * OBSERVER_TS*60/(2*PI())/1000*/                     921719008L,\
/*speed_gains_table[1].speed_abs = =SPEED_THRESHOLD_REGION_1*TRANSMISSION RATIO*/                     1184133120L,\
/*speed_gains_table[1].transtion_time = =ROUND(SPEED_TIME_TO_SWITCH_REGION_1/0.025,0)*/                     20L,\
/*speed_gains_table[2].kp = =SPEED_KP_REGION_2*60/(2*PI())/1000*/                     1000109223L,\
/*speed_gains_table[2].ki = =SPEED_KI_REGION_2 * OBSERVER_TS*60/(2*PI())/1000*/                     907555641L,\
/*speed_gains_table[2].speed_abs = =SPEED_THRESHOLD_REGION_2*TRANSMISSION RATIO*/                     1184389120L,\
/*speed_gains_table[2].transtion_time = =ROUND(SPEED_TIME_TO_SWITCH_REGION_2/0.025,0)*/                     20L,\
/*speed_gains_table[3].kp = =SPEED_KP_REGION_3*60/(2*PI())/1000*/                     0L,\
/*speed_gains_table[3].ki = =SPEED_KI_REGION_3 * OBSERVER_TS/1000*/                     0L,\
/*speed_gains_table[3].speed_abs = =SPEED_THRESHOLD_REGION_3*TRANSMISSION RATIO*/                     0L,\
/*speed_gains_table[3].transtion_time = =ROUND(SPEED_TIME_TO_SWITCH_REGION_3/0.025,0)*/                     80L,\
/*Max_Braking_Current = = BRAKING_CURRENT*/                     1077936128L,\
/*Max_Braking_Torque = =MAX_BRAKING_TORQUE*/                     1065353216L,\
/*Max_Squared_Inv_Current_High_Speed  = =MAX_PHASE_CURRENT_HIGH_SPEED*MAX_PHASE_CURRENT_HIGH_SPEED*/                     1099332321L,\
/*High_Speed_Thr_Max_Current = =SPEED_THRESHOLD_MAX_PHASE_CURRENT*/                     1169915904L,\
/*pole_pairs_factor = =1.5*POLE_PAIRS*/                     1086324736L,\
/*sizeof_lut  = 15*/                     15L,\
/*step_inv = =1/(MTPATorque_1-MTPATorque_0)*/                     1082780994L,\
/*Mtpa_Torque_0 = = MTPATorque_0*/                     1032000111L,\
/*Mtpa_Flux_0 = = MTPAFlux_0*/                     1033718098L,\
/*Mtpa_Torque_1 = = MTPATorque_1*/                     1050119744L,\
/*Mtpa_Flux_1 = = MTPAFlux_1*/                     1033838901L,\
/*Mtpa_Torque_2 = = MTPATorque_2*/                     1057434610L,\
/*Mtpa_Flux_2 = = MTPAFlux_2*/                     1034107349L,\
/*Mtpa_Torque_3 = = MTPATorque_3*/                     1061327044L,\
/*Mtpa_Flux_3 = = MTPAFlux_3*/                     1034496605L,\
/*Mtpa_Torque_4 = = MTPATorque_4*/                     1065219478L,\
/*Mtpa_Flux_4 = = MTPAFlux_4*/                     1034993235L,\
/*Mtpa_Torque_5 = = MTPATorque_5*/                     1067232564L,\
/*Mtpa_Flux_5 = = MTPAFlux_5*/                     1035530124L,\
/*Mtpa_Torque_6 = = MTPATorque_6*/                     1069178781L,\
/*Mtpa_Flux_6 = = MTPAFlux_6*/                     1036107277L,\
/*Mtpa_Torque_7 = = MTPATorque_7*/                     1071124998L,\
/*Mtpa_Flux_7 = = MTPAFlux_7*/                     1036684431L,\
/*Mtpa_Torque_8 = = MTPATorque_8*/                     1073071215L,\
/*Mtpa_Flux_8 = = MTPAFlux_8*/                     1037261341L,\
/*Mtpa_Torque_9 = = MTPATorque_9*/                     1074379628L,\
/*Mtpa_Flux_9 = = MTPAFlux_9*/                     1037838483L,\
/*Mtpa_Torque_10 = = MTPATorque_10*/                     1075352736L,\
/*Mtpa_Flux_10 = = MTPAFlux_10*/                     1038402227L,\
/*Mtpa_Torque_11 = = MTPATorque_11*/                     1076325845L,\
/*Mtpa_Flux_11 = = MTPAFlux_11*/                     1038952537L,\
/*Mtpa_Torque_12 = = MTPATorque_12*/                     1077298953L,\
/*Mtpa_Flux_12 = = MTPAFlux_12*/                     1039408885L,\
/*Mtpa_Torque_13 = = MTPATorque_13*/                     1078272062L,\
/*Mtpa_Flux_13 = = MTPAFlux_13*/                     1040120261L,\
/*Mtpa_Torque_14 = = MTPATorque_14*/                     1079245170L,\
/*Mtpa_Flux_14 = = MTPAFlux_14*/                     1040516225L,\
/*Mtpa_Torque_15 = = MTPATorque_15*/                     1079245170L,\
/*Mtpa_Flux_15 = = MTPAFlux_15*/                     1040516225L,\
/*sizeof_mtpv = 15*/                     15L,\
/*step_inv = =1/(MTPVFlux_1-MTPVFlux_0)*/                     1128905383L,\
/*Mtpv_Flux_0 = = MTPVFlux_0*/                     1005961871L,\
/*Mtpv_Torque_0 = =MTPVTorque_0*/                     1047240534L,\
/*Mtpv_Flux_1 = = MTPVFlux_1*/                     1011620107L,\
/*Mtpv_Torque_1 = =MTPVTorque_1*/                     1053026208L,\
/*Mtpv_Flux_2 = = MTPVFlux_2*/                     1015982183L,\
/*Mtpv_Torque_2 = =MTPVTorque_2*/                     1057603580L,\
/*Mtpv_Flux_3 = = MTPVFlux_3*/                     1018643529L,\
/*Mtpv_Torque_3 = =MTPVTorque_3*/                     1060587391L,\
/*Mtpv_Flux_4 = = MTPVFlux_4*/                     1021304875L,\
/*Mtpv_Torque_4 = =MTPVTorque_4*/                     1063270821L,\
/*Mtpv_Flux_5 = = MTPVFlux_5*/                     1023688198L,\
/*Mtpv_Torque_5 = =MTPVTorque_5*/                     1065660290L,\
/*Mtpv_Flux_6 = = MTPVFlux_6*/                     1025018871L,\
/*Mtpv_Torque_6 = =MTPVTorque_6*/                     1067015564L,\
/*Mtpv_Flux_7 = = MTPVFlux_7*/                     1026349544L,\
/*Mtpv_Torque_7 = =MTPVTorque_7*/                     1068378987L,\
/*Mtpv_Flux_8 = = MTPVFlux_8*/                     1027680217L,\
/*Mtpv_Torque_8 = =MTPVTorque_8*/                     1069750952L,\
/*Mtpv_Flux_9 = = MTPVFlux_9*/                     1029010890L,\
/*Mtpv_Torque_9 = =MTPVTorque_9*/                     1071133121L,\
/*Mtpv_Flux_10 = = MTPVFlux_9*/                     1030341563L,\
/*Mtpv_Torque_10 = =MTPVTorque_9*/                     1072526024L,\
/*Mtpv_Flux_11 = = MTPVFlux_10*/                     1031672236L,\
/*Mtpv_Torque_11 = =MTPVTorque_10*/                     1073836076L,\
/*Mtpv_Flux_12 = = MTPVFlux_10*/                     1032400846L,\
/*Mtpv_Torque_12 = =MTPVTorque_10*/                     1074544246L,\
/*Mtpv_Flux_13 = = MTPVFlux_10*/                     1033066183L,\
/*Mtpv_Torque_13 = =MTPVTorque_10*/                     1075259220L,\
/*Mtpv_Flux_14 = = MTPVFlux_10*/                     1033731519L,\
/*Mtpv_Torque_14 = =MTPVTorque_10*/                     1082130432L,\
/*Mtpv_Flux_15 = = MTPVFlux_10*/                     1033731519L,\
/*Mtpv_Torque_15 = =MTPVTorque_10*/                     1082130432L,\
};


#endif //


#endif //MCL_PRM_H
