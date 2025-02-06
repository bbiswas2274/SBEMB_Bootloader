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
// Component: UserCan
//******************************************************************************

// Rev 1.0    20/10/2017		CM
// Description
//


#ifndef _USER_CAN_H

	#define _USER_CAN_H (0)


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

	typedef struct _TS_USER_CAN_ID
	{
		uns32 u32_Id;
		uns32 IDE; /*!< Specifies the type of identifier for the message  @ref CAN_identifier_type */
	}TS_USER_CAN_ID;

	typedef struct
	{
		uns8 DLC;
		TS_USER_CAN_ID st_CanId;

		uns8 Data[8];
	}CanMsgTypeDef;






	/* Callback type for CAN Frame reception */
	typedef BOOL (*UserCanRxFrameCB)(CanMsgTypeDef *CanMsg);
	/* Callback type for CAN Frame Transmission */
	typedef void (*UserCanTxFrameCB)(TS_USER_CAN_ID ts_CanIds, BOOL b_succeed);


	typedef BOOL (*UserCanRxFramCBForCpp) (void* object, CanMsgTypeDef* CanMsg);
	typedef void (*UserCanTxFramCBForCpp)(void* object, TS_USER_CAN_ID ts_CanIds, BOOL b_succeed);




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
#ifdef __cplusplus
 extern "C" {
#endif
	extern void UserCan_Init(void);
	extern void UserCan_Start(uns8 u8_UserCanIndex);
	extern void UserCan_Stop(uns8 u8_UserCanIndex);
	extern void UserCan_Restart(uns8 u8_UserCanIndex);

	extern void UserCan_SetCanBusSpeed(TE_USED_CAN_LIST e_CanBusId, E_CAN_BUS_SPEED e_CanSpeed);


	extern uns8 UserCan_CreateHandle(TE_USED_CAN_LIST e_CanBusId);
	extern BOOL UserCan_SendCanFrame(uns8 u8_UserCanIndex, CanMsgTypeDef st_TxMessage);
	extern BOOL UserCan_CheckFreeSpaceInTxBuffer(uns8 u8_UserCanIndex);

	extern BOOL UserCan_registerRXCallbackForCpp(uns8 u8_UserCanIndex, void* obj, UserCanRxFramCBForCpp cb);
    extern BOOL UserCan_registerTXCallbackForCpp(uns8 u8_UserCanIndex, void* obj, UserCanTxFramCBForCpp cb);

	extern BOOL UserCan_registerRXCallback(uns8 u8_UserCanIndex, UserCanRxFrameCB cb);
    extern BOOL UserCan_registerTXCallback(uns8 u8_UserCanIndex, UserCanTxFrameCB cb);

#ifdef __cplusplus
 }
#endif
#endif


