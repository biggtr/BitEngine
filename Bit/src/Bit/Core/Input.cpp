#include "Input.h"
#include <cstring>
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"

namespace BitEngine 
{

static InputState* inputState = 0;
b8 InputInitialize(u64* memoryRequirement, void* state)
{
    *memoryRequirement = sizeof(InputState);
    if(!state)
    {
        return true;
    }
    inputState = (InputState*)state;
    memset(inputState, 0, sizeof(InputState));
    return true;
}
void InputShutdown(void* state)
{
    if(state)
    {
        inputState = 0;
    }
    BIT_LOG_INFO("Event System Is shutting down..!");
}
void InputUpdate(f32 deltaTime)
{
    memcpy(&inputState->KeyboardPrevState, &inputState->KeyboardCurrentState, sizeof(KeyboardState));
    memcpy(&inputState->MousePrevState, &inputState->MouseCurrentState, sizeof(MouseState));
}
b8 InputIsKeyDown(KEYS key)
{
    return inputState->KeyboardCurrentState.Keys[(u16)key] == true;
}
b8 InputIsKeyUp(KEYS key)
{
    return inputState->KeyboardCurrentState.Keys[(u16)key] == false;
}
b8 InputWasKeyDown(KEYS key)
{
    return inputState->KeyboardPrevState.Keys[(u16)key] == true;
}
b8 InputWasKeyUp(KEYS key)
{
    return inputState->KeyboardPrevState.Keys[(u16)key] == false;
}

b8 InputIsMouseButtonDown(MOUSE_BUTTONS button)
{
    return inputState->MouseCurrentState.Buttons[(u8)button] == true;
}
b8 InputIsMouseButtonPressed(MOUSE_BUTTONS button)
{
    b8 current = inputState->MouseCurrentState.Buttons[(u8)button];
    b8 previous = inputState->MousePrevState.Buttons[(u8)button];
    b8 result = current && !previous;
    
    return result;
}
b8 InputIsMouseButtonUp(MOUSE_BUTTONS  button)
{
    return inputState->MouseCurrentState.Buttons[(u8)button] == false;
}
b8 InputIsMouseButtonReleased(MOUSE_BUTTONS button)
{
    b8 current = inputState->MouseCurrentState.Buttons[(u8)button];
    b8 previous = inputState->MousePrevState.Buttons[(u8)button];
    b8 result = !current && previous;

    return result;
}
b8 InputWasMouseButtonDown(MOUSE_BUTTONS  button)
{
    return inputState->MousePrevState.Buttons[(u8)button] == true;
}
b8 InputWasMouseButtonUp(MOUSE_BUTTONS  button)
{
    return inputState->MousePrevState.Buttons[(u8)button] == false;
}
void InputGetMousePosition(i32* x, i32* y)
{
    *x = inputState->MouseCurrentState.X;
    *y = inputState->MouseCurrentState.Y;
}
void InputGetPrevMousePosition(i32* x, i32* y)
{
    *x = inputState->MousePrevState.X;
    *y = inputState->MousePrevState.Y;
}
void InputProcessKey(KEYS key, b8 pressed)
{
    EventContext context;
    context.U16[0] = key;
    EventFire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    inputState->KeyboardCurrentState.Keys[key] = pressed;
}
void InputProcessMouseButton(MOUSE_BUTTONS button, b8 pressed)
{
    // BIT_LOG_DEBUG("InputProcessMouseButton: button=%d, pressed=%d", (int)button, pressed);
    EventContext context;
    context.U16[0] = button;
    context.U16[1] = inputState->MouseCurrentState.X;
    context.U16[2] = inputState->MouseCurrentState.Y;
    EventFire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED , 0, context);
    inputState->MouseCurrentState.Buttons[button] = pressed;
    // BIT_LOG_DEBUG("Mouse button state updated: button[%d] = %d", (int)button, pressed);
}

void InputProcessMouseMove(i16* x, i16* y)
{
    EventContext context;
    context.I16[0] = *x;
    context.I16[1] = *y;
    EventFire(EVENT_CODE_MOUSE_MOVE, 0, context);
    inputState->MouseCurrentState.X = *x;
    inputState->MouseCurrentState.Y = *y;
}
void InputProcessMouseWheel(i8 delta)
{
    EventContext context;
    context.I8[0] = delta;
    EventFire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

}   




