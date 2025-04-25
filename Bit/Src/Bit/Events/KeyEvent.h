#include "Event.h"
namespace BitEngine 
{

class KeyPressedEvent : public Event
{
    
public:
    KeyPressedEvent(unsigned int keyCode)
        : m_KeyCode(keyCode), m_IsRepeated(true)
    {

    }
    EVENT_CLASS_TYPE(KEY_PRESSED);
private:
    unsigned int m_KeyCode;
    bool m_IsRepeated;
};

class KeyReleasedEvent : public Event
{
    
public:
    KeyReleasedEvent(unsigned int keyCode)
        : m_KeyCode(keyCode) 
    {

    }
    EVENT_CLASS_TYPE(KEY_RELEASED);
private:
    unsigned int m_KeyCode;
};

}
