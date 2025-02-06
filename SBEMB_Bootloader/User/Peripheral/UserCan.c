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

//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//
// #include <system_file_name.h>
// #include "project_file_name.h"
//------------------------------------------------------------------------------

#include "UserCan.h"


//------------------------------------------------------------------------------
// Local constants
//
// #define cConstantName   ((tType) ConstantValue)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local macros
//
// #define mMacroName	(MacroDefinition)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local types
//
// typedef Expression	 tTypeName;
//------------------------------------------------------------------------------

	typedef struct _ST_USER_CAN_HANDLE
	{
		TS_USER_CAN_ID _tst_MailBoxesIds[3];
		uns8 _tu8_MailBoxUser;
	}ST_USER_CAN_HANDLE;

	typedef struct _ST_LOC_USER_CAN_HANDLE
	{
		uns8 u8_CanIndex;

		UserCanRxFrameCB UserCanRXFrameCallback;
		UserCanTxFrameCB UserCanTXFrameCallback;

		UserCanRxFramCBForCpp UserCanRXFrameCallbackCpp;
		UserCanTxFramCBForCpp UserCanTXFrameCallbackCpp;
		void* obj;

		BOOL b_CanRunning;
	}ST_LOC_USER_CAN_HANDLE;
//------------------------------------------------------------------------------
// Data prefix
//
// unsigned int {u8,u16,u32} / signed int {s8,s16,s32}
// register size unsigned int {r} / float {f32,f64}  / s for struct
//
//------------------------------------------------------------------------------
// Local data
//
// static tType   u8VariableName;
// static tType*  pu8VariableName;
//------------------------------------------------------------------------------
static CAN_HandleTypeDef * pst_CanHandle[NB_OF_USED_CANS] = m_USED_CANS;
static uns8 _tu8_CanBusSpeedTable[NB_OF_CAN_BUS_SPEED_DEF] = m_CAN_SPEED_TABLE;
static ST_USER_CAN_HANDLE st_UserCanH[NB_OF_USED_CANS];
static ST_LOC_USER_CAN_HANDLE pst_LocUserCans[NB_OF_APPS_USING_CAN_MAX];
static uns8 u8_IndexInitLocUserCans = 0;

//------------------------------------------------------------------------------
// Constant local data
//
// static const tType  VariableName;
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Exported data
//
// tType   u8LAYCmpVariableName; (LAY: 3 characters to identify the layer)
// tType*  pu8LAYCmpVariableName;(Cmp: 3 characters to identify the component)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Constant exported data
//
//   (LAY: 3 characters to identify the layer)
//   (Cmp: 3 characters to identify this component)
//
// const tType	 LAYCmpVariableName;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local function prototypes
//
// static tTypeRetour FunctionName(tTypeArgument1 ArgumentName1, ... );
//------------------------------------------------------------------------------
BOOL User_CAN_notifyRXFrameReceived(uns8 u8_UserCanIndex, CanMsgTypeDef *CanMsg);
void User_CAN_notifyTXFrameSent(uns8 u8_UserCanIndex, TS_USER_CAN_ID ts_mailBox, BOOL b_succeed);

//==============================================================================
//=========================== LOCAL FUNCTIONS ==================================
//==============================================================================


uns8 _UserCan_FindCanBusIndex(CAN_HandleTypeDef *hcan)
{
	for (uns8 u8_TmpVal = 0; u8_TmpVal < NB_OF_USED_CANS;u8_TmpVal++)
	{
		if(pst_CanHandle[u8_TmpVal] == hcan) return u8_TmpVal;
	}

	//_Error_Handler(__FILE__, __LINE__);

	return 255;
}

//==============================================================================
// DESCRIPTION : HAL_CAN_notifyRXFrameReceived
// Notify registered listeners of a new Rx Frame
// Notify listeners till the linked listener is found
//
// PARAMETERS :
// CanMsg (CanMsgTypeDef *, IN) : Received Message
//
// RETURN VALUE : None
//==============================================================================
BOOL User_CAN_notifyRXFrameReceived(uns8 u8_UserCanIndex, CanMsgTypeDef *CanMsg)
{
	BOOL b_FrameLinked = FALSE;

	if (pst_LocUserCans[u8_UserCanIndex].obj != NULL)
	{
		if(pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallbackCpp != NULL)
				b_FrameLinked = pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallbackCpp(pst_LocUserCans[u8_UserCanIndex].obj, CanMsg);
	}
	else
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallback != NULL)
			b_FrameLinked = pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallback(CanMsg);
	}

	return b_FrameLinked;
}

