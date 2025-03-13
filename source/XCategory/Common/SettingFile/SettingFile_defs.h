/**
 *  @file
 *
 *  @brief      Cross category Class A definitions for the SettingFile module.
 *
 *  @copyright  Copyright 2015-2017. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef SETTINGFILE_DEFS_H_
#define SETTINGFILE_DEFS_H_

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

//! Cross category pointers that do not have any safety relevant data
#define SETTINGFILE_CROSS_CATEGORY_SF_PTR       \
    /* ACU */                                   \
    SF_PTR_ACU_MODIFIERS_EXTENDED    = 0x000E,  \
                                                \
    /* HMI */                                   \
    SF_PTR_UI_TOUCH_PARAMETERS       = 0x013C,

//! Standard displacements for the SF_PTR_MCU_CLASS_A_MCI setting file pointer
typedef enum SETTINGFILE_MCU_CLASS_A_MCI_DISPLACEMENT_ENUM
{
    SF_DISPL_MOTOR_CLASS_A_VERSION      = 0x00,
    SF_DISPL_MOTOR_PARAMS_DTC           = 0x01,
    SF_DISPL_MOTOR_PARAMS_CIM           = 0x02,
    SF_DISPL_MOTOR_PARAMS_LUT           = 0x03,
    SF_DISPL_MOTOR_SPEED_CTRL           = 0x04,
    SF_DISPL_MOTOR_PWM_MODULATOR        = 0x05,
    SF_DISPL_MOTOR_RS                   = 0x06,
    SF_DISPL_MOTOR_OTE                  = 0x07,
} SETTINGFILE_MCU_CLASS_A_MCI_DISPLACEMENT_TYPE;

//! Displacements for cross-product pointers (Non-safety relevant)
// New non-safety relevant cross category displacements must be added here
// New category specific safety relevant displacements must be in SettingFile_SRCat.h, else they can be added in SettingFile_prm.h
typedef enum SETTINGFILE_NON_SR_CROSS_PRODUCT_DISPLACEMENT_ENUM
{
    // Displacement numbers for SF_PTR_ACU_CROSS_PRODUCT_CONFIG
    SF_DISPL_ACU_EXPANSION_CONFIGURATION        = 0x04,
    SF_DISPL_LO_STATEMENT_DISPLACEMENT_SIZE     = 0x06,

    // Displacement numbers for SF_PTR_UI_PRODUCT_CONFIG
    SF_DISPL_FUNCTIONAL_TIMERS                  = 0x0E,

} SETTINGFILE_NON_SR_CROSS_PRODUCT_DISPLACEMENT_TYPE;

#endif // SETTINGFILE_DEFS_H_
