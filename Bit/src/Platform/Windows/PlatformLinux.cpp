#include "PlatformLinux.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <cstring>
#include <unordered_set>


namespace BitEngine
{
static KEYS TranslateKeysymToPlatformKey(KeySym key);
    
PlatformLinux::PlatformLinux(u32 width, u32 height, const char* name)
    : m_Width(width), m_Height(height), m_Name(name)
{
    m_PlatformWindow = new PlatformWindow();
    memset(m_PlatformWindow, 0, sizeof(PlatformWindow));
}
b8 PlatformLinux::Initialize()
{

    Display* display = XOpenDisplay(NULL);
    m_PlatformWindow->Platform = PLATFORM_LINUX;
    m_PlatformWindow->Width = m_Width;
    m_PlatformWindow->Height = m_Height;
    m_PlatformWindow->Linux.Display = display;
    if(!m_PlatformWindow->Linux.Display)
    {
        BIT_LOG_ERROR("Couldn't Create a Display on linux..!");
        return false;
    }
    i32 screen = XDefaultScreen(display);
    m_PlatformWindow->Linux.Screen = screen;
    GLint attributes[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        None
    };

    i32 frameBuffersCount;
    GLXFBConfig* fb= glXChooseFBConfig(display, screen, attributes, &frameBuffersCount);
    GLXFBConfig bestFB = fb[0];
    XFree(fb);
    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display, bestFB);
    m_PlatformWindow->Linux.VisualInfo = visualInfo;

    m_WindowRoot = RootWindow(display, screen);
    u32 colormap = XCreateColormap(display, m_WindowRoot, visualInfo->visual, AllocNone); 
    m_PlatformWindow->Linux.ColorMap = colormap;

    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(XSetWindowAttributes));
    attr.colormap = m_PlatformWindow->Linux.ColorMap;
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | 
                      ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                      StructureNotifyMask | FocusChangeMask;
    attr.background_pixel = BlackPixel(display, screen);

    u64 valueMask = CWEventMask | CWBackPixel | CWColormap;

    Window window = XCreateWindow(display, m_WindowRoot, 0, 0,
                m_Width, m_Height, 0,
                visualInfo->depth, InputOutput, visualInfo->visual, valueMask, &attr);
    m_PlatformWindow->Linux.Window = window;

    XStoreName(display, window, m_Name);
    XMapWindow(display, window);
    m_Context = GraphicsContext::Create(m_PlatformWindow);
    m_Context->Initialize();
    return true;
}
void PlatformLinux::Shutdown()
{
    XUnmapWindow((Display*)m_PlatformWindow->Linux.Display, m_PlatformWindow->Linux.Window);
    XDestroyWindow((Display*)m_PlatformWindow->Linux.Display, m_PlatformWindow->Linux.Window);
    XCloseDisplay((Display *)m_PlatformWindow->Linux.Display);
    
}
void PlatformLinux::ProcessEvents()
{
    Display* display = (Display*)m_PlatformWindow->Linux.Display;
    Window window = m_PlatformWindow->Linux.Window;
    XEvent event;
    while(XPending(display))
    {
        XNextEvent(display, &event);
        switch (event.type) 
        {

            case ConfigureNotify:
                {
                    XConfigureEvent* configureEvent = (XConfigureEvent*)&event;
                    u16 width = configureEvent->width;
                    u16 height = configureEvent->height;
                    if(width != m_PlatformWindow->Width || height != m_PlatformWindow->Height)
                    {
                        m_PlatformWindow->Width = width;
                        m_PlatformWindow->Height = height;
                        BIT_LOG_DEBUG("Width: %d, Height: %d", width, height);
                        glViewport(0, 0, width, height);
                        EventContext context{};
                        context.U16[0] = width;
                        context.U16[1] = width;
                        EventManager::EventFire(EVENT_CODE_WINDOW_RESIZED, 0, context);
                    }
                    break;
                }
            case KeyPress:
            case KeyRelease:
            {
                KeySym keySym = XLookupKeysym(&event.xkey, 0);
                b8 pressed = event.type == KeyPress ? true : false;
                KEYS key = TranslateKeysymToPlatformKey(keySym);
                Input::ProcessKey(key, pressed);
                break;
            }
        }

    }

}
void PlatformLinux::OnUpdate() 
{
    m_Context->SwapBuffers();
    ProcessEvents();
}

u32 PlatformLinux::GetWidth() const 
{
    return m_Width;
}
u32 PlatformLinux::GetHeight() const
{
    return m_Height;
}