//==============================================================================
// DESCRIPTION : HAL_CAN_notifyTXFrameSent
// Notify registered listeners of the CAN frame sending result
// To modify : only sender should be notified
//
// PARAMETERS :
// ts_mailBox (TS_MAIL_BOXES_IDS, IN) : ID of the Sent Message
// b_succeed (BOOL, IN) : Result of the transmission True if message correctly sent
//
// RETURN VALUE : None
//==============================================================================
void User_CAN_notifyTXFrameSent(uns8 u8_UserCanIndex, TS_USER_CAN_ID ts_mailBox, BOOL b_succeed)
{
	if (pst_LocUserCans[u8_UserCanIndex].obj != NULL)
	{
		if(pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallbackCpp != NULL)
				pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallbackCpp(pst_LocUserCans[u8_UserCanIndex].obj, ts_mailBox, b_succeed);
	}
	else
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallback != NULL)
			pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallback(ts_mailBox, b_succeed);
	}


}


//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef	RxHeader;
	CanMsgTypeDef st_CanMsg;

	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);


	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, st_CanMsg.Data);
	__HAL_CAN_DISABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	for (uns8 u8_UserCan = 0; (u8_UserCan < u8_IndexInitLocUserCans); u8_UserCan++)
	{
		if(pst_LocUserCans[u8_UserCan].u8_CanIndex != 255)
		{
			if(pst_LocUserCans[u8_UserCan].u8_CanIndex == u8_CanBusIndex)
			{
				__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

				st_CanMsg.DLC =  RxHeader.DLC;
				if(RxHeader.IDE == CAN_ID_STD)
					st_CanMsg.st_CanId.u32_Id = RxHeader.StdId;
				else
					st_CanMsg.st_CanId.u32_Id = RxHeader.ExtId;

				st_CanMsg.st_CanId.IDE = RxHeader.IDE;

				if(User_CAN_notifyRXFrameReceived(u8_UserCan,&st_CanMsg))
				{
					break;
				}
			}
		}
	}
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
/*void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef	RxHeader;
	CanMsgTypeDef st_CanMsg;


	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, st_CanMsg.Data);
	__HAL_CAN_DISABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

	for (uns8 u8_UserCan = 0; (u8_UserCan < NB_OF_APPS_USING_CAN_MAX); u8_UserCan++)
	{
		if(pst_LocUserCans[u8_UserCan] != NULL)
		{
			if(pst_LocUserCans[u8_UserCan]->hcan == hcan)
			{
				st_CanMsg.DLC =  RxHeader.DLC;
				st_CanMsg.StdId = RxHeader.StdId;
				st_CanMsg.ExtId = RxHeader.ExtId;
				st_CanMsg.IDE = RxHeader.IDE;

				User_CAN_notifyRXFrameReceived(pst_LocUserCans[u8_UserCan],&st_CanMsg);

				__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

				break;
			}
		}
	}
}*/


//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);

	if((hcan->ErrorCode & (HAL_CAN_ERROR_TX_ALST0 | HAL_CAN_ERROR_TX_TERR0) ) != 0)
	{
		User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[0], FALSE);
		hcan->ErrorCode ^= (HAL_CAN_ERROR_TX_ALST0 | HAL_CAN_ERROR_TX_TERR0) ;
	}

	if((hcan->ErrorCode & (HAL_CAN_ERROR_TX_ALST1 | HAL_CAN_ERROR_TX_TERR1) ) != 0)
	{
		User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[1], FALSE);
		hcan->ErrorCode ^= (HAL_CAN_ERROR_TX_ALST1 | HAL_CAN_ERROR_TX_TERR1) ;
	}

	if((hcan->ErrorCode & (HAL_CAN_ERROR_TX_ALST2 | HAL_CAN_ERROR_TX_TERR2) ) != 0)
	{
		User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[2], FALSE);
		hcan->ErrorCode ^= (HAL_CAN_ERROR_TX_ALST2 | HAL_CAN_ERROR_TX_TERR2) ;
	}

}



