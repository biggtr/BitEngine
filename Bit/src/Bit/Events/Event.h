#include <string.h>

namespace BitEngine 
{
    
enum class EVENT_TYPE
{
    WINDOW_RESIZE, WINDOW_CLOSE,
    KEY_PRESSED, KEY_RELEASED,
    MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, 
};

#define EVENT_CLASS_TYPE(type) static EVENT_TYPE GetStaticType() { return EVENT_TYPE::type; }\
                                                                 virtual EVENT_TYPE GetEventType() const override { return EVENT_TYPE::type; }\
                                                                 virtual const char* GetName() const override { return #type; }
class Event 
{
public:
    virtual ~Event() = default;

    EVENT_TYPE m_EventType;
    bool m_Handled = false;
    
    virtual EVENT_TYPE GetEventType() const = 0;
    static EVENT_TYPE GetStaticType();
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const  { return GetName(); }

};



class EventDispatcher 
{
public:
    EventDispatcher (Event& event)
        : m_Event(event)
    {

    }

    template<typename T, typename F>
    bool Dispatch(const F& func)
    {

        if(m_Event.GetEventType() == T::GetStaticType)
        {
            m_Event.m_Handled |= func<static_cast<T&>(m_Event); //if event is already handled dont run func -> handled || callback
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

}
