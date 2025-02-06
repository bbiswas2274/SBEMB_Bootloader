#ifndef __TYPES_H 

#define __TYPES_H

	#include "stm32f7xx_hal.h"

	typedef enum _BOOL { FALSE = 0, TRUE } BOOL;	// Undefined size
	//typedef enum _bool { false = 0, true } bool;

	typedef uint64_t  uns64;
	typedef int64_t  s64;
	typedef int32_t  s32;
	typedef int16_t  s16;
	typedef int8_t  s8;

	typedef uint32_t  uns32;
	typedef uint16_t uns16;
	typedef uint8_t  uns8;

	typedef uint32_t uns;

	#ifndef NULL
	#define NULL ((void*)0)
	#endif

	#define UNDEF_U8_VALUE          0xff
	#define UNDEF_I8_VALUE          127
	#define UNDEF_I16_VALUE         32767
	#define UNDEF_UI16_VALUE        65535
	#define UNDEF_UI32_VALUE        0xFFFFFFFF
	#define UNDEF_I32_VALUE         0x7FFFFFFF

	#define UNDEF_TEMP_VALUE        UNDEF_I16_VALUE
	#define UNDEF_VOLT_VALUE        UNDEF_UI16_VALUE
	#define UNDEF_CURR_VALUE        UNDEF_I32_VALUE

	typedef enum _TE_TYPE_OF
	{
		TE_TYPE_OF_UNS8 = 0,
		TE_TYPE_OF_S8,

		TE_TYPE_OF_UNS16,
		TE_TYPE_OF_S16,

		TE_TYPE_OF_UNS32,
		TE_TYPE_OF_S32


	}TE_TYPE_OF;



#endif

