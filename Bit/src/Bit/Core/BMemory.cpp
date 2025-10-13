#include "Bit/Core/BMemory.h"
#include "Platform/Platform.h"
#include "Logger.h"
#include <cstdio>
#include <cstring>

namespace BitEngine
{


struct MemoryStats
{
    u64 TotalAllocated;
    u64 TaggedAllocations[MEMORY_TAG_MAX_TAGS];

};

static const char* MemoryTagStrings[MEMORY_TAG_MAX_TAGS] = {

    "MEMORY_TAG_UNKOWN",
    "MEMORY_TAG_DARRAY",
    "MEMORY_TAG_ARRAY",
    "MEMORY_TAG_DICT",
    "MEMORY_TAG_HASHTABLE",
    "MEMORY_TAG_STRING",
    "MEMORY_TAG_APPLICATION",
    "MEMORY_TAG_GAME"
};
static MemoryStats* memoryStats = 0;

b8 MemoryInitialize(u64 *memoryRequirement, void *state)
{
    *memoryRequirement = sizeof(MemoryStats);
    if(!state)
    {
        return false;
    }
    memoryStats = (MemoryStats*)state;
    PlatformZeroMemory(memoryStats, sizeof(MemoryStats));
    return true;
}
void MemoryShutdown(void *state)
{
    if(state)
    {
        memoryStats = 0;
        return;
    }
}

void* CMemoryAllocate(u64 size, MEMORY_TAG memoryTag)
{

    if(memoryTag == MEMORY_TAG_UNKOWN)
    {
        BIT_LOG_WARN("CAllocate Called With Unkown Memory Tag, Change the MemoryTag Type");
    }
    memoryStats->TotalAllocated += size;
    memoryStats->TaggedAllocations[memoryTag] += size;
    
    //TODO: memory alignment
    void* block = PlatformAllocate(size, false);
    PlatformZeroMemory(block, size);

    return block;
}
void CMemoryFree(void *block, u64 size, MEMORY_TAG memoryTag)
{
    if(memoryTag == MEMORY_TAG_UNKOWN)
    {
        BIT_LOG_WARN("CAllocate Called With Unkown Memory Tag, Change the MemoryTag Type");
    }

    memoryStats->TotalAllocated -= size;
    memoryStats->TaggedAllocations[memoryTag] -= size;

    PlatformFree(block, false);
}
void CMemorySet(void *dest, i32 value, u64 size)
{
    PlatformSetMemory(dest, value, size);
}

void CMemoryCopy(void *dest, const void *source, u64 size)
{
    PlatformCopyMemory(dest, source, size);
}
char* GetMemoryUsageStr()
{
    u64 kib = 1024;
    u64 mib = 1024 * kib;
    u64 gib = 1024 * mib; 

    char buffer[8000] = "System Memory Use (Tagged):\n";
    u64 offset = strlen(buffer);
    for(u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i)
    {
        char unit[4] = "XiB";
        f32 amount = 1.0f;
        if(memoryStats->TaggedAllocations[i] >= gib)
        {
            unit[0] = 'G';
            amount = memoryStats->TaggedAllocations[i] / (f32)gib;
        }
        else if(memoryStats->TaggedAllocations[i] >= mib)
        {
            unit[0] = 'M';
            amount = memoryStats->TaggedAllocations[i] / (f32)mib;
        }
        else if(memoryStats->TaggedAllocations[i] >= kib)
        {
            unit[0] = 'K';
            amount = memoryStats->TaggedAllocations[i] / (f32)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = memoryStats->TaggedAllocations[i];
        }
        offset += snprintf(buffer + offset, 8000, " %s: %.2f%s\n", MemoryTagStrings[i], amount, unit);
    }
    char* stringOut = strdup(buffer);
    return stringOut;
}
}

