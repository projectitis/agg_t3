#ifndef AGG_CONFIG_INCLUDED
#define AGG_CONFIG_INCLUDED

#include <stdint.h>
#include <float.h>

// This file can be used to redefine certain data types.

//---------------------------------------
// 1. Default basic types such as:
// 
// AGG_INT8
// AGG_INT8U
// AGG_INT16
// AGG_INT16U
// AGG_INT32
// AGG_INT32U
// AGG_INT64
// AGG_INT64U
//
// Just replace this file with new defines if necessary. 
// For example, if your compiler doesn't have a 64 bit integer type
// you can still use AGG if you define the follows:
//
// #define AGG_INT64  int
// #define AGG_INT64U unsigned
//
// It will result in overflow in 16 bit-per-component image/pattern resampling
// but it won't result any crash and the rest of the library will remain 
// fully functional.
#define AGG_INT8 	int8_t
#define AGG_INT8U 	uint8_t
#define AGG_INT16 	int16_t
#define AGG_INT16U 	uint16_t
#define AGG_INT32 	int32_t
#define AGG_INT32U 	uint32_t

#ifdef UINT64_MAX

#define AGG_INT64 	int64_t
#define AGG_INT64U 	uint64_t

#elif

#define AGG_INT64 	int32_t
#define AGG_INT64U 	uint32_t

#endif

// Float/Double types added by Peter "Projectitis" Vullings (peter@projectitis.com)
// Added to allow using library with float instead of double for better performance
// on micorprocessor systems with native floating point support.
// To revert back to double, comment out the AGG_USE_FLOAT_INSTEAD_OF_DOUBLE define:
//#define AGG_USE_FLOAT_INSTEAD_OF_DOUBLE

#define AGG_FLT 	float

#ifdef AGG_USE_FLOAT_INSTEAD_OF_DOUBLE

#define AGG_DBL 	float
#define AGG_DBL_MIN	FLT_MIN
#define AGG_DBL_MAX	FLT_MAX

#else

#define AGG_DBL 	double
#define AGG_DBL_MIN	DBL_MIN
#define AGG_DBL_MAX	DBL_MAX

#endif

//---------------------------------------
// 2. Default rendering_buffer type. Can be:
//
// Provides faster access for massive pixel operations, 
// such as blur, image filtering:
// #define AGG_RENDERING_BUFFER row_ptr_cache<int8u>
// 
// Provides cheaper creation and destruction (no mem allocs):
// #define AGG_RENDERING_BUFFER row_accessor<int8u>
//
// You can still use both of them simultaneously in your applications 
// This #define is used only for default rendering_buffer type,
// in short hand typedefs like pixfmt_rgba32.

// ---------------------------------------
// 3. Misc defines
//
// Defines added by Peter "Projectitis" Vullings (peter@projectitis.com)
// these are various defines found in teh source code that would normally
// be in a makefile or similar. No makefile is being used in the
// Arduino environment I am using, so adding them here as I find them:
//
// Something to do with x86 (https://msdn.microsoft.com/en-us/library/6d9xx1d2.aspx)
// @see agg_basics.h
// #define AGG_FISTP
// #define AGG_QIFIST
//
// I'm assuming this means "No ASM" :D
// @see agg_math.h
// #define AGG_NO_ASM
//
// Something to do with win32 fonts.
// @see agg_font_win32_tt.cpp
// #define AGG_WIN9X_COMPLIANT
//
// Custom memory allocator and deallocator (if you want them!)
// @see agg_basics.h
// #define AGG_CUSTOM_ALLOCATOR

#endif