void* PlatformLinux::GetWindow() const
{
    return nullptr;
}
PlatformLinux::~PlatformLinux()
{
    delete m_PlatformWindow;
}
static KEYS TranslateKeysymToPlatformKey(KeySym key)
{
    switch(key) 
    {
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Return:
            return KEY_ENTER;
        case XK_Tab:
            return KEY_TAB;

        case XK_Pause:
            return KEY_PAUSE;
        case XK_Caps_Lock:
            return KEY_CAPITAL;

        case XK_Escape:
            return KEY_ESCAPE;

        case XK_Mode_switch:
            return KEY_MODECHANGE;

        case XK_space:
            return KEY_SPACE;
        case XK_Prior:
            return KEY_PAGEUP;
        case XK_Next:
            return KEY_PAGEDOWN;
        case XK_End:
            return KEY_END;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Execute:
            return KEY_EXECUTE;
        case XK_Insert:
            return KEY_INSERT;
        case XK_Delete:
            return KEY_DELETE;
        case XK_Help:
            return KEY_HELP;

        case XK_Meta_L:
        case XK_Super_L:
            return KEY_LSUPER;
        case XK_Meta_R:
        case XK_Super_R:
            return KEY_RSUPER;

        case XK_KP_0:
            return KEY_NUMPAD0;
        case XK_KP_1:
            return KEY_NUMPAD1;
        case XK_KP_2:
            return KEY_NUMPAD2;
        case XK_KP_3:
            return KEY_NUMPAD3;
        case XK_KP_4:
            return KEY_NUMPAD4;
        case XK_KP_5:
            return KEY_NUMPAD5;
        case XK_KP_6:
            return KEY_NUMPAD6;
        case XK_KP_7:
            return KEY_NUMPAD7;
        case XK_KP_8:
            return KEY_NUMPAD8;
        case XK_KP_9:
            return KEY_NUMPAD9;
        case XK_multiply:
            return KEY_MULTIPLY;
        case XK_KP_Add:
            return KEY_ADD;
        case XK_KP_Separator:
            return KEY_SEPARATOR;
        case XK_KP_Subtract:
            return KEY_SUBTRACT;
        case XK_KP_Decimal:
            return KEY_DECIMAL;
        case XK_KP_Divide:
            return KEY_DIVIDE;
        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
        case XK_F13:
            return KEY_F13;
        case XK_F14:
            return KEY_F14;
        case XK_F15:
            return KEY_F15;
        case XK_F16:
            return KEY_F16;
        case XK_F17:
            return KEY_F17;
        case XK_F18:
            return KEY_F18;
        case XK_F19:
            return KEY_F19;
        case XK_F20:
            return KEY_F20;
        case XK_F21:
            return KEY_F21;
        case XK_F22:
            return KEY_F22;
        case XK_F23:
            return KEY_F23;
        case XK_F24:
            return KEY_F24;

        case XK_Num_Lock:
            return KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return KEY_SCROLL;

        case XK_KP_Equal:
            return KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return KEY_LSHIFT;
        case XK_Shift_R:
            return KEY_RSHIFT;
        case XK_Control_L:
            return KEY_LCONTROL;
        case XK_Control_R:
            return KEY_RCONTROL;
        case XK_Alt_L:
            return KEY_LALT;
        case XK_Alt_R:
            return KEY_RALT;

        case XK_semicolon:
            return KEY_SEMICOLON;
        case XK_plus:
            return KEY_EQUAL;
        case XK_comma:
            return KEY_COMMA;
        case XK_minus:
            return KEY_MINUS;
        case XK_period:
            return KEY_PERIOD;
        case XK_slash:
            return KEY_SLASH;
        case XK_grave:
            return KEY_GRAVE;

        case XK_0:
            return KEY_0;
        case XK_1:
            return KEY_1;
        case XK_2:
            return KEY_2;
        case XK_3:
            return KEY_3;
        case XK_4:
            return KEY_4;
        case XK_5:
            return KEY_5;
        case XK_6:
            return KEY_6;
        case XK_7:
            return KEY_7;
        case XK_8:
            return KEY_8;
        case XK_9:
            return KEY_9;

        case XK_a:
        case XK_A:
            return KEY_A;
        case XK_b:
        case XK_B:
            return KEY_B;
        case XK_c:
        case XK_C:
            return KEY_C;
        case XK_d:
        case XK_D:
            return KEY_D;
        case XK_e:
        case XK_E:
            return KEY_E;
        case XK_f:
        case XK_F:
            return KEY_F;
        case XK_g:
        case XK_G:
            return KEY_G;
        case XK_h:
        case XK_H:
            return KEY_H;
        case XK_i:
        case XK_I:
            return KEY_I;
        case XK_j:
        case XK_J:
            return KEY_J;
        case XK_k:
        case XK_K:
            return KEY_K;
        case XK_l:
        case XK_L:
            return KEY_L;
        case XK_m:
        case XK_M:
            return KEY_M;
        case XK_n:
        case XK_N:
            return KEY_N;
        case XK_o:
        case XK_O:
            return KEY_O;
        case XK_p:
        case XK_P:
            return KEY_P;
        case XK_q:
        case XK_Q:
            return KEY_Q;
        case XK_r:
        case XK_R:
            return KEY_R;
        case XK_s:
        case XK_S:
            return KEY_S;
        case XK_t:
        case XK_T:
            return KEY_T;
        case XK_u:
        case XK_U:
            return KEY_U;
        case XK_v:
        case XK_V:
            return KEY_V;
        case XK_w:
        case XK_W:
            return KEY_W;
        case XK_x:
        case XK_X:
            return KEY_X;
        case XK_y:
        case XK_Y:
            return KEY_Y;
        case XK_z:
        case XK_Z:
            return KEY_Z;   

        default:
            return MAX_KEYS;
    }


}
}