//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);
	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[0], TRUE);
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);
	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[1], TRUE);
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);
	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[2], TRUE);
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);
	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[0], FALSE);
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);

	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[1], FALSE);
}

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan)
{
	uns8 u8_CanBusIndex = _UserCan_FindCanBusIndex(hcan);

	User_CAN_notifyTXFrameSent(st_UserCanH[u8_CanBusIndex]._tu8_MailBoxUser, st_UserCanH[u8_CanBusIndex]._tst_MailBoxesIds[2], FALSE);

}



//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
static void _UserCan_Restart(uns8 u8_CanIndex)
{
	CAN_FilterTypeDef  sFilterConfig;

	CAN_HandleTypeDef * hcan = pst_CanHandle[u8_CanIndex];

	/*##-2- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;

	/*##-3- Start the CAN peripheral ###########################################*/
	if (HAL_CAN_Stop(hcan) != HAL_OK)
	{

	}

	/*##-4- Activate CAN RX notification #######################################*/
	if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{

	}

	if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{

	}


	if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{

	}

	HAL_CAN_DeInit(hcan);


	HAL_CAN_Init(hcan);

	if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
	{
	/* Filter configuration Error */
		//Error_Handler();
	}

	/*##-3- Start the CAN peripheral ###########################################*/
	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
	/* Start Error */
		//Error_Handler();
	}

	/*##-4- Activate CAN RX notification #######################################*/
	if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
	/* Notification Error */
		//Error_Handler();
	}


	if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
	/* Notification Error */
		//Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
	/* Notification Error */
		//Error_Handler();
	}


}


//==============================================================================
//=========================== EXPORTED FUNCTIONS ===============================
//==============================================================================
//==============================================================================
// DESCRIPTION : ******* UserCan_registerRXCallback *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    cb, (UserCanRxFrameCB, IN) : callback function pointer
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    BOOL : TRUE if callback is correctly register / FALSE if register failed
//
// DESIGN INFORMATION :
//==============================================================================
extern BOOL UserCan_registerRXCallback(uns8 u8_UserCanIndex, UserCanRxFrameCB cb)
{
	BOOL b_success = FALSE;

	if (cb != NULL)
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallback == NULL && pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallbackCpp == NULL) // CB free
		{
			pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallback = cb;
			b_success = TRUE;
		}
	}

	if(b_success == FALSE)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return b_success;
}

//==============================================================================
// DESCRIPTION : ******* UserCan_registerRXCallback *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    cb, (UserCanRxFrameCB, IN) : callback function pointer
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    BOOL : TRUE if callback is correctly register / FALSE if register failed
//
// DESIGN INFORMATION :
//==============================================================================
extern BOOL UserCan_registerRXCallbackForCpp(uns8 u8_UserCanIndex, void* obj, UserCanRxFramCBForCpp cb)
{
	BOOL b_success = FALSE;

	if (cb != NULL)
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallback == NULL && pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallbackCpp == NULL) // CB free
		{
			pst_LocUserCans[u8_UserCanIndex].UserCanRXFrameCallbackCpp = cb;
			pst_LocUserCans[u8_UserCanIndex].obj = obj;
			b_success = TRUE;
		}
	}

	if(b_success == FALSE)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return b_success;
}

//==============================================================================
// DESCRIPTION : ******* UserCan_registerTXCallback *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    cb, (UserCanTxFrameCB, IN) : callback function pointer
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    BOOL : TRUE if callback is correctly register / FALSE if register failed
//
// DESIGN INFORMATION :
//==============================================================================
extern BOOL UserCan_registerTXCallback(uns8 u8_UserCanIndex, UserCanTxFrameCB cb)
{
	BOOL b_success = FALSE;


	if (cb != NULL)
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallback == NULL && pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallbackCpp == NULL) // CB free
		{
			pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallback = cb;
			b_success = TRUE;
		}
	}

	if(b_success == FALSE)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return b_success;
}

//==============================================================================
// DESCRIPTION : ******* UserCan_registerTXCallback *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    cb, (UserCanTxFrameCB, IN) : callback function pointer
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    BOOL : TRUE if callback is correctly register / FALSE if register failed
//
// DESIGN INFORMATION :
//==============================================================================
extern BOOL UserCan_registerTXCallbackForCpp(uns8 u8_UserCanIndex, void* obj, UserCanTxFramCBForCpp cb)
{
	BOOL b_success = FALSE;


	if (cb != NULL)
	{
		if (pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallback == NULL && pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallbackCpp == NULL) // CB free
		{
			pst_LocUserCans[u8_UserCanIndex].UserCanTXFrameCallbackCpp = cb;
			pst_LocUserCans[u8_UserCanIndex].obj = obj;
			b_success = TRUE;
		}
	}

	if(b_success == FALSE)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return b_success;
}



