#include "Event.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Logger.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"
#include <cstring>

struct RegisteredEvent
{
    void* Listener;    
    PFN_ON_EVENT Callback;
};
#define MAX_MESSAGE_CODE 4096

struct ListenersForSpecCode
{
    RegisteredEvent* Events;
};
struct EventState
{
    ListenersForSpecCode m_Registered[MAX_MESSAGE_CODE];
};
static EventState* statePtr = 0;
b8 EventInitialize(u64* memoryRequirement, void* state)
{
    *memoryRequirement = sizeof(EventState);
    if(!state)
    {
        return true;
    }
    statePtr = (EventState*)state;
    memset(statePtr, 0, sizeof(EventState));
    return true;
}
void EventShutdown(void* state)
{
    for(u16 i = 0; i < MAX_MESSAGE_CODE; ++i)
    {
        if(statePtr->m_Registered[i].Events != 0)
        {
            DArrayDestroy(statePtr->m_Registered[i].Events);
            statePtr->m_Registered[i].Events = 0;
        }
    }
    if(state)
    {
        statePtr = 0;
    }
    BIT_LOG_INFO("Event System Is shutting down..!");
}
b8 EventRegister(u16 code, void* listener, PFN_ON_EVENT callback)
{
    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    
    if(!statePtr->m_Registered[code].Events)
    {
        statePtr->m_Registered[code].Events = (RegisteredEvent*)DArrayCreate(RegisteredEvent);
    }
    u64 registeredCount = DArrayLength(statePtr->m_Registered[code].Events);
    for(u64 i = 0; i < registeredCount; ++i)
    {
        if(statePtr->m_Registered[code].Events->Listener == listener && statePtr->m_Registered[code].Events->Callback == callback)
        {
            BIT_LOG_ERROR("This Event with code %u and with address %p is already registered..!", code, callback);
            return false;
        }
    }
    
    RegisteredEvent newEvent;
    newEvent.Listener = listener;
    newEvent.Callback = callback;
    DArrayPush(statePtr->m_Registered[code].Events, newEvent);
    return true;
}
b8 EventUnRegister(u16 code, void* listener, PFN_ON_EVENT callback)
{

    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    u64 registeredCount = DArrayLength(statePtr->m_Registered[code].Events);
    for(u64 i = 0; i < registeredCount; ++i)
    {
        RegisteredEvent e = statePtr->m_Registered[code].Events[i];
        if(e.Listener == listener && e.Callback == callback)
        {
            RegisteredEvent poppedEvent;
            DArrayPopAt(statePtr->m_Registered[code].Events, i, &poppedEvent);
            return true;
        }
    }
    return false;

}
b8 EventFire(u16 code, void* sender, EventContext data)
{
    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    if(statePtr->m_Registered[code].Events == 0)
    {
        return false;
    }
    u64 registeredCount = DArrayLength(statePtr->m_Registered[code].Events);
    for(u64 i = 0; i < registeredCount; ++i)
    {
        RegisteredEvent event = statePtr->m_Registered[code].Events[i];
        if(event.Callback(code, sender, event.Listener, data))
        {
            //event has been handled for all listeners
            return true;
        }
    }
    return false;
}

