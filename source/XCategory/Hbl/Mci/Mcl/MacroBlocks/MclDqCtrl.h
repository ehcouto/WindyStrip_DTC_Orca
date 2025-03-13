/**
 *  @file
 *  @brief       Motor Control Loop 3-phase motors: DQ Reference Controllers Macro Block.
 *  @details     This module implements theDQ Reference Controllers  module.
 *  @author      alessio.beato/luigi.fagnano  (only temporary, since it is not integrated in MKS)
 *  $Header: FOC/Mcl.h 1.3 2015/11/05 11:53:55CET Marcelo Silva (SILVAMC) Exp  $
 * @copyright Copyright 2012 - $Date: 2015/11/05 11:53:55CET $. Whirlpool Corporation. All rights reserved � CONFIDENTIAL
*/
/*
 *
 *---------------------------------------------------------------------------------------------------------------------
 *---------------------------------------------------------------------------------------------------------------------
 */
#ifndef MCL_DQ_CTRL_H_
    #define MCL_DQ_CTRL_H_


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"
#include "MclCrossTypes.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef struct
{
    float32 Vd_Max;                                  //!< [V] - max d voltage allowed to the d PI
    uint32 Force_Gains_Starting_Time;               //!< [s xTs] - time to use default gains after starting
    float32 Phase_Advance_Coeff;                     //!< [rad/(rad/s)] - phase advance
    float32 m_index;                                 //!< modulation index limitation

    struct
    {
    	float32 torque_kp;                           //!< [V/Nm] - Torque controller proportional Kp
    	float32 torque_ki;                           //!< [Vs/Nm] - Torque controller integral Ki

        float32 flux_kp;                             //!< [V/Nm] - Torque controller proportional Kp
        float32 flux_ki;                             //!< [Vs/Nm] - Torque controller integral Ki

        float32 abs_speed;                           //!< [rpm] - define the limit between regions
        sint32 transition_time;                      //!< [x 25ms] - Time to transitate between regions
    } gains[3];


} MCL_DQ_CTRL_PARAMS_TYPE;
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void MclDqCtrl__ResetState(MCL_DQ_CTRL_IO_F_TYPE *io, MCL_DQ_CTRL_PARAMS_TYPE *params);
void MclDqCtrl__Initialize(MCL_DQ_CTRL_IO_F_TYPE *io, MCL_DQ_CTRL_PARAMS_TYPE *params);
void MclDqCtrl__OverrideRegGains(MCL_DQ_CTRL_PARAMS_TYPE *params);
void MclDqCtrl__RunningHandler(MCL_DQ_CTRL_IO_F_TYPE *io, MCL_DQ_CTRL_PARAMS_TYPE *params);
void MclDqCtrl__2ndRunningHandler(MCL_DQ_CTRL_IO_F_TYPE *io, MCL_DQ_CTRL_PARAMS_TYPE *params);
void MclDqCtrl__25msHandler(MCL_DQ_CTRL_IO_F_TYPE *io, MCL_DQ_CTRL_PARAMS_TYPE *params);


#endif // MCL_DQ_CTRL_H_
