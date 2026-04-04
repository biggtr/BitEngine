#pragma once 
#include "Bit/Core/Defines.h"

namespace BitEngine
{
class GraphicsContext;


b8 PlatformInitialize(u64* memoryRequirement, void* platformState);
b8 PlatformStartup(const char* applicationName, i32 x, i32 y, u32 width, u32 height);
b8 PlatformPumpMessages();
void PlatformShutdown(void* platformState);

void PlatformSwapBuffers();
void PlatformHideCursor();
void PlatformSetCursorPos(i32 posx, i32 posy);

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

