#pragma once 
#include "Bit/Core/Defines.h"

namespace BitEngine
{
    
enum MEMORY_TAG
{
    MEMORY_TAG_UNKOWN,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_HASHTABLE,
    MEMORY_TAG_STRING,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_ENGINE,
    MEMORY_TAG_GAME,
    MEMORY_TAG_MAX_TAGS
};


b8 MemoryInitialize(u64* memoryRequirement, void* state);
void MemoryShutdown(void* state);

void* CMemoryAllocate(u64 size, MEMORY_TAG memoryTag);

void CMemoryFree(void* block, u64 size, MEMORY_TAG memoryTag);

void* CMemoryZero(void* block, u64 size);

void CMemoryCopy(void* dest, const void* source, u64 size);

void CMemorySet(void* dest, i32 value, u64 size);

char* GetMemoryUsageStr();
}
