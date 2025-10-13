#pragma once 
#include "Bit/Core/Defines.h"

namespace BitEngine
{
class GraphicsContext;

struct PlatformState
{
    void* InternalState;
    u32 Width;
    u32 Height;
    const char* Name;
    GraphicsContext* Context;
};


b8 PlatformStartup(PlatformState* platformState, const char* applicationName, i32 x, i32 y, u32 width, u32 height);
b8 PlatformPumpMessages(PlatformState* platformState);
void PlatformShutdown(PlatformState* platformState);

void* PlatformAllocate(u64 size, b8 aligned);
void PlatformFree(void* block, b8 aligned);
void PlatformZeroMemory(void* block, u64 size);
void PlatformSetMemory(void* dest, i32 value, u64 size);
void PlatformCopyMemory(void* dest, const void* source, u64 size);

void PlatformConsoleWrite(const char* message, u8 color);
void PlatformConsoleWriteError(const char* message, u8 color);

f64 PlatformGetAbsoluteTime();

void PlatformSleep(u64 ms);

}

