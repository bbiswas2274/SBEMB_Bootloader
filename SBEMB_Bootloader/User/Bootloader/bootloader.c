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

//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//
// #include <system_file_name.h>
// #include "project_file_name.h"
//------------------------------------------------------------------------------
#include "bootloader.h"

#include "UserCan.h"
#include "flash.h"

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
typedef enum _TE_BMS_MODE
{
    eBMS_MODE_INIT = 0,
    eBMS_MODE_SLEEP,
    eBMS_MODE_CONFIG,
    eBMS_MODE_BALANCING,
    eBMS_MODE_STANDBY,
    eBMS_MODE_PRECHARGE,
    eBMS_MODE_DISCHARGE,
    eBMS_MODE_CHARGE,
    eBMS_MODE_CHARGE_DISCHARGE,


	eBMS_MODE_BOOTLOADER = 0x25

}TE_BMS_MODE;

typedef  union _TS_CAN_FRAME_BMS_BOOTLOADER_INFOS
{
	struct __attribute((packed))
	{
		TE_BMS_MODE e_BmsState;

		uns8 u8_BootloaderVersionMajor;
		uns8 u8_BootloaderVersionMinor;

		uns8 u8_SystemUniqueID;
	};
	uns8 _tu8[8];
}TS_CAN_FRAME_BMS_BOOTLOADER_INFOS;

typedef enum _TE_CAN_IDS
{
	CAN_ID_MASTER_CMD = 0,


	CAN_ID_MASTER_STATE= 8,
	CAN_ID_MASTER_CELLS_MINS_MAXS ,
	CAN_ID_MASTER_CURRENT_SOX,
	CAN_ID_MASTER_CHARGE_CURRENT_SET_POINT,
	CAN_ID_MASTER_CHARGE_DISCHARGE_CURRENT,
	CAN_ID_MASTER_FLAGS,
	CAN_ID_MASTER_MINS_MAXS_MODULES_CURRENTS,
	CAN_ID_MASTER_MAX_PCB_TEMP,
	CAN_ID_MASTER_PACK_VOLTAGE,
	CAN_ID_MASTER_MAX_REGEN_VOLTAGE,

	CAN_ID_MASTER_BOOTLOADER_RW_CMD = 0x61,
	CAN_ID_MASTER_BOOTLOADER_ACK,


	CAN_ID_MASTER_CHARGER_CMD = 0x1806E5F4,

} TE_CAN_IDS;
//-----------------------------------------------------------------------------
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
static uns8 _u8_CanHandleIndex;
static CanMsgTypeDef _st_LastRxMessage;
volatile static BOOL _b_NewRxMessage;

static uns32 _u32_AddToProg;
static uns8 _u8_SizeToProgram;
//Declare input buffer and its pointer
static uns8 DataBuffer[130];
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

