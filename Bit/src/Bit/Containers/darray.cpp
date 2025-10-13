#include "darray.h"
#include "Bit/Core/Logger.h"
#include <cstdlib>
#include <cstring>


void* _DArrayCreate(u64 capacity, u64 stride)
{
    if(capacity == 0) capacity = DARRAY_DEFAULT_CAPACITY;

    u64 headerSize = sizeof(DArrayHeader);
    u64 arraySize = capacity * stride;
    u64 totalSize = headerSize + arraySize;

    DArrayHeader* header = (DArrayHeader*)malloc(totalSize);
    if(!header) return 0;

    memset(header, 0, totalSize);
    header->Length = 0;
    header->Capacity = capacity;
    header->Stride = stride;

    return (void*)(header + 1); // skip the darray header 
}
void _DArrayDestroy(void* array)
{
    DArrayHeader* header = _DarrayHeader(array);
    free(header); // [24 bit for header | void* data] 
}
u64 _DArrayFieldGet(void* array, u64 field)
{
    DArrayHeader* header = _DarrayHeader(array);
    switch (field) 
    {
        case DARRAY_CAPACITY:
            return header->Capacity;
            break;
        case DARRAY_STRIDE:
            return header->Stride;
            break;
        case DARRAY_LENGTH:
            return header->Length;
            break;
        default:
            return 0;
    }
}
void _DArrayFieldSet(void* array, u64 field, u64 value)
{
    DArrayHeader* header = _DarrayHeader(array);
    switch (field) 
    {
        case DARRAY_CAPACITY:
            header->Capacity = value;
            break;
        case DARRAY_STRIDE:
            header->Stride = value;
            break;
        case DARRAY_LENGTH:
            header->Length = value;
            break;
    }
}

void* _DArrayResize(void* array)
{
    u64 stride = DArrayStride(array);
    u64 length = DArrayLength(array);
    void* temp = _DArrayCreate(DARRAY_RESIZE_FACTOR * DArrayCapacity(array), stride);
    memcpy(temp, array, length * stride);
    _DArrayFieldSet(temp, DARRAY_LENGTH, length);
    _DArrayDestroy(array);
    return temp;
}

void* _DArrayPush(void* array, const void* valuePtr)
{
    u64 length = DArrayLength(array);
    u64 stride = DArrayStride(array);
    if(length >= DArrayCapacity(array))
    {
        array = _DArrayResize(array);
    }
    u64 addr = (u64)array + (length * stride);
    memcpy((void*)addr, valuePtr, stride);
    _DArrayFieldSet(array, DARRAY_LENGTH, length + 1);
    return array;
}
void _DArrayPop(void* array, void* dest)
{
    u64 length = DArrayLength(array);
    u64 stride = DArrayStride(array);
    u64 addr =  (u64)array + ((length - 1) * stride);
    memcpy(dest, (void*)addr, stride);
    _DArrayFieldSet(array, DARRAY_LENGTH, length - 1);
}
void* _DArrayPopAt(void* array, u64 index, void* dest)
{
    u64 length = DArrayLength(array);
    u64 stride = DArrayStride(array);
    if(index >= length)
    {
        BIT_LOG_ERROR("Index Is Outside the boundries of the darray");
        return array;
    }
    u64 addr = (u64)array;
    memcpy(dest, (void*)(addr + (index * stride)), stride);
    if(index != length - 1)
    {
        memcpy(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }
    _DArrayFieldSet(array, DARRAY_LENGTH, length - 1);
    return array;
}

void* _DArrayPushAt(void* array, u64 index, void* valuePtr)
{
    u64 length = DArrayLength(array);
    u64 stride = DArrayStride(array);
    if(index >= length)
    {
        BIT_LOG_ERROR("Index Is Outside the boundries of the darray");
        return array;
    }
    if(length >= DArrayCapacity(array))
    {
        _DArrayResize(array);
    }
    u64 addr = (u64)array;
    if(index != length - 1)
    {
        memcpy(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }
    memcpy((void*)(addr + (index * stride)), valuePtr, stride);
    _DArrayFieldSet(array, DARRAY_LENGTH, length + 1);
    return array;
    
}



