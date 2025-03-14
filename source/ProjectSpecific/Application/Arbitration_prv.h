/**
 *  @file
 *  @brief      Private parameters for the Arbitration module.
 *
 *  $Header: Arbitration_prv.h 1.1 2015/02/05 14:48:47EST Ryan P Kujawski (KUJAWRP) Exp  $
 *
 *  @copyright  Copyright 2013 - $Date: 2015/02/05 14:48:47EST $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef ARBITRATION_PRV_H_
#define ARBITRATION_PRV_H_

//  --- Include Files -------------------------------------------------------------------------------------------------
#include "Log.h"

//=====================================================================================================================
//  --- Private Parameters --------------------------------------------------------------------------------------------
//=====================================================================================================================

//! Declaration for the Log module macros to identify this module.
#define MODULE_NAME MODULE_ARBITRATION

//! List of log message IDs that are generated by this module.
typedef enum MODULE_ARBITRATION_LOG_MESSAGE_ID_ENUM
{
    UNINITIALIZED_POINTER,
    INVALID_REQUESTOR
} MODULE_ARBITRATION_LOG_MESSAGE_ID_TYPE;

//=====================================================================================================================
//  --- Private Methods -----------------------------------------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 * The Arbitration module calls this macro if a NULL pointer is passed in place of the arbitration
 * control structure or in place of the action to any of the API functions.
 *
 * @param ptr = A pointer to the API function that was called improperly.
 */
#define DEBUG_UNINITIALIZED_POINTER(ptr)    LOG_ADD_EXCEPTION(UNINITIALIZED_POINTER, (uint16)((uint32)ptr & 0xFFFF))

//---------------------------------------------------------------------------------------------------------------------
/**
 * The Arbitration module calls this macro if an invalid requestor is passed to the
 * Arbitration__RequestAction() function or the Arbitration__ReleaseControl() function.
 *
 * @param requestor = The invalid requestor that was passed to the function.
 */
#define DEBUG_INVALID_REQUESTOR(requestor)  LOG_ADD_EXCEPTION(INVALID_REQUESTOR, requestor)

#endif  // ARBITRATION_PRV_H_
