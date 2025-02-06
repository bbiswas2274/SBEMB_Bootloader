#ifndef _UTILS_H

	#define _UTILS_H

	#include "TypesDefCM.h"

	#define DIGITAL_INPUT	0
	#define DIGITAL_OUTPUT	1
	#define ANALOG_INPUT	3
	#define PERIPH_IN_OUT	2

	#define LOW8(a)			(uns8)a
	#define HIGH8(a)		(uns8)((a >> 8) & 0x00FF)
	#define LOW16(a)		(uns16)a
	#define HIGH16(a)		(uns16)((a >> 16) & 0x0000FFFF)

	#define _MS		*1 //	*((uint32_t)10 _100US)
	#define _SEC			*((uint32_t)1000 _MS)
	#define _MIN		*((uint32_t)60 _SEC)

	#define CM_MS		*1 //	*((uint32_t)10 _100US)
	#define CM_SEC		*((uint32_t)1000 _MS)
	#define CM_MIN		*((uint32_t)60 _SEC)
	#define CM_HOUR     *((uns32)60 CM_MIN)
	#define CM_DAY      *((uns32)24 CM_HOUR)

	#define FreqHz_(a)  (1 _SEC/a)


	#define CRC16 0x8005

#ifdef __cplusplus
 extern "C" {
#endif

	extern uns32 gu32_TickCount;

	extern uns32 UTILS_Tick_Get(uns32 u32_TickVal);
	extern void hang(void);
	extern void UTILS_Delay_Ms(uns32 milliseconds);

	extern uns8 UTILS_User_ctoh(char c);
	extern char UTILS_User_htoc(uns8 u8_Val);
	extern uns8 *UTILS_User_itoa (s32 value, uns8 *string, s32 radix, uns8 u8_Digits);
	extern uns8 *UTILS_User_unsitoa(uns32 v, uns8 *string, int radix, uns8 u8_Digits);
	extern uns8 UTILS_Ascii_To_Byte(uns8 u8_Char);
	extern uns32 UTILS_Binary_To_BCD(uns32 u32_Value, uns32 u32_MaxValue);
	extern uns32 UTILS_BCD_To_Binary(uns32 u32_BCDValue);
	extern uns8 UTILS_Days_Of_Month_Resolution(uns8 u8_Month, uns8 u8_Year);
	extern uns8 UTILS_Day_Of_Week_Resolution(uns8 u8_Day, uns8 u8_Month, uns8 u8_Year);
//	extern u8 *UTILS_ByteSwap(uns8* pu8_InBuffer, uns16 u16_InLength);
	extern BOOL UTILS_Get_Char_Pos(uns8 u8_Char, uns8* pu8_Str, uns8 u8_StartPos, uns8 *pu8_CharPos);
//	extern float UTILS_Format_Value(s32 s32_Val, uns8 *pu8_Format, uns8 u8_Dec);
	extern uns32 UTILS_String_Hash(uns8 *pu8_Buffer);
//	extern float UTILS_Float(float f_Val, uns8 u8_Dec);
	extern s32 UTILS_Round(const float f_Val, uns8 u8_Dec);
	extern uns8 *UTILS_Print_Period(uns8 *pu8_Buffer, uns32 u32_SecCount);

	extern float UTILS_Str2Float(uns8 * pu8_Buffer);
	extern uns32 UTILS_Str2uns32(uns8 * pu8_Buffer);
	extern uns8 UTILS_Str2uns8(uns8 * pu8_Buffer);
	extern uns32 UTILS_GetNumberInXPosInStr(uns8 * pu8_Str, uns8 u8_NumberPos);

	extern uns8 *UTILS_User_ftoa (float value, uns8 *string,uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits);

	extern uns16 UTILS_CalculateCheckSum(uns8 * pu8_Msg, uns8 u8_Length);
	extern uns8 UTILS_CalculateNmeaCheckSum(uns8 * pu8_Msg, uns8 u8_Length);


	extern BOOL UTILS_IsBufferStartWithConst(uns8 * pu8_Buffer,	uns8 * pu8_Str, uns16 u16_BufferSize);
	extern uns8 UTILS_NumberOfCharInStr( uns8 * pu8_Buffer,  uns8 u8_CharToFind);
	extern uns8 UTILS_IndexOfCharInStr( uns8 * pu8_Buffer,  uns8 u8_CharToFind);
	extern uns8 UTILS_LastIndexOfCharInStr( uns8 * pu8_Buffer,  uns8 u8_CharToFind);
	extern BOOL UTILS_StrCmp( uns8 * pu8_Buffer1,  uns8 * pu8_Buffer2);
	extern BOOL UTILS_StrCmpRom( uns8 * pu8_Buffer1,  uns8 * pu8_Buffer2);

	extern void UTILS_strcat(uns8* pu8_Dest,  uns8* pcru8_strToAdd);
	extern void UTILS_strcatram(uns8* pu8_Dest, uns8* pu8_strToAdd);

	extern uns8 *UTILS_User_unsitoa_cat(uns32 v, uns8 *string, int radix, uns8 u8_Digits);
	extern uns8 *UTILS_User_itoa_cat(s32 v, uns8 *string, int radix, uns8 u8_Digits);
	extern uns8 *UTILS_User_ftoa_cat (float value, uns8 *string,uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits);
	extern uns8 *UTILS_User_dtoa (double value, uns8 *string, uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits);

	extern void UTILS_strcharcat(uns8* pu8_Dest, uns8 u8_CharToAdd);

	extern uns8 UTILS_StrLn(uns8 *pu8_Buffer);
	extern uns8 UTILS_StrLnCst(const uns8 * pu8_Buffer);

	extern void UTILS_MemCpy(uns8 *pu8_BufferCible, uns8 *pu8_BufferSource, uns16 u16_Size);

	extern void UTILS_strcpy(uns8* pu8_Dest,  uns8* pcru8_Source);
	extern void UTILS_strcpyram(uns8* pu8_Dest, uns8* pcru8_Source);

    extern uns16 UTILS_IndianSwitchUns16(uns16 u16_InVal);
    extern uns32 UTILS_IndianSwitchUns32(uns32 u32_InVal);
    extern uns64 UTILS_IndianSwitchUns64(uns64 u64_InVal);

    extern void UTILS_StrTrim(uns8 * pu8_String);
    extern BOOL UTILS_IsBufferContains(uns8 * pu8_Buffer,  uns8 * pu8_Str);

    extern void UTILS_SubString(uns8 * pu8_Str, uns8 u8_StartIndex);
    extern uns8 UTILS_IndexOfStrInString(uns8 * pu8_Buffer,  uns8 * pu8_Str);
		
	extern uns16 UTILS_crc16(const uns8 *data, uns16 size);

	extern uns32 UTILS_User_s32abs(s32 value);
	extern float UTILS_User_fabs(float value);

	#define ASSERT_FAILED() assert_failed((uns8 *)__FILE__, __LINE__);
	#define ASSERT(b) {if (!(b)) {ASSERT_FAILED();}}

#ifdef __cplusplus
 }
#endif
#endif