//==============================================================================
//=========================== LOCAL FUNCTIONS ==================================
//==============================================================================

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
BOOL _Supervisor_OnRxFrameCallback(CanMsgTypeDef *CanMsg)
{
	BOOL b_FrameLinked = TRUE;

	if(_b_NewRxMessage == FALSE && CanMsg->st_CanId.IDE == CAN_ID_STD && CanMsg->st_CanId.u32_Id == CAN_ID_MASTER_BOOTLOADER_RW_CMD)
	{
		_st_LastRxMessage = *CanMsg;
		_b_NewRxMessage = TRUE;
	}

  return b_FrameLinked;
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
void _Supervisor_OnTxFrameCallback(TS_USER_CAN_ID ts_CanIds, BOOL b_succeed)
{

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
static BOOL Bootloader_ReceiveAdd()
{
	BOOL b_Tmp =FALSE;
	uns8 u8_ChkSum = 0;
	uns32 u32_TimeOut = UTILS_Tick_Get(0);

	_b_NewRxMessage = FALSE;

	while(_b_NewRxMessage == FALSE && UTILS_Tick_Get(u32_TimeOut) < 100 _MS);

	if(_b_NewRxMessage == TRUE)
	{
		_u32_AddToProg = ((uns32)_st_LastRxMessage.Data[0]) | ((uns32)_st_LastRxMessage.Data[1] << 8) |
				((uns32)_st_LastRxMessage.Data[2] << 16) | ((uns32)_st_LastRxMessage.Data[3] << 24);

		u8_ChkSum = _st_LastRxMessage.Data[0] ^ _st_LastRxMessage.Data[1] ^ _st_LastRxMessage.Data[2] ^ _st_LastRxMessage.Data[3] ^ _st_LastRxMessage.Data[4] ;

		if(u8_ChkSum == 0)
			b_Tmp = TRUE;
	}

	return b_Tmp;
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
static BOOL Bootloader_ReceiveCount()
{
	BOOL b_Tmp =FALSE;
	uns32 u32_TimeOut = UTILS_Tick_Get(0);

	_b_NewRxMessage = FALSE;

	while(_b_NewRxMessage == FALSE && UTILS_Tick_Get(u32_TimeOut) < 100 _MS);

	if(_b_NewRxMessage == TRUE)
	{
		_u8_SizeToProgram = _st_LastRxMessage.Data[0];
		b_Tmp = TRUE;
	}

	return b_Tmp;
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
static BOOL BootloaderReceiveData()
{
	uns8 u8_ChkSum = _u8_SizeToProgram;

	uns8 j = 0;
	uns8 i,k = 0;
	BOOL b_tmp = TRUE;
	uns32 u32_TimeOut = UTILS_Tick_Get(0);

	for(i=0; i<_u8_SizeToProgram && (b_tmp == TRUE); i+=8)
	{
		_b_NewRxMessage = FALSE;
		while(_b_NewRxMessage == FALSE && UTILS_Tick_Get(u32_TimeOut) < 100 _MS);

		if(_b_NewRxMessage == TRUE)
		{
			for(k = 0; k < 8; k++)
			{
				DataBuffer[j++] = _st_LastRxMessage.Data[k];
				u8_ChkSum ^= _st_LastRxMessage.Data[k];
			}
		}
		else
		{
			b_tmp = FALSE;
		}
	}

	_b_NewRxMessage = FALSE;
	while(_b_NewRxMessage == FALSE && UTILS_Tick_Get(u32_TimeOut) < 100 _MS);

	if(_b_NewRxMessage == TRUE && b_tmp == TRUE)
	{
		if(u8_ChkSum != _st_LastRxMessage.Data[0])
		{
			b_tmp = FALSE;
		}
	}

	return b_tmp;
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
static void Bootloader_TransmitAckNack(BOOL b_Succes)
{
	CanMsgTypeDef st_TxMessage;
	uns32 u32_TimeOut = UTILS_Tick_Get(0);

	st_TxMessage.st_CanId.u32_Id = CAN_ID_MASTER_BOOTLOADER_ACK;
	st_TxMessage.st_CanId.IDE = CAN_ID_STD;
	st_TxMessage.DLC = 8;

	if(b_Succes)
		st_TxMessage.Data[0] = 1;
	else
		st_TxMessage.Data[0] = 0;


	while(UserCan_CheckFreeSpaceInTxBuffer(_u8_CanHandleIndex) == FALSE && UTILS_Tick_Get(u32_TimeOut) < 100 _MS);

	if(UserCan_CheckFreeSpaceInTxBuffer(_u8_CanHandleIndex) == TRUE)
	{
		UserCan_SendCanFrame(_u8_CanHandleIndex, st_TxMessage);
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
static void Bootloader_ProcessWMCmd()
{
	BOOL b_tmp;

	Bootloader_TransmitAckNack(TRUE);

	b_tmp = Bootloader_ReceiveAdd();

	if(b_tmp)
		b_tmp = Bootloader_ReceiveCount();

	if(_u8_SizeToProgram <= 128 && b_tmp)
	{
		b_tmp = BootloaderReceiveData();
	}
	else
		b_tmp = FALSE;

	if(b_tmp)
	{
		if(flash_write(_u32_AddToProg, DataBuffer, _u8_SizeToProgram) != 0)
			b_tmp = FALSE;
	}

	Bootloader_TransmitAckNack(b_tmp);
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
static void Bootloader_ProcessChkMCmd()
{
	BOOL b_tmp;
	uns8 i = 0;

	Bootloader_TransmitAckNack(TRUE);

	b_tmp = Bootloader_ReceiveAdd();

	if(b_tmp)
		b_tmp = Bootloader_ReceiveCount();

	if(_u8_SizeToProgram <= 128 && b_tmp)
	{
		b_tmp = BootloaderReceiveData();
	}
	else
		b_tmp = FALSE;

	if(b_tmp)
	{
		for(i = 0; i < _u8_SizeToProgram && b_tmp == TRUE; i++)
		{
			if(*(uns8*)_u32_AddToProg++ != DataBuffer[i])
				b_tmp = FALSE;
		}
	}

	Bootloader_TransmitAckNack(b_tmp);
}


//==============================================================================
//=========================== EXPORTED FUNCTIONS ===============================
//==============================================================================

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
extern void Bootloader_Init(void)
{
	_u8_CanHandleIndex = UserCan_CreateHandle(SUPERVISOR_CAN);
	UserCan_registerRXCallback(_u8_CanHandleIndex, _Supervisor_OnRxFrameCallback);
	UserCan_registerTXCallback(_u8_CanHandleIndex, _Supervisor_OnTxFrameCallback);

	HAL_GPIO_WritePin(EN_SUPERVISER_CAN_GPIO_Port, EN_SUPERVISER_CAN_Pin, GPIO_PIN_RESET);
	UserCan_Start(_u8_CanHandleIndex);
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
static TS_CAN_FRAME_BMS_BOOTLOADER_INFOS _st_FrameBmsBootloaderInfo;

#define CMD_CHECK_MEMORY_COMMAND   		0x06
#define CMD_GO_COMMAND  				0x07
#define CMD_WRITE_MEMORY_COMMAND  		0x05
#define CMD_ERASE_MEMORY_COMMAND  		0x75
#define CMD_RESYNC_COMMAND  			0x8F

extern void Bootloader_Process(void)
{
	static uns32 _u32_TimerState = 0;
	CanMsgTypeDef st_TxMessage;


	if(_b_NewRxMessage == TRUE)
	{
		switch(_st_LastRxMessage.Data[0])
		{
			case CMD_WRITE_MEMORY_COMMAND:
				Bootloader_ProcessWMCmd();
				break;

			case CMD_CHECK_MEMORY_COMMAND:
				Bootloader_ProcessChkMCmd();
				break;

			case CMD_GO_COMMAND:
				flash_jump_to_app();
				break;

			case CMD_ERASE_MEMORY_COMMAND:
				Bootloader_TransmitAckNack(flash_erase() == FLASH_OK);
				break;

			case CMD_RESYNC_COMMAND:
				Bootloader_TransmitAckNack(TRUE);
				break;

			default:
				break;
		}
		_b_NewRxMessage = FALSE;
	}
	else if(UTILS_Tick_Get(_u32_TimerState) > 1 _SEC)
	{
		if(UserCan_CheckFreeSpaceInTxBuffer(_u8_CanHandleIndex) == TRUE)
		{
			_u32_TimerState = UTILS_Tick_Get(0);
			_st_FrameBmsBootloaderInfo.e_BmsState = eBMS_MODE_BOOTLOADER;

			_st_FrameBmsBootloaderInfo.u8_BootloaderVersionMajor = cBOOTLOADER_VERSION_MAJOR;
			_st_FrameBmsBootloaderInfo.u8_BootloaderVersionMinor = cBOOTLOADER_VERSION_MINOR;

			_st_FrameBmsBootloaderInfo.u8_SystemUniqueID = cUNIQUE_BOOTLOADER_ID;

			UTILS_MemCpy(st_TxMessage.Data, _st_FrameBmsBootloaderInfo._tu8, 8);
			st_TxMessage.st_CanId.u32_Id = CAN_ID_MASTER_STATE;

			st_TxMessage.st_CanId.IDE = CAN_ID_STD;
			st_TxMessage.DLC = 8;
			UserCan_SendCanFrame(_u8_CanHandleIndex, st_TxMessage);
		}
	}
}




