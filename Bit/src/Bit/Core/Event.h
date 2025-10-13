#pragma once
#include "Bit/Core/Defines.h"
struct EventContext
{
    //data is 128 bytes in total
    union 
    {
        i64 I64[2];
        u64 U64[2];
        
        f64 F64[2];

        i32 I32[4];
        u32 U32[4];
        f32 F32[4];

        i16 I16[8];
        u16 U16[8];

        i8 I8[16];
        u8 U8[16];

        union
        {
            u64 Size;
            void* Data;
        };

    };
};
typedef enum 
{
    EVENT_CODE_APPLICATION_QUIT = 0x01,
    EVENT_CODE_KEY_PRESSED = 0x02,
    EVENT_CODE_KEY_RELEASED = 0x03,
    EVENT_CODE_BUTTON_PRESSED = 0x04,
    EVENT_CODE_BUTTON_RELEASED = 0x05,
    EVENT_CODE_MOUSE_MOVE = 0x06,
    EVENT_CODE_MOUSE_WHEEL = 0x07,
    EVENT_CODE_WINDOW_RESIZED = 0x08,
    EVENT_CODE_ENTITY_CREATE = 0x09,
    EVENT_CODE_ENTITY_DESTROY = 0x0A,
    EVENT_CODE_MAX = 0xFF,

}EVENT_CODE;
typedef b8 (*PFN_ON_EVENT)(u16 code, void* sender, void* listener, EventContext data);


b8 EventInitialize(u64* memoryRequirement, void* state);
void EventShutdown(void* state);
b8 EventRegister(u16 code, void* listener, PFN_ON_EVENT callback);
b8 EventUnRegister(u16 code, void* listener, PFN_ON_EVENT callback);
b8 EventFire(u16 code, void* sender, EventContext data);


