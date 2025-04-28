#include "Event.h"
#include <sstream>

namespace BitEngine 
{

class WindowResizeEvent : public Event
{

public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height)
    {

    }
    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent" << m_Width << ", " << m_Height;
        return ss.str();

    }

    EVENT_CLASS_TYPE(WINDOW_RESIZE);
private:

    unsigned int m_Width, m_Height;
};

}
