#include "Event.h"
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"

namespace BitEngine
{

static std::array<RegisteredEvents, MAX_MESSAGE_CODE> m_Registered;
b8 EventManager::Initialize()
{
    if(m_Initialized)
    {
        BIT_LOG_ERROR("Input Already Initialized");
        return false;
    }
    m_Initialized = true;
    return true;
}
void EventManager::Shutdown()
{
    for(auto& registered : m_Registered)
    {
        registered.events.clear();
    }
    BIT_LOG_INFO("Event System Is shutting down..!");
}
b8 EventManager::Register(u16 code, void* listener, PFN_ON_EVENT callback)
{
    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    
    if(!m_Registered[code].events.empty())
    {
        u64 registeredCount = m_Registered[code].events.size();
        for(u64 i = 0; i < registeredCount; ++i)
        {
            const RegisteredEvent& event = m_Registered[code].events[i];
            if(event.Listener == listener && event.Callback == callback)
            {
                BIT_LOG_ERROR("This Event with code %u and with address %p is already registered..!", code, callback);
                return false;
            }
        }
    }
    RegisteredEvent newEvent;
    newEvent.Listener = listener;
    newEvent.Callback = callback;
    m_Registered[code].events.push_back(newEvent);
    return true;
}
b8 EventManager::UnRegister(u16 code, void* listener, PFN_ON_EVENT callback)
{

    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    u64 registeredCount = m_Registered[code].events.size();
    for(u64 i = 0; i < registeredCount; ++i)
    {
        const RegisteredEvent& event = m_Registered[code].events[i];
        if(event.Listener == listener && event.Callback == callback)
        {
            m_Registered[code].events[i] = m_Registered[code].events.back();
            m_Registered[code].events.pop_back();
            return true;
        }
    }
    return false;

}
b8 EventManager::EventFire(u16 code, void* sender, EventContext data)
{
    if(code >= MAX_MESSAGE_CODE)
    {
        BIT_LOG_ERROR("Event Code Is Bigger than the size %d that the engine have", MAX_MESSAGE_CODE);
        return false;
    }
    u64 registeredCount = m_Registered[code].events.size();
    for(u64 i = 0; i < registeredCount; ++i)
    {
        const RegisteredEvent& event = m_Registered[code].events[i];
        if(event.Callback(code, sender, event.Listener, data))
        {
            //event has been handled for all listeners
            return true;
        }
    }
    return false;
}
}
