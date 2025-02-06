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
// Component: Bootloader
//******************************************************************************

// Rev 1.0    20/10/2017		CM
// Description
//


#ifndef _BOOTLOADER_H

	#define _BOOTLOADER_H (0)


//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//
// #include <system_file_name.h>
// #include "project_file_name.h"
//------------------------------------------------------------------------------
#include "global.h"


//------------------------------------------------------------------------------
// Data prefix for constant & data
//
// unsigned int {u8,u16,u32} / signed int {s8,s16,s32}
// register size unsigned int {r} / float {f32,f64}  / s for struct
// no prefix for other types.
//
//------------------------------------------------------------------------------
// Constant data
//
// #define cu16LAYCmpConstantName   ((U16) ConstantValueUL)
//------------------------------------------------------------------------------
#define cBOOTLOADER_VERSION_MAJOR	0x01
#define cBOOTLOADER_VERSION_MINOR	0x00

//------------------------------------------------------------------------------
// Exported Macros
//
// #define LAYCmpMacroName(...)   (MacroDefinition)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Exported type
//
// typedef Expression tLAYCmpTypeName;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Exported data
//
// extern tType   LAYCmpVariableName;
// extern tType*  pLAYCmpVariableName;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Constant exported data
//
// extern const tType  s8LAYCmpVariableName;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// CallBacks functions
//
// tReturnType LAYCmpFunctionName(tTypeArgument1 ArgumentName1, ... );
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Exported functions
//
// extern tReturnType LAYCmpFunctionName(tTypeArgument1 ArgumentName1, ... );
//------------------------------------------------------------------------------

	extern void Bootloader_Init(void);
	extern void Bootloader_Process(void);

#endif


