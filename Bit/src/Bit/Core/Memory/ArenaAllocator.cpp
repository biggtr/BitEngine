#include "ArenaAllocator.h"
#include "Bit/Core/Logger.h"

namespace BitEngine
{
void ArenaCreate(ArenaAllocator* arena, u64 size, void* memory)
{
    arena->Memory = memory;
    arena->TotalSize = size;
    arena->Offset = 0;
}
void* ArenaAllocate(ArenaAllocator* arena, u64 size)
{
    if(arena->Offset + size > arena->TotalSize)
    {
        BIT_LOG_ERROR("Arena allocator out of memory! Requested: %llu, Available: %llu",
                size, arena->TotalSize - arena->Offset);
        return nullptr;
    }
    void* ptr = (u8*)arena->Memory + arena->Offset; // current position before adding the offset and moving forward
    arena->Offset += size;
    return ptr;
}
void ArenaReset(ArenaAllocator* arena)
{
    arena->Offset = 0;
}
void ArenaClear(ArenaAllocator* arena)
{
    arena->Memory = nullptr;
    arena->Offset = 0;
    arena->TotalSize = 0;
}
}
