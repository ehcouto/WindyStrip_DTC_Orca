/**
 *  @file
 *
 *  @defgroup CLASS_B
 *
 *  @brief      API for the CRC8 module which calculates the standard CRC8
 *
 *  @copyright  Copyright 2007-2017. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef CRC8_H
	#define CRC8_H

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================


//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================
unsigned char Crc8(unsigned char init,unsigned char * buf, unsigned char size);
unsigned char Crc8__ProcessByte(unsigned char previews,unsigned char data);
#endif
