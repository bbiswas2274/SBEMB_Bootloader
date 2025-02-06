
#include "UtilsCM.h"

	uns32 gu32_TickCount = 0;

	const uns8 u8tab_DaysofMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
static uns32 E(uns8 u8_Pow);
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
// Main function
//==============================================================================	
extern uns32 UTILS_Tick_Get(uns32 u32_TickVal)
{
	uns32 u32_TmpResult;

	u32_TmpResult = gu32_TickCount;
	if (u32_TmpResult >= u32_TickVal)
		return(u32_TmpResult - u32_TickVal);
	else
		return(0xFFFFFFFF - u32_TickVal + u32_TmpResult);
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
// Main function
//==============================================================================
extern void hang(void)
{
    do {} while (1);
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
// Main function
//==============================================================================
extern uns32 UTILS_Binary_To_BCD(uns32 u32_Value, uns32 u32_MaxValue)
{
	uns32 u32_TmpResult = 0;
	uns32 u32_Shift = 0;
	uns32 u32_Mask = 0x0000000F;

	if(u32_Value > u32_MaxValue)
	{
		u32_Value = u32_MaxValue;
	}

	while(u32_Value > 0)
	{
 		u32_TmpResult |= ((u32_Value % (uns32)10) << u32_Shift) & u32_Mask;
 		u32_Value /= (uns32)10;
		u32_Shift += (uns32)4;
		u32_Mask *= (uns32)0x10;
	}

	return(u32_TmpResult);
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
// Main function
//==============================================================================
extern uns32 UTILS_BCD_To_Binary(uns32 u32_BCDValue)
{
	uns32 u32_TmpResult;

	u32_TmpResult = ((u32_BCDValue >> (uns32)16) & 0x0000000F) * (uns32)10000;
	u32_TmpResult += ((u32_BCDValue >> (uns32)12) & 0x0000000F) * (uns32)1000;
	u32_TmpResult += ((u32_BCDValue >> (uns32)8) & 0x0000000F) * (uns32)100;
	u32_TmpResult += ((u32_BCDValue >> (uns32)4) & 0x0000000F) * (uns32)10;
	u32_TmpResult += (u32_BCDValue & 0x0000000F);
	
	return(u32_TmpResult);	
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
// Main function
//==============================================================================
extern char UTILS_User_htoc(uns8 u8_Val)
{
	uns8 u8_TmpResult;

	u8_TmpResult = u8_Val;
	if (u8_TmpResult <= 9)
	{
		u8_TmpResult += 0x30;
	}
	else
	{
		u8_TmpResult += 0x37;
	}

	return(u8_TmpResult);
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
// Main function
//==============================================================================
extern uns8 UTILS_User_ctoh(char c)
{
	if ((c>='0')&&(c<='9'))
		return c-0x30;
	if (c>0x60)
		c-=0x20;
	if ((c>='A')&&(c<='F'))
		return c-0x37;

	return 0;
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
// Main function
//==============================================================================
extern void UTILS_Delay_Ms(uns32 milliseconds)
{
	/**/uns32 u32_TmpVal = UTILS_Tick_Get(0);

	while(UTILS_Tick_Get(u32_TmpVal) < milliseconds _MS);/**/
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
// Main function
//==============================================================================
extern uns8 Ascii_To_Byte(uns8 u8_Char)
{
	if(u8_Char >= 'A')	
	{
		u8_Char -= ('A' - 10);		
	}
	else
	{
		u8_Char -= '0';	
	}

	return(u8_Char);
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
// Main function
//==============================================================================
//extern uns8 UTILS_Convert_U8_To_BCD(uns8 u8_U8Value, uns8 u8_MaxValue)
//{
//	uns8 u8_TmpResult;
//
//	if(u8_U8Value > u8_MaxValue)
//		u8_U8Value = u8_MaxValue;
//	u8_TmpResult = (((u8_U8Value / 10) << 4) & 0xF0);
//	u8_TmpResult |= ((u8_U8Value % 10) & 0x0F);
//
//	return(u8_TmpResult);
//}	

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
// Main function
//==============================================================================
//extern uns8 UTILS_Convert_BCD_To_U8(uns8 u8_BCDValue)
//{
//	uns8 u8_TmpResult;
//
//	u8_TmpResult = ((u8_BCDValue >> 4) & 0x0F) * 10;
//	u8_TmpResult += (u8_BCDValue & 0x0F);
//	
//	return(u8_TmpResult);	
//}

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
// Main function
//==============================================================================
extern uns8 UTILS_Days_Of_Month_Resolution(uns8 u8_Month, uns8 u8_Year)
{
	uns8 u8_TmpResult;

	u8_TmpResult = u8tab_DaysofMonths[u8_Month - 1];
	if (u8_Month == 2)
		if (((u8_Year % 400) == 0) || (((u8_Year % 100) != 0) && ((u8_Year % 4) == 0)))
			u8_TmpResult ++;
	
	return(u8_TmpResult);
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
// Main function
//==============================================================================
extern uns8 UTILS_Day_Of_Week_Resolution(uns8 u8_Day, uns8 u8_Month, uns8 u8_Year)
{	  
    s16 day_of_week;

    if (u8_Month < 3)
    {
        u8_Month += 12;
        --u8_Year;
    }
    day_of_week = u8_Day + (13 * u8_Month - 27) / 5 + u8_Year + u8_Year/4 - u8_Year/100 + u8_Year/400;
    day_of_week = (day_of_week % 7);

    if(day_of_week == 0) day_of_week = 7;
    return ((uns8)day_of_week);

}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_strcpy(uns8* pu8_Dest, uns8* pcru8_Source)
{
	while(*pcru8_Source) *pu8_Dest++ = *pcru8_Source++;
        *pu8_Dest = 0;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_strcpyram(uns8* pu8_Dest, uns8* pcru8_Source)
{
	while(*pcru8_Source) *pu8_Dest++ = *pcru8_Source++;
        *pu8_Dest = 0;
}
//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================	
//extern u8 *Util_ByteSwap(u8* pu8_InBuffer, u16 u16_InLength)
//{
//u8 *pu8_LocResult=pu8_InBuffer;
//u16 u16_LocCount;
//u8 u8_LocSwap;
//	u16_LocCount=(u16_InLength>>1);
//	u16_InLength-=u16_LocCount;
//	while (u16_LocCount!=0)
//	{
//		u16_LocCount--;
//		u8_LocSwap=pu8_InBuffer[u16_LocCount];
//		pu8_InBuffer[u16_LocCount]=pu8_InBuffer[u16_InLength];
//		pu8_InBuffer[u16_InLength]=u8_LocSwap;
//		u16_InLength++;
//	}
//	return pu8_LocResult;
//}

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
// Main function
//==============================================================================
extern uns8 UTILS_StrLn(uns8 *pu8_Buffer)
{
	uns8 u8_TmpIndex = 0;

	while(*pu8_Buffer++)
	{
		u8_TmpIndex++;
	}
	return u8_TmpIndex;
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
// Main function
//==============================================================================
extern uns8 UTILS_StrLnCst(const uns8 * pu8_Buffer)
{
	uns8 u8_TmpIndex = 0;

	while(*pu8_Buffer++)
	{
		u8_TmpIndex++;
	}
	return u8_TmpIndex;
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
// Main function
//==============================================================================
extern void UTILS_MemCpy(uns8 *pu8_BufferCible, uns8 *pu8_BufferSource, uns16 u16_Size)
{
    while(u16_Size > 0)
    {
        *pu8_BufferCible++ = *pu8_BufferSource++;
        u16_Size--;
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
// Main function
//==============================================================================
extern uns8 *UTILS_User_unsitoa(uns32 v, uns8 *string, int radix, uns8 u8_Digits)
{
  	char tmp[32];
  	char *tp = tmp;
  	uns32 i;
  	uns8 *sp;
	uns8 u8_Count;

  	if (radix > 36 || radix <= 1)
  	{
    	return 0;
  	}

	u8_Count = 0;	
  	while (v || tp == tmp)
  	{
		u8_Count ++;
    	i = v % radix;
    	v = v / radix;

    	if (i < 10)
    	{
      		*tp++ = i+'0';
    	}
    	else
    	{
      		*tp++ = i + 'A' - 10;
    	}
  	}

  	sp = string;

	for(i=u8_Count; i<u8_Digits; i++)
	{
		*sp++ = '0';
	}


  	while (tp > tmp)
  	{
    	*sp++ = *--tp;
  	}

  	*sp = 0;

  	return (string);
}/**/

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
// Main function
//==============================================================================
extern uns8 *UTILS_User_itoa (s32 value, uns8 *string, s32 radix, uns8 u8_Digits)
{
  	char tmp[32];
  	char *tp = tmp;
  	s32 i;
  	unsigned v;
  	s32 sign;
  	uns8 *sp;
	uns8 u8_Count;

  	if (radix > 36 || radix <= 1)
  	{
    	return 0;
  	}

  	sign = (radix == 10 && value < 0);

  	if (sign)
  	{
    	v = -value;
  	}
  	else
  	{
    	v = (unsigned)value;
  	}

	u8_Count = 0;		
  	while (v || tp == tmp)
  	{
		u8_Count ++;
    	i = v % radix;
    	v = v / radix;

    	if (i < 10)
    	{
      		*tp++ = i+'0';
    	}
    	else
    	{
      		*tp++ = i + 'A' - 10;
    	}
  	}

  	sp = string;

  	if (sign)
  	{
    	*sp++ = '-';
  	}

	for(i=u8_Count; i<u8_Digits; i++)
	{
		*sp++ = '0';
	}

  	while (tp > tmp)
  	{
    	*sp++ = *--tp;
  	}

  	*sp = 0;

  	return (string);
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
// Main function
//==============================================================================
extern s32 UTILS_Round(const float f_Val, uns8 u8_Dec) //u8_Dec: decimals after coma
{
    s32 s32_TmpResult = 0;
    float f_TmpResult;
    uns8 i = 0;

    f_TmpResult = f_Val;

    while(i < u8_Dec)
    {
        f_TmpResult *= (float)10.0;
        i++;
    }

    if(f_TmpResult >= 0)
    {
        s32_TmpResult += (s32)(f_TmpResult + 0.5);
    }
    else
    {
        s32_TmpResult += (s32)(f_TmpResult - 0.5);
    }

    return(s32_TmpResult);
}

/**/
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
// Main function
//==============================================================================
static uns32 E(uns8 u8_Pow)
{
	uns32 u32_TmpVal = 1;
	uns8 i = 0;
	
	while(i < u8_Pow)
	{
		u32_TmpVal *= 10;
		i++;	
	}	
	
	return u32_TmpVal;
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
// Main function
//==============================================================================
extern uns32 UTILS_Str2uns32(uns8 * pu8_Buffer)
{
    uns8 i = 0;
    uns32 u32_TmpResult = 0;
    uns8 u8_Digits = 0;
    uns8 tu8_TmpBuffer[16];

    while(*pu8_Buffer != 0 && i < 15)
    {
        tu8_TmpBuffer[i++] = *pu8_Buffer++;
    }
    tu8_TmpBuffer[i] = 0;

    i = 0;

    while(i < 16 && tu8_TmpBuffer[i] >= '0' && tu8_TmpBuffer[i] <= '9')
    {
        u8_Digits++;
        i++;
    }

    for(i=0; i < u8_Digits; i++)
    {
        u32_TmpResult += ((uns32)UTILS_User_ctoh(tu8_TmpBuffer[i])) * E((u8_Digits - i - 1));
    }

    return(u32_TmpResult);
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
// Main function
//==============================================================================
extern uns8 UTILS_Str2uns8(uns8 * pu8_Buffer)
{
    return(uns8)UTILS_Str2uns32(pu8_Buffer);
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
// Main function
//==============================================================================
extern float UTILS_Str2Float(uns8 * pu8_BufferIn)
{
	float f_TmpResult = 0.0;
	uns8 u8_ComaPos;
	BOOL b_ComaFound;
	uns32 u32_TmpVal = 0;
	uns8 u8_TmpVal;
	uns8 tu8_TmpStr[16];
        
	UTILS_strcpyram(tu8_TmpStr, pu8_BufferIn);

	b_ComaFound = UTILS_Get_Char_Pos(',', tu8_TmpStr, 0, &u8_ComaPos);
	
	if(b_ComaFound == FALSE)
	{
            b_ComaFound = UTILS_Get_Char_Pos('.', tu8_TmpStr, 0, &u8_ComaPos);
	}
		
	if(b_ComaFound == TRUE)
	{
            u32_TmpVal = UTILS_Str2uns32(&tu8_TmpStr[u8_ComaPos + 1]);
            u8_TmpVal = UTILS_StrLn(&tu8_TmpStr[u8_ComaPos + 1]);
            f_TmpResult = ((float)u32_TmpVal)/E(u8_TmpVal);
            tu8_TmpStr[u8_ComaPos] = 0;
	}	
	
	u32_TmpVal = UTILS_Str2uns32(tu8_TmpStr);

	f_TmpResult += (float)u32_TmpVal;
	
	return f_TmpResult;
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
// Main function
//==============================================================================
extern BOOL UTILS_Get_Char_Pos(uns8 u8_Char, uns8* pu8_Str, uns8 u8_StartPos, uns8 *pu8_CharPos)
{
	uns8 i;
	BOOL b_CharFound = FALSE;

	i = u8_StartPos;

	while((pu8_Str[i]>0) && (!b_CharFound))
	{
		if(pu8_Str[i] == u8_Char)
		{
			b_CharFound = TRUE;
		}
		else
		{
			i++;	
		}		
	}
	
	*pu8_CharPos = i;

	return(b_CharFound);
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
// Main function
//==============================================================================
extern uns8 *UTILS_User_ftoa (float value, uns8 *string,uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits)
{
    s32 s32_TmpVal;
    uns8 i = 0;
    uns8 u8_TrameLength;
  //  uns8 tu8_TmpTab[16];
    float f_TmpVal;
	
    f_TmpVal = value;
	
		string[0] = '\0';
		if(f_TmpVal < 0) 
		{
			UTILS_strcpy(string, (uns8 *) "-");
			f_TmpVal = -f_TmpVal;
		}
    
    UTILS_User_unsitoa_cat ((uns32)f_TmpVal, string, 10, u8_BeforeComaMinDigits);
    u8_TrameLength = UTILS_StrLn(string);
    if(u8_AfterComaDigits != 0) 
    {
        string[u8_TrameLength++] = '.';
    }

    s32_TmpVal = (s32)f_TmpVal;

    while(i < u8_AfterComaDigits)
    {
        f_TmpVal -= (float)s32_TmpVal;
        f_TmpVal *= 10.0;

      //  if(i == (u8_AfterComaMinDigits - 1)) f_TmpVal += 0.5;
        s32_TmpVal = (s32)f_TmpVal;
        string[u8_TrameLength++] = (uns8)f_TmpVal + '0';
        i++;
    }


    string[u8_TrameLength] = 0;

    return string;
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
// Main function
//==============================================================================
extern uns8 *UTILS_User_dtoa (double value, uns8 *string, uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits)
{
    s32 s32_TmpVal;
    uns8 i = 0;
    uns8 u8_TrameLength;
  //  uns8 tu8_TmpTab[16];
    double f_TmpVal;

    f_TmpVal = value;
	
		string[0] = '\0';
		if(f_TmpVal < 0) 
		{
			UTILS_strcpy(string, (uns8 *) "-");
			f_TmpVal = -f_TmpVal;
		}
    
    UTILS_User_unsitoa_cat ((uns32)f_TmpVal, string, 10, u8_BeforeComaMinDigits);
    u8_TrameLength = UTILS_StrLn(string);
    if(u8_AfterComaDigits != 0) 
    {
        string[u8_TrameLength++] = ',';
    }

    s32_TmpVal = (s32)f_TmpVal;

    while(i < u8_AfterComaDigits)
    {
        f_TmpVal -= (float)s32_TmpVal;
        f_TmpVal *= 10.0;

      //  if(i == (u8_AfterComaMinDigits - 1)) f_TmpVal += 0.5;
        s32_TmpVal = (s32)f_TmpVal;
        string[u8_TrameLength++] = (uns8)f_TmpVal + '0';
        i++;
    }


    string[u8_TrameLength] = 0;

    return string;
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
extern BOOL UTILS_IsBufferStartWithConst(uns8 * pu8_Buffer, uns8 * pu8_Str, uns16 u16_BufferSize)
{
    BOOL b_TmpResult = TRUE;
    uns16 u16_Index = 0;


    while(*pu8_Str != 0 && b_TmpResult == TRUE && u16_Index < u16_BufferSize)
    {
        if(*pu8_Buffer++ != *pu8_Str++)
        {
            b_TmpResult = FALSE;
        }
        u16_Index++;
    }

    return b_TmpResult;
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
extern uns8 UTILS_IndexOfStrInString(uns8 * pu8_Buffer, uns8 * pu8_Str)
{
	uns8 u8_TmpIndex = 0;
	uns8 u8_Result = 0;

	while(*pu8_Buffer != 0)
	{
		if(UTILS_IsBufferStartWithConst(pu8_Buffer, pu8_Str, 255) == TRUE)
		{
			u8_Result = u8_TmpIndex;
			break;
		}
		u8_TmpIndex++;
		pu8_Buffer++;
	}

	return u8_Result;
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
extern void UTILS_SubString(uns8 * pu8_Str, uns8 u8_StartIndex)
{
	uns8 i = 0;

	while(pu8_Str[u8_StartIndex] != 0)
	{
		pu8_Str[i++] = pu8_Str[u8_StartIndex++];
	}

	pu8_Str[i] = 0;
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
extern BOOL UTILS_IsBufferContains(uns8 * pu8_Buffer, uns8 * pu8_Str)
{
	BOOL b_Result = FALSE;
	uns8 u8_StrLn = UTILS_StrLn(pu8_Str);

	while(*pu8_Buffer != 0 && b_Result == FALSE)
	{
		if(UTILS_IsBufferStartWithConst(pu8_Buffer, pu8_Str, u8_StrLn) == TRUE) b_Result = TRUE;
		pu8_Buffer++;
	}

	return b_Result;
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
extern uns8 UTILS_NumberOfCharInStr(uns8 * pu8_Buffer, uns8 u8_CharToFind)
{
    uns8 u8_LoopCount = 0;
    uns8 u8_NbOfChars = 0;

    while(u8_LoopCount < 255 && *pu8_Buffer != 0)
    {
        if(*pu8_Buffer++ == u8_CharToFind) u8_NbOfChars++;
        u8_LoopCount++;
    }

    return u8_NbOfChars;
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
extern uns8 UTILS_IndexOfCharInStr( uns8 * pu8_Buffer,  uns8 u8_CharToFind)
{
    uns8 u8_Index = 0;

    if(UTILS_NumberOfCharInStr(pu8_Buffer, u8_CharToFind) > 0)
    {
        while(u8_Index < 255 && pu8_Buffer[u8_Index] != 0)
        {
            if(pu8_Buffer[u8_Index] == u8_CharToFind) break;
            u8_Index++;
        }

        if(pu8_Buffer[u8_Index] == 0) u8_Index = 0;
    }

    return u8_Index;
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
extern uns8 UTILS_LastIndexOfCharInStr( uns8 * pu8_Buffer,  uns8 u8_CharToFind)
{
    uns8 u8_Index = UTILS_StrLn(pu8_Buffer) - 1;

    if(UTILS_NumberOfCharInStr(pu8_Buffer, u8_CharToFind) > 0)
    {
        while(u8_Index != 0)
        {
            if(pu8_Buffer[u8_Index] == u8_CharToFind) break;
            u8_Index--;
        }

        if(u8_Index == 0) u8_Index = UTILS_StrLn(pu8_Buffer) - 1;
    }

    return u8_Index;
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
extern BOOL UTILS_StrCmp( uns8 * pu8_Buffer1,  uns8 * pu8_Buffer2)
{
    BOOL b_TmpResult = TRUE;

    while(*pu8_Buffer1 != 0 && *pu8_Buffer2 != 0 && b_TmpResult == TRUE)
    {
        if(*pu8_Buffer1++ != *pu8_Buffer2++) b_TmpResult = FALSE;
    }

    if(*pu8_Buffer1 != *pu8_Buffer2) b_TmpResult = FALSE;

    return b_TmpResult;
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
extern BOOL UTILS_StrCmpRom( uns8 * pu8_Buffer1,  uns8 * pu8_Buffer2)
{
    BOOL b_TmpResult = TRUE;

    while(*pu8_Buffer1 != 0 && *pu8_Buffer2 != 0 && b_TmpResult == TRUE)
    {
        if(*pu8_Buffer1++ != *pu8_Buffer2++) b_TmpResult = FALSE;
    }

    if(*pu8_Buffer1 != *pu8_Buffer2) b_TmpResult = FALSE;

    return b_TmpResult;
}


//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_strcat(uns8* pu8_Dest,  uns8* pcru8_strToAdd)
{
    while(*pu8_Dest) pu8_Dest++;

    while(*pcru8_strToAdd)
    {
        *pu8_Dest++ = *pcru8_strToAdd++;
    }

    *pu8_Dest = 0;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_strcatram(uns8* pu8_Dest, uns8* pu8_strToAdd)
{
    while(*pu8_Dest)pu8_Dest++;
    while(*pu8_strToAdd)
    {
        *pu8_Dest++ = *pu8_strToAdd++;
    }
    *pu8_Dest = 0;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns8 *UTILS_User_ftoa_cat (float value, uns8 *string, uns8 u8_BeforeComaMinDigits, uns8 u8_AfterComaDigits)
{
    uns8 u8_StrLn;

    u8_StrLn = UTILS_StrLn(string);

    UTILS_User_ftoa (value, &string[u8_StrLn], u8_BeforeComaMinDigits, u8_AfterComaDigits);
    return string;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns8 *UTILS_User_unsitoa_cat(uns32 v, uns8 *string, int radix, uns8 u8_Digits)
{
    uns8 u8_StrLn;
    u8_StrLn = UTILS_StrLn(string);
    UTILS_User_unsitoa(v, &string[u8_StrLn], radix, u8_Digits);
    return string;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns8 *UTILS_User_itoa_cat(s32 v, uns8 *string, int radix, uns8 u8_Digits)
{
    uns8 u8_StrLn;
    u8_StrLn = UTILS_StrLn(string);
    UTILS_User_itoa(v, &string[u8_StrLn], radix, u8_Digits);
    return string;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_strcharcat(uns8* pu8_Dest, uns8 u8_CharToAdd)
{
    while(*pu8_Dest) pu8_Dest++;
    *pu8_Dest++ = u8_CharToAdd;
    *pu8_Dest = 0;
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
// Main function
//==============================================================================
extern uns16 UTILS_CalculateCheckSum(uns8 * pu8_Msg, uns8 u8_Length)
{
    uns8 i = 0;
    uns16 u16_ChkSum = 0;

    while(i < u8_Length)
    {
        u16_ChkSum += (uns16)pu8_Msg[i];
        i++;
    }

    return u16_ChkSum;
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
// Main function
//==============================================================================
extern uns8 UTILS_CalculateNmeaCheckSum(uns8 * pu8_Msg, uns8 u8_Length)
{
    uns8 u8_ChkSum = 0;

	while(u8_Length != 0)
	{
		u8_Length --;
		u8_ChkSum ^= *pu8_Msg++;;
	}

	return u8_ChkSum;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns16 UTILS_IndianSwitchUns16(uns16 u16_InVal)
{
    uns16 u16_TmpVal;


    u16_TmpVal =  ((u16_InVal >> 8) & (uns16)0x00ff);
    u16_TmpVal |= ((u16_InVal << 8) & (uns16)0xff00);

    return u16_TmpVal;

    //return u16_InVal;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns32 UTILS_IndianSwitchUns32(uns32 u32_InVal)
{
    uns32 u32_TmpVal;

    u32_TmpVal = ((u32_InVal >> 24) & (uns32)0x000000ff);
    u32_TmpVal |= ((u32_InVal >> 8) & (uns32)0x0000ff00);
    u32_TmpVal |= ((u32_InVal << 8) & (uns32)0x00ff0000);
    u32_TmpVal |=((u32_InVal << 24) & (uns32)0xff000000);

    return u32_TmpVal;

    //return u32_InVal;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns64 UTILS_IndianSwitchUns64(uns64 u64_InVal)
{
    uns64 u64_TmpVal;

    u64_TmpVal = ((u64_InVal >> 56)  & (uns64)0x00000000000000ff);
    u64_TmpVal |= ((u64_InVal >> 40) & (uns64)0x000000000000ff00);
    u64_TmpVal |= ((u64_InVal >> 24) & (uns64)0x0000000000ff0000);
    u64_TmpVal |= ((u64_InVal >> 8)  & (uns64)0x00000000ff000000);
    u64_TmpVal |= ((u64_InVal << 8)  & (uns64)0x000000ff00000000);
    u64_TmpVal |= ((u64_InVal >> 24) & (uns64)0x0000ff0000000000);
    u64_TmpVal |= ((u64_InVal << 40) & (uns64)0x00ff000000000000);
    u64_TmpVal |= ((u64_InVal << 56) & (uns64)0xff00000000000000);

    return u64_TmpVal;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns32 UTILS_GetNumberInXPosInStr(uns8 * pu8_Str, uns8 u8_NumberPos)
{
	uns32 u32_TmpVal = 0;

	while(u8_NumberPos != 0)
	{
		u8_NumberPos--;
		while((*pu8_Str < '0' || *pu8_Str > '9') && *pu8_Str != 0) pu8_Str++;

		if(*pu8_Str == 0) break;
		if(u8_NumberPos == 0) break;

		while(*pu8_Str >= '0' && *pu8_Str <= '9' && *pu8_Str != 0) pu8_Str++;
	}

	if(*pu8_Str != 0) u32_TmpVal = UTILS_Str2uns32(pu8_Str);

	return u32_TmpVal;
}

//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern void UTILS_StrTrim(uns8 * pu8_String)
{
	uns8 * pu8_TmpPtr = pu8_String;
	uns8 * pu8_StartString = pu8_String;
	uns8 u8_StrLn;

	if(*pu8_String == ' ')
	{
		while(*pu8_TmpPtr == ' ') pu8_TmpPtr++;
		while(*pu8_TmpPtr != 0)
		{
			*pu8_String = *pu8_TmpPtr;
			pu8_String++;
			pu8_TmpPtr++;
		}
		*pu8_String = 0;
	}

	u8_StrLn = UTILS_StrLn(pu8_StartString);

	pu8_String = &pu8_StartString[u8_StrLn];

	pu8_String--;
	while((*pu8_String == '\n' || *pu8_String == '\r') && u8_StrLn != 0)
	{
		*pu8_String = 0;
		pu8_String--;
		u8_StrLn--;
	}
}




//==============================================================================
// DESCRIPTION : _Util_ByteSwap
//
// PARAMETERS (Name,(Type,Direction,Min/Max) : Description) :
//    none
//
// RETURN VALUE ((Type,Min/Max) : Description) :
//    none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
extern uns16 UTILS_crc16(const uns8 *data, uns16 size)
{
    uns16 out = 0;
    s32 bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
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
// Main function
//==============================================================================
extern uns32 UTILS_User_s32abs(s32 value)
{
    if(value < 0)
        return -value;
    else
        return value;
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
// Main function
//==============================================================================
extern float UTILS_User_fabs(float value)
{
    if(value < 0)
        return -value;
    else
        return value;
}


