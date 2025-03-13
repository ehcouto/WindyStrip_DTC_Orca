/**
 * @file
 * @brief       Public interface to the API010PollVar module.
 *
 * @details     Refer to the API010PollVar.c source file for more detailed information.
 *
 * @copyright   Copyright 2014-2017 Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */

#ifndef API010POLLVAR_H_
#define API010POLLVAR_H_

#if (API010POLLVAR_FEATURE == ENABLED)


//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --
#include "Reveal.h"


//=====================================================================================================================
//  --- Public Properties ---------------------------------------------------------------------------------------------
//=====================================================================================================================


// -- Public Constant Declarations --

//! A constant used to identify the API implemented by this module.
#define API010POLLVAR_NUM       10
#define API010POLLVAR_TYPE      4
#define API010POLLVAR_VERSION   3
#define API010POLLVAR_INSTANCES 1


// -- Public Enumerated Constant Declarations --


// -- Public Type Declarations --


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================


void API010PollVar__Initialize(void);
REVEAL_RECIPE_STATUS_TYPE API010PollVar__CommandParser(REVEAL_MSG_TYPE * buffer);
void API010PollVar__AckParser(uint8 address,uint8 api,uint8 opcode, REVEAL_RECIPE_STATUS_TYPE reasoncode);
void API010PollVar__FastHandler(void);
void API010PollVar__SlowHandler(void);


#endif      // #if (API010POLLVAR_FEATURE == ENABLED)
#endif      // API010POLLVAR_H_
