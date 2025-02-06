//******************************************************************************
// Company:   ALP'TRONIC
// -----------------------------------------------------------------------------
// Copyright: This software is ALP'TRONIC property.
//            Duplication or disclosure without ALP'TRONIC written authorization
//            is prohibited.
// -----------------------------------------------------------------------------
// Project:
// Language:  C
// -----------------------------------------------------------------------------
// Component: HardwareProfile
//
// -----------------------------------------------------------------------------
// Covered Requirements: (Implements: REQUIREMENT_REF.VERSION)
//******************************************************************************

//
// Rev 1.0   13/10/2016		CM
// Description
//
#ifndef __HARDWARE_PROFILE_H

	#define __HARDWARE_PROFILE_H
	
	#include "stm32f7xx_hal.h"

	extern CAN_HandleTypeDef hcan1;

#define cUNIQUE_BOOTLOADER_ID	0x01


//------------------------------------------------------------------------------
// Exported Macros
//
// #define LAYCmpMacroName(...)   (MacroDefinition)
//------------------------------------------------------------------------------


#define ClrWdg() {if (HAL_WWDG_Refresh(&hwwdg) != HAL_OK){Error_Handler();}}

//Peripheral defs





//**************************************************************
// CAN BUS
//**************************************************************
	typedef enum _TE_USED_CAN_LIST
	{
		SUPERVISOR_CAN = 0,

		NB_OF_USED_CANS //MUST BE END!!!
	}TE_USED_CAN_LIST;
	//CAN CONFIG

	// Can bus speed
	typedef enum _E_CAN_BUS_SPEED {

		CAN_BUS_SPEED_1MBPS = 0,
		CAN_BUS_SPEED_500KBPS,
		CAN_BUS_SPEED_250KBPS,
		CAN_BUS_SPEED_125KBPS,
		CAN_BUS_SPEED_100KBPS,

		NB_OF_CAN_BUS_SPEED_DEF//MUST BE END!!!
	} E_CAN_BUS_SPEED;

	//Order must correspond to E_CAN_BUS_SPEED, this table define the prescaler for CAN clk
	#define m_CAN_SPEED_TABLE {6, 12, 24,48,60}

	#define m_USED_CANS {&hcan1}

	#define NB_OF_APPS_USING_CAN_MAX	1
//--------------------------------------------------------------
// CAN BUS END
//--------------------------------------------------------------


//--------------------------------------------------------------
// Analog END
//--------------------------------------------------------------

//------------------------------------------------------------------------------
// Exported functions
//
// extern tReturnType LAYCmpFunctionName(tTypeArgument1 ArgumentName1, ... );
//------------------------------------------------------------------------------

#endif