//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UserCan_Init(void)
{
	u8_IndexInitLocUserCans = 0;

	for (uns8 u8_UserCan = 0; (u8_UserCan < NB_OF_APPS_USING_CAN_MAX); u8_UserCan++)
	{
		pst_LocUserCans[u8_UserCan].u8_CanIndex = 255;
		pst_LocUserCans[u8_UserCan].b_CanRunning = FALSE;

		pst_LocUserCans[u8_UserCan].UserCanRXFrameCallback = NULL;
		pst_LocUserCans[u8_UserCan].UserCanTXFrameCallback = NULL;

		pst_LocUserCans[u8_UserCan].UserCanRXFrameCallbackCpp = NULL;
		pst_LocUserCans[u8_UserCan].UserCanTXFrameCallbackCpp = NULL;
		pst_LocUserCans[u8_UserCan].obj = NULL;
	}
}




//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns8 UserCan_CreateHandle(TE_USED_CAN_LIST e_CanBusId)
{
	uns8 u8_LocIndex = 255;

	if(u8_IndexInitLocUserCans < NB_OF_APPS_USING_CAN_MAX)
	{
		pst_LocUserCans[u8_IndexInitLocUserCans].u8_CanIndex = e_CanBusId;
		pst_LocUserCans[u8_IndexInitLocUserCans].b_CanRunning = FALSE;

		pst_LocUserCans[u8_IndexInitLocUserCans].UserCanRXFrameCallback = NULL;
		pst_LocUserCans[u8_IndexInitLocUserCans].UserCanTXFrameCallback = NULL;

		pst_LocUserCans[u8_IndexInitLocUserCans].UserCanRXFrameCallbackCpp = NULL;
		pst_LocUserCans[u8_IndexInitLocUserCans].UserCanTXFrameCallbackCpp = NULL;
		pst_LocUserCans[u8_IndexInitLocUserCans].obj = NULL;

		u8_LocIndex = u8_IndexInitLocUserCans;
		u8_IndexInitLocUserCans++;
	}
	else
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return u8_LocIndex;
}


extern void UserCan_SetCanBusSpeed(TE_USED_CAN_LIST e_CanBusId, E_CAN_BUS_SPEED e_CanSpeed)
{
	CAN_HandleTypeDef * hcan = pst_CanHandle[e_CanBusId];
	hcan->Init.Prescaler = _tu8_CanBusSpeedTable[e_CanSpeed];

	if(hcan->State != HAL_CAN_STATE_RESET)
		_UserCan_Restart(e_CanBusId);
}

//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UserCan_Start(uns8 u8_UserCanIndex)
{
	BOOL b_CanRunning = FALSE;
	CAN_FilterTypeDef  sFilterConfig;

	CAN_HandleTypeDef * hcan = pst_CanHandle[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex];

	/*##-2- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;





	//Scannin to check of CAN already started from another user
	for (uns8 u8_UserCan = 0; (u8_UserCan < u8_IndexInitLocUserCans); u8_UserCan++)
	{
		if(pst_LocUserCans[u8_UserCan].u8_CanIndex != 255)
		{
			if(pst_LocUserCans[u8_UserCan].u8_CanIndex == pst_LocUserCans[u8_UserCanIndex].u8_CanIndex)
			{
				b_CanRunning |= pst_LocUserCans[u8_UserCan].b_CanRunning;
			}
		}
	}




	if(b_CanRunning == FALSE || hcan->State == HAL_CAN_STATE_RESET)
	{
		if(hcan->State == HAL_CAN_STATE_RESET)
			HAL_CAN_Init(hcan);

		if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
		{
		/* Filter configuration Error */
			//Error_Handler();
		}

		/*##-3- Start the CAN peripheral ###########################################*/
		if (HAL_CAN_Start(hcan) != HAL_OK)
		{
		/* Start Error */
			//Error_Handler();
		}

		/*##-4- Activate CAN RX notification #######################################*/
		if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		{
		/* Notification Error */
			//Error_Handler();
		}


		if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
		{
		/* Notification Error */
			//Error_Handler();
		}

		if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
		{
		/* Notification Error */
			//Error_Handler();
		}
	}

	pst_LocUserCans[u8_UserCanIndex].b_CanRunning = TRUE;
}




