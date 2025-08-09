#pragma once
#include "Defines.h"
namespace BitEngine
{

enum class MOUSE_BUTTONS : u8 
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,
    BUTTON_MAX
};

enum class KEYS : u16
{
    BACKSPACE = 0x08,
    ENTER = 0x0D,
    TAB = 0x09,
    SHIFT = 0x10,
    CONTROL = 0x11,

    PAUSE = 0x13,
    CAPITAL = 0x14,

    ESCAPE = 0x1B,

    CONVERT = 0x1C,
    NONCONVERT = 0x1D,
    ACCEPT = 0x1E,
    MODECHANGE = 0x1F,

    SPACE = 0x20,
    PAGEUP = 0x21,
    PAGEDOWN = 0x22,
    END = 0x23,
    HOME = 0x24,
    LEFT = 0x25,
    UP = 0x26,
    RIGHT = 0x27,
    DOWN = 0x28,
    SELECT = 0x29,
    PRINT = 0x2A,
    EXECUTE = 0x2B,
    PRINTSCREEN = 0x2C,
    INSERT = 0x2D,
    DELETE = 0x2E,
    HELP = 0x2F,

    KEY_0 = 0x30,
    KEY_1 = 0x31,
    KEY_2 = 0x32,
    KEY_3 = 0x33,
    KEY_4 = 0x34,
    KEY_5 = 0x35,
    KEY_6 = 0x36,
    KEY_7 = 0x37,
    KEY_8 = 0x38,
    KEY_9 = 0x39,

    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,

    LSUPER = 0x5B,
    RSUPER = 0x5C,
    APPS = 0x5D,

    SLEEP = 0x5F,

    NUMPAD0 = 0x60,
    NUMPAD1 = 0x61,
    NUMPAD2 = 0x62,
    NUMPAD3 = 0x63,
    NUMPAD4 = 0x64,
    NUMPAD5 = 0x65,
    NUMPAD6 = 0x66,
    NUMPAD7 = 0x67,
    NUMPAD8 = 0x68,
    NUMPAD9 = 0x69,
    MULTIPLY = 0x6A,
    ADD = 0x6B,
    SEPARATOR = 0x6C,
    SUBTRACT = 0x6D,
    DECIMAL = 0x6E,
    DIVIDE = 0x6F,

    F1 = 0x70,
    F2 = 0x71,
    F3 = 0x72,
    F4 = 0x73,
    F5 = 0x74,
    F6 = 0x75,
    F7 = 0x76,
    F8 = 0x77,
    F9 = 0x78,
    F10 = 0x79,
    F11 = 0x7A,
    F12 = 0x7B,
    F13 = 0x7C,
    F14 = 0x7D,
    F15 = 0x7E,
    F16 = 0x7F,
    F17 = 0x80,
    F18 = 0x81,
    F19 = 0x82,
    F20 = 0x83,
    F21 = 0x84,
    F22 = 0x85,
    F23 = 0x86,
    F24 = 0x87,

    NUMLOCK = 0x90,

    SCROLL = 0x91,

    NUMPAD_EQUAL = 0x92,

    LSHIFT = 0xA0,
    RSHIFT = 0xA1,
    LCONTROL = 0xA2,
    RCONTROL = 0xA3,
    LALT = 0xA4,
    RALT = 0xA5,

    SEMICOLON = 0x3B,

    APOSTROPHE = 0xDE,
    QUOTE = APOSTROPHE,
    EQUAL = 0xBB,
    COMMA = 0xBC,
    MINUS = 0xBD,
    PERIOD = 0xBE,
    SLASH = 0xBF,

    GRAVE = 0xC0,

    LBRACKET = 0xDB,
    PIPE = 0xDC,
    BACKSLASH = PIPE,
    RBRACKET = 0xDD,

    MAX_KEYS = 0xFF
};

struct KeyboardState
{
    b8 Keys[256];
};
struct MouseState
{
    i16 X;
    i16 Y;
    b8 Buttons[(u8)MOUSE_BUTTONS::BUTTON_MAX];
};
struct InputState
{
    KeyboardState KeyboardCurrentState;
    KeyboardState KeyboardPrevState;
    MouseState MouseCurrentState;
    MouseState MousePrevState;
};
class Input
{
private:
    InputState m_InputState;
    b8 m_Initialized = false;

public:
    b8 Initialize();
    void Shutdown();
    void Update();
    b8 IsKeyDown(KEYS key);
    b8 IsKeyUp(KEYS key);
    b8 WasKeyDown(KEYS key);
    b8 WasKeyUp(KEYS key);

    void ProcessKey(KEYS key, b8 pressed);
    
    b8 IsMouseButtonDown(MOUSE_BUTTONS button);
    b8 IsMouseButtonUp(MOUSE_BUTTONS button);
    b8 WasMouseButtonDown(MOUSE_BUTTONS button);
    b8 WasMouseButtonUp(MOUSE_BUTTONS button);

    void GetMousePosition(i32* x, i32* y);
    void GetPrevMousePosition(i32* x, i32* y);
    void ProcessMouseButton(MOUSE_BUTTONS button, b8 pressed);
};
}
