#pragma once

#include "Bit/Core/Defines.h"
#include <array>
namespace BitEngine
{
struct EventContext
{
    //data is 128 bytes in total
    union data
    {
        i64 i64[2];
        u64 u64[2];
        
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

    };
};
enum class EVENT_CODE : u8
{
    APPLICATION_QUIT = 0x01,
    KEY_PRESSED = 0x02,
    KEY_RELEASED = 0x03,
    BUTTON_PRESSED = 0x04,
    BUTTON_RELEASED = 0x05,
    MAX = 0xFF,

};
typedef b8 (*PFN_ON_EVENT)(u16 code, void* sender, void* listener, EventContext data);

struct RegisteredEvent
{
    void* Listener;    
    PFN_ON_EVENT Callback;
};
#define MAX_MESSAGE_CODE 4096

struct RegisteredEvents
{
    std::vector<RegisteredEvent> events;
};
class Event
{
private:
// TODO: Change it to my custom made containter
    std::array<RegisteredEvents, MAX_MESSAGE_CODE> m_Registered;
    b8 m_Initialized = false;

public:
    b8 Initialize();
    void Shutdown();
    b8 Register(u16 code, void* listener, PFN_ON_EVENT callback);
    b8 UnRegister(u16 code, void* listener, PFN_ON_EVENT callback);
    b8 EventFire(u16 code, void* sender, EventContext data);


};
}
