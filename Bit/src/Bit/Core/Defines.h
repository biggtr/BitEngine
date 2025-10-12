#pragma once 


#include <stdint.h>

typedef uint32_t Signature;
const unsigned int MAX_COMPONENTS = 32;
// Unsigned int types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed int types
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


typedef bool b8;
typedef int32_t b32; 

// Floating point types
typedef float f32;
typedef double f64;

#if defined (__clang__) || defined (__gcc__)
#define STATIC_ASSERT _Static_assert 
#else
#define STATIC_ASSERT static_assert
#endif


STATIC_ASSERT(sizeof(i8)  == 1, "Size of int8 should be 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "Size of int16 should be 2 byte");
STATIC_ASSERT(sizeof(i32) == 4, "Size of int32 should be 4 byte");
STATIC_ASSERT(sizeof(i64) == 8, "Size of int64 should be 8 byte");

STATIC_ASSERT(sizeof(u8)  == 1, "Size of uint8  should be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "Size of uint16 should be 2 byte");
STATIC_ASSERT(sizeof(u32) == 4, "Size of uint32 should be 4 byte");
STATIC_ASSERT(sizeof(u64) == 8, "Size of uint64 should be 8 byte");

STATIC_ASSERT(sizeof(f32) == 4, "Size of float32 should be 4 byte");
STATIC_ASSERT(sizeof(f64) == 8, "Size of float64 should be 8 byte");
#define TRUE 1
#define FALSE 0 



// Windows
#if defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
#define BPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "The system is not 64-bit windows "
#endif

// Linux 
#elif defined(__linux__) || defined(__gnu_linux__)
#define BPLATFORM_LINUX 1

#endif
