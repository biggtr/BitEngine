#pragma once
#include "Bit/Core/Defines.h"

namespace BitEngine
{
struct ArenaAllocator
{
    void* Memory; // the start of the big block of memory
    u64 TotalSize; 
    u64 Offset;  // where the next allocation goes
};

void ArenaCreate(ArenaAllocator* arena, u64 size, void* memory);
void* ArenaAllocate(ArenaAllocator* arena, u64 size);
void ArenaReset(ArenaAllocator* arena); // reset the offset to 0 doesnt free
void ArenaClear(ArenaAllocator* arena); 
}
