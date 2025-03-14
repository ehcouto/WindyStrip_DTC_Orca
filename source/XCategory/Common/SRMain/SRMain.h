/**
 *  @defgroup   CLASS_B
 *  @file       SRMain.h
 *  @brief      Public interface to the Main module.
 *
 *  
 *
 *  @copyright  *****  Copyright 2008-2011.  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL  *****
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


#ifndef SRMAIN_H_INCLUDED
	#define SRMAIN_H_INCLUDED

#include "Micro.h"


//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void main(void);
INTERRUPT_DIRECTIVE void SRMain__TimerISR(void);


#endif                                              // MAIN_H_INCLUDED