//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UserCan_Stop(uns8 u8_UserCanIndex)
{
	CAN_HandleTypeDef * hcan = pst_CanHandle[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex];
	BOOL b_CanRunning = FALSE;



	//Scannin to check of CAN still used by another user
	for (uns8 u8_UserCan = 0; (u8_UserCan < u8_IndexInitLocUserCans); u8_UserCan++)
	{
		if((pst_LocUserCans[u8_UserCan].u8_CanIndex != 255) && (u8_UserCanIndex != u8_UserCan))
		{
			if(pst_LocUserCans[u8_UserCan].u8_CanIndex == pst_LocUserCans[u8_UserCanIndex].u8_CanIndex)
			{
				b_CanRunning |= pst_LocUserCans[u8_UserCan].b_CanRunning;
			}
		}
	}


	if(b_CanRunning == FALSE)
	{
		/*##-3- Start the CAN peripheral ###########################################*/
		if (HAL_CAN_Stop(hcan) != HAL_OK)
		{

		}

		/*##-4- Activate CAN RX notification #######################################*/
		/*if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
		{

		}

		if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
		{

		}


		if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
		{

		}

		HAL_CAN_DeInit(hcan);*/

	}
	pst_LocUserCans[u8_UserCanIndex].b_CanRunning = FALSE;
}

//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UserCan_Restart(uns8 u8_UserCanIndex)
{
	_UserCan_Restart(pst_LocUserCans[u8_UserCanIndex].u8_CanIndex);
}
//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern BOOL UserCan_SendCanFrame(uns8 u8_UserCanIndex, CanMsgTypeDef st_TxMessage)
{
	BOOL b_TmpResult = FALSE;
	CAN_TxHeaderTypeDef   TxHeader;
	uns32 u32_TxMailBox = 0;

	CAN_HandleTypeDef * hcan = pst_CanHandle[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex];


	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 1)
	{
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.TransmitGlobalTime = DISABLE;
		TxHeader.DLC = st_TxMessage.DLC;
		TxHeader.StdId = st_TxMessage.st_CanId.u32_Id;
		TxHeader.ExtId  = st_TxMessage.st_CanId.u32_Id;
		TxHeader.IDE  = st_TxMessage.st_CanId.IDE;

		if(HAL_CAN_AddTxMessage(hcan, &TxHeader, st_TxMessage.Data, &u32_TxMailBox) == HAL_OK)
		{
			if(u32_TxMailBox == CAN_TX_MAILBOX0) u32_TxMailBox = 0;
			else if(u32_TxMailBox == CAN_TX_MAILBOX1) u32_TxMailBox = 1;
			else if(u32_TxMailBox == CAN_TX_MAILBOX2) u32_TxMailBox = 2;


			st_UserCanH[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex]._tst_MailBoxesIds[u32_TxMailBox].IDE = TxHeader.IDE;
			if(TxHeader.IDE == CAN_ID_STD)
				st_UserCanH[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex]._tst_MailBoxesIds[u32_TxMailBox].u32_Id = TxHeader.StdId;
			else
				st_UserCanH[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex]._tst_MailBoxesIds[u32_TxMailBox].u32_Id = TxHeader.ExtId;

			st_UserCanH[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex]._tu8_MailBoxUser = u8_UserCanIndex;

			b_TmpResult = TRUE;

		}
	}

	return b_TmpResult;
}

//==============================================================================
// DESCRIPTION : ******* FUNCTION NAME HERE *******
//
// PARAMETERS : (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE : ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern BOOL UserCan_CheckFreeSpaceInTxBuffer(uns8 u8_UserCanIndex)
{
	BOOL b_TmpResult = FALSE;

	CAN_HandleTypeDef * hcan = pst_CanHandle[pst_LocUserCans[u8_UserCanIndex].u8_CanIndex];

	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 3) //3 to use only one mail box, prevent messages to stay on mail boxes.
	{
		b_TmpResult = TRUE;
	}

	return b_TmpResult;
}






