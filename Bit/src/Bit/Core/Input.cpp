#include "Input.h"
#include <cstring>
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"

namespace BitEngine 
{
namespace BInput
{
static InputState* m_InputState;

b8 m_Initialized = false;

b8 Initialize()
{
    if(m_Initialized)
    {
        BIT_LOG_ERROR("Input Already Initialized");
        return false;
    }
    m_InputState = new InputState();
    m_Initialized = true;
    return m_Initialized;
}
void Shutdown()
{
    if(!m_Initialized)
    {
        BIT_LOG_ERROR("Input Is Not Initialized Already");
        return;
    }
    delete m_InputState;
    m_Initialized = false;
    BIT_LOG_INFO("Input Is Shutting down..");
}
void Update()
{
    memcpy(&m_InputState->KeyboardPrevState, &m_InputState->KeyboardCurrentState, sizeof(KeyboardState));
    memcpy(&m_InputState->MousePrevState, &m_InputState->MouseCurrentState, sizeof(MouseState));
}
b8 IsKeyDown(KEYS key)
{
    return m_InputState->KeyboardCurrentState.Keys[(u16)key] == true;
}
b8 IsKeyUp(KEYS key)
{
    return m_InputState->KeyboardCurrentState.Keys[(u16)key] == false;
}
b8 WasKeyDown(KEYS key)
{
    return m_InputState->KeyboardPrevState.Keys[(u16)key] == true;
}
b8 WasKeyUp(KEYS key)
{
    return m_InputState->KeyboardPrevState.Keys[(u16)key] == false;
}

b8 IsMouseButtonDown(MOUSE_BUTTONS button)
{
    return m_InputState->MouseCurrentState.Buttons[(u8)button] == true;
}
b8 IsMouseButtonPressed(MOUSE_BUTTONS button)
{
    b8 current = m_InputState->MouseCurrentState.Buttons[(u8)button];
    b8 previous = m_InputState->MousePrevState.Buttons[(u8)button];
    b8 result = current && !previous;
    
    return result;
}
b8 IsMouseButtonUp(MOUSE_BUTTONS  button)
{
    return m_InputState->MouseCurrentState.Buttons[(u8)button] == false;
}
b8 IsMouseButtonReleased(MOUSE_BUTTONS button)
{
    b8 current = m_InputState->MouseCurrentState.Buttons[(u8)button];
    b8 previous = m_InputState->MousePrevState.Buttons[(u8)button];
    b8 result = !current && previous;

    return result;
}
b8 WasMouseButtonDown(MOUSE_BUTTONS  button)
{
    return m_InputState->MousePrevState.Buttons[(u8)button] == true;
}
b8 WasMouseButtonUp(MOUSE_BUTTONS  button)
{
    return m_InputState->MousePrevState.Buttons[(u8)button] == false;
}
void GetMousePosition(i32* x, i32* y)
{
    *x = m_InputState->MouseCurrentState.X;
    *y = m_InputState->MouseCurrentState.Y;
}
void GetPrevMousePosition(i32* x, i32* y)
{
    *x = m_InputState->MousePrevState.X;
    *y = m_InputState->MousePrevState.Y;
}
void ProcessKey(KEYS key, b8 pressed)
{
    EventContext context;
    context.U16[0] = key;
    EventManager::EventFire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    m_InputState->KeyboardCurrentState.Keys[key] = pressed;
}
void ProcessMouseButton(MOUSE_BUTTONS button, b8 pressed)
{
    // BIT_LOG_DEBUG("ProcessMouseButton: button=%d, pressed=%d", (int)button, pressed);
    EventContext context;
    context.U16[0] = button;
    context.U16[1] = m_InputState->MouseCurrentState.X;
    context.U16[2] = m_InputState->MouseCurrentState.Y;
    EventManager::EventFire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED , 0, context);
    m_InputState->MouseCurrentState.Buttons[button] = pressed;
    // BIT_LOG_DEBUG("Mouse button state updated: button[%d] = %d", (int)button, pressed);
}

void ProcessMouseMove(i16* x, i16* y)
{
    EventContext context;
    context.I16[0] = *x;
    context.I16[1] = *y;
    EventManager::EventFire(EVENT_CODE_MOUSE_MOVE, 0, context);
    m_InputState->MouseCurrentState.X = *x;
    m_InputState->MouseCurrentState.Y = *y;
}
void ProcessMouseWheel(i8 delta)
{
    EventContext context;
    context.I8[0] = delta;
    EventManager::EventFire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

}   

}


