#include "Input.h"
#include <cstring>
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"


namespace BitEngine
{
static InputState m_InputState{};
b8 Input::Initialize()
{
    if(m_Initialized)
    {
        BIT_LOG_ERROR("Input Already Initialized");
        return false;
    }
    memset(&m_InputState, 0, sizeof(m_InputState));

    m_Initialized = true;
    return m_Initialized;
}
void Input::Shutdown()
{
    if(!m_Initialized)
    {
        BIT_LOG_ERROR("Input Is Not Initialized Already");
        return;
    }
    m_Initialized = false;
    BIT_LOG_INFO("Input Is Shutting down..");
}
void Input::Update()
{
    memcpy(&m_InputState.KeyboardPrevState, &m_InputState.KeyboardCurrentState, sizeof(KeyboardState));
    memcpy(&m_InputState.MousePrevState, &m_InputState.MouseCurrentState, sizeof(MouseState));
}
b8 Input::IsKeyDown(KEYS key)
{
    return m_InputState.KeyboardCurrentState.Keys[(u16)key] == true;
}
b8 Input::IsKeyUp(KEYS key)
{
    return m_InputState.KeyboardCurrentState.Keys[(u16)key] == false;
}
b8 Input::WasKeyDown(KEYS key)
{
    return m_InputState.KeyboardPrevState.Keys[(u16)key] == true;
}
b8 Input::WasKeyUp(KEYS key)
{
    return m_InputState.KeyboardPrevState.Keys[(u16)key] == false;
}

b8 Input::IsMouseButtonDown(MOUSE_BUTTONS button)
{
    return m_InputState.MouseCurrentState.Buttons[(u8)button] == true;
}
b8 Input::IsMouseButtonUp(MOUSE_BUTTONS  button)
{
    return m_InputState.MouseCurrentState.Buttons[(u8)button] == false;
}
b8 Input::WasMouseButtonDown(MOUSE_BUTTONS  button)
{
    return m_InputState.MousePrevState.Buttons[(u8)button] == true;
}
b8 Input::WasMouseButtonUp(MOUSE_BUTTONS  button)
{
    return m_InputState.MousePrevState.Buttons[(u8)button] == false;
}
void Input::GetMousePosition(i32* x, i32* y)
{
    *x = m_InputState.MouseCurrentState.X;
    *y = m_InputState.MouseCurrentState.Y;
}
void Input::GetPrevMousePosition(i32* x, i32* y)
{
    *x = m_InputState.MousePrevState.X;
    *y = m_InputState.MousePrevState.Y;
}
void Input::ProcessKey(KEYS key, b8 pressed)
{
    EventContext context;
    context.U16[0] = key;
    EventManager::EventFire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    m_InputState.KeyboardCurrentState.Keys[key] = pressed;
}
void Input::ProcessMouseButton(MOUSE_BUTTONS button, b8 pressed)
{
    EventContext context;
    context.U16[0] = button;
    context.U16[1] = m_InputState.MouseCurrentState.X;
    context.U16[2] = m_InputState.MouseCurrentState.Y;
    EventManager::EventFire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED , 0, context);
    m_InputState.MouseCurrentState.Buttons[button] = pressed;
}

void Input::ProcessMouseMove(i16* x, i16* y)
{
    EventContext context;
    context.I16[0] = *x;
    context.I16[1] = *y;
    EventManager::EventFire(EVENT_CODE_MOUSE_MOVE, 0, context);
    m_InputState.MouseCurrentState.X = *x;
    m_InputState.MouseCurrentState.Y = *y;
}
void Input::ProcessMouseWheel(i8 delta)
{
    EventContext context;
    context.I8[0] = delta;
    EventManager::EventFire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}
}

