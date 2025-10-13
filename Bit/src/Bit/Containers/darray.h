#pragma once

#include "Bit/Core/Defines.h"


struct DArrayHeader
{
    u64 Capacity; // how much size can the darray hold 
    u64 Stride; // size of Each Element in the array
    u64 Length; // current number of elements in darray 
};
enum
{
    DARRAY_CAPACITY,
    DARRAY_STRIDE,
    DARRAY_LENGTH,
};

void* _DArrayCreate(u64 Capacity, u64 stride);
void _DArrayDestroy(void* array);

void* _DArrayPush(void* array, const void* valuePtr);
void _DArrayPop(void* array, void* dest);

inline DArrayHeader* _DarrayHeader(void* array){ return ((DArrayHeader*)array) - 1;}

u64 _DArrayFieldGet(void* array, u64 field);
void _DArrayFieldSet(void* array, u64 field, u64 value);

void* _DArrayResize(void* array);

void* _DArrayPushAt(void* array, u64 index, void* valuePtr);
void* _DArrayPopAt(void* array, u64 index, void* dest);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#ifdef _MSC_VER 
    #define TYPEOF_KEYWORD decltype
#else 
    #define TYPEOF_KEYWORD typeof
#endif
#define DArrayCreate(type) \
    _DArrayCreate(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define DArrayReserve(type, arrayCapacity) \
    _DArrayCreate(arrayCapacity, sizeof(type))

#define DArrayDestroy(array) \
    _DArrayDestroy(array)

#define DArrayPush(array, value) \
    TYPEOF_KEYWORD(value) temp = value; \
    _DArrayPush(array, &temp); \

#define DArrayPop(array, valuePtr) \
    _DArrayPop(array, valuePtr); \

#define DArrayInsertAt(array, index, value) \
    TYPEOF_KEYWORD(value) temp = value; \
    _DArrayPushAt(array, index, &value) \

#define DArrayPopAt(array, index, valuePtr) \
    _DArrayPopAt(array, index, valuePtr) \

#define DArrayClear(array) \
    _DArrayFieldSet(array, DARRAY_LENGTH, 0)

#define DArrayCapacity(array) \
    _DArrayFieldGet(array, DARRAY_CAPACITY)

#define DArrayLength(array) \
    _DArrayFieldGet(array, DARRAY_LENGTH)

#define DArrayStride(array) \
    _DArrayFieldGet(array, DARRAY_STRIDE)

#define DArraySetLength(array, len) \
    _DArrayFieldSet(array, DARRAY_LENGTH, len)
