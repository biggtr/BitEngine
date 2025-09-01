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
static BInput::KEYS TranslateKeysymToPlatformKey(KeySym key);
    
PlatformLinux::PlatformLinux(u32 width, u32 height, const char* name)
    : m_Width(width), m_Height(height), m_Name(name)
{
    m_PlatformWindow = new PlatformWindow();
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
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,

        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,

        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,

        GLX_DOUBLEBUFFER, True,
        None
    };

    i32 frameBuffersCount;
    GLXFBConfig* fbConfig= glXChooseFBConfig(display, screen, attributes, &frameBuffersCount);

    GLXFBConfig cfgChoosen;
    i32 indexBestConfig = 0;
    i32 bestSamples = 0;
    for(i32 i = 0; i < frameBuffersCount; ++i)
    {
        XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbConfig[i]);
        if(vi)
        {
            i32 sampleBuffers = 0;
            i32 samples = 0;
            glXGetFBConfigAttrib(display, fbConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            glXGetFBConfigAttrib(display, fbConfig[i], GLX_SAMPLES, &samples);
            BIT_LOG_INFO("\t%02lx, Sample buffers: %d, Samples: %d\n", vi->visualid, sampleBuffers, samples);
            if((bestSamples < samples) && (sampleBuffers > 0))
            {
                bestSamples = samples;
                indexBestConfig = i;
            }
        }
        XFree(vi);
    }
    cfgChoosen = fbConfig[indexBestConfig];

    XVisualInfo* vi = glXGetVisualFromFBConfig(display, cfgChoosen);
    XFree(fbConfig);
    m_PlatformWindow->Linux.VisualInfo = vi;
    m_PlatformWindow->Linux.FBConfig = cfgChoosen;

    m_WindowRoot = RootWindow(display, screen);
    u32 colormap = XCreateColormap(display, m_WindowRoot, vi->visual, AllocNone); 
    m_PlatformWindow->Linux.ColorMap = colormap;

    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(XSetWindowAttributes));
    attr.colormap = colormap;
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | 
                      ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                      StructureNotifyMask | FocusChangeMask;
    attr.background_pixmap = None;
    attr.border_pixel = 0;

    u64 valueMask = CWEventMask | CWBackPixel | CWColormap;

    Window window = XCreateWindow(display, m_WindowRoot, 0, 0,
                m_Width, m_Height, 0,
                vi->depth, InputOutput, vi->visual, valueMask, &attr);
    if(!window)
    {
        BIT_LOG_ERROR("Failed To Create A Window");
        XCloseDisplay(display);
        XFree(vi);
        return false;
    }
    m_PlatformWindow->Linux.Window = window;

    XStoreName(display, window, m_Name);
    XMapWindow(display, window);
    XFlush(display);

    m_Context = GraphicsContext::Create(m_PlatformWindow);
    m_Context->Initialize();

    return true;
}
void PlatformLinux::Shutdown()
{
    delete m_Context;
    XFree(m_PlatformWindow->Linux.VisualInfo);
    XUnmapWindow((Display*)m_PlatformWindow->Linux.Display, m_PlatformWindow->Linux.Window);
    XDestroyWindow((Display*)m_PlatformWindow->Linux.Display, m_PlatformWindow->Linux.Window);
    XCloseDisplay((Display *)m_PlatformWindow->Linux.Display);
    
}
void PlatformLinux::ProcessEvents()
{
    Display* display = (Display*)m_PlatformWindow->Linux.Display;
    // Window window = m_PlatformWindow->Linux.Window;
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
                        context.U16[1] = height;
                        EventManager::EventFire(EVENT_CODE_WINDOW_RESIZED, 0, context);
                    }
                    break;
                }
            case KeyPress:
            case KeyRelease:
            {
                KeySym keySym = XLookupKeysym(&event.xkey, 0);
                b8 pressed = event.type == KeyPress ? true : false;
                BInput::KEYS key = TranslateKeysymToPlatformKey(keySym);
                BInput::ProcessKey(key, pressed);
                break;
            }
            case ButtonPress:
            case ButtonRelease:
            {
                BInput::MOUSE_BUTTONS button;
                switch (event.xbutton.button) 
                {
                    case Button1:
                        button = BInput::MOUSE_BUTTON_LEFT;
                        break;
                    case Button2:
                        button = BInput::MOUSE_BUTTON_MIDDLE;
                        break;
                    case Button3:
                        button = BInput::MOUSE_BUTTON_RIGHT;
                        break;
                    case Button4:
                        button = BInput::MOUSE_BUTTON_MIDDLE;
                        BInput::ProcessMouseWheel(1);
                        break;
                    case Button5:
                        button = BInput::MOUSE_BUTTON_MIDDLE;
                        BInput::ProcessMouseWheel(-1);
                        break;
                }
                b8 pressed = event.type == ButtonPress ? true : false;
                BInput::ProcessMouseButton(button,  pressed);
                break;
            }
            case MotionNotify:
            {
                i16 x, y;
                x = event.xmotion.x;
                y = event.xmotion.y;
                BInput::ProcessMouseMove(&x, &y);
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
static BInput::KEYS TranslateKeysymToPlatformKey(KeySym key)
{
    switch(key) 
    {
        case XK_BackSpace:
            return BInput::KEY_BACKSPACE;
        case XK_Return:
            return BInput::KEY_ENTER;
        case XK_Tab:
            return BInput::KEY_TAB;

        case XK_Pause:
            return BInput::KEY_PAUSE;
        case XK_Caps_Lock:
            return BInput::KEY_CAPITAL;

        case XK_Escape:
            return BInput::KEY_ESCAPE;

        case XK_Mode_switch:
            return BInput::KEY_MODECHANGE;

        case XK_space:
            return BInput::KEY_SPACE;
        case XK_Prior:
            return BInput::KEY_PAGEUP;
        case XK_Next:
            return BInput::KEY_PAGEDOWN;
        case XK_End:
            return BInput::KEY_END;
        case XK_Home:
            return BInput::KEY_HOME;
        case XK_Left:
            return BInput::KEY_LEFT;
        case XK_Up:
            return BInput::KEY_UP;
        case XK_Right:
            return BInput::KEY_RIGHT;
        case XK_Down:
            return BInput::KEY_DOWN;
        case XK_Select:
            return BInput::KEY_SELECT;
        case XK_Print:
            return BInput::KEY_PRINT;
        case XK_Execute:
            return BInput::KEY_EXECUTE;
        case XK_Insert:
            return BInput::KEY_INSERT;
        case XK_Delete:
            return BInput::KEY_DELETE;
        case XK_Help:
            return BInput::KEY_HELP;

        case XK_Meta_L:
        case XK_Super_L:
            return BInput::KEY_LSUPER;
        case XK_Meta_R:
        case XK_Super_R:
            return BInput::KEY_RSUPER;

        case XK_KP_0:
            return BInput::KEY_NUMPAD0;
        case XK_KP_1:
            return BInput::KEY_NUMPAD1;
        case XK_KP_2:
            return BInput::KEY_NUMPAD2;
        case XK_KP_3:
            return BInput::KEY_NUMPAD3;
        case XK_KP_4:
            return BInput::KEY_NUMPAD4;
        case XK_KP_5:
            return BInput::KEY_NUMPAD5;
        case XK_KP_6:
            return BInput::KEY_NUMPAD6;
        case XK_KP_7:
            return BInput::KEY_NUMPAD7;
        case XK_KP_8:
            return BInput::KEY_NUMPAD8;
        case XK_KP_9:
            return BInput::KEY_NUMPAD9;
        case XK_multiply:
            return BInput::KEY_MULTIPLY;
        case XK_KP_Add:
            return BInput::KEY_ADD;
        case XK_KP_Separator:
            return BInput::KEY_SEPARATOR;
        case XK_KP_Subtract:
            return BInput::KEY_SUBTRACT;
        case XK_KP_Decimal:
            return BInput::KEY_DECIMAL;
        case XK_KP_Divide:
            return BInput::KEY_DIVIDE;
        case XK_F1:
            return BInput::KEY_F1;
        case XK_F2:
            return BInput::KEY_F2;
        case XK_F3:
            return BInput::KEY_F3;
        case XK_F4:
            return BInput::KEY_F4;
        case XK_F5:
            return BInput::KEY_F5;
        case XK_F6:
            return BInput::KEY_F6;
        case XK_F7:
            return BInput::KEY_F7;
        case XK_F8:
            return BInput::KEY_F8;
        case XK_F9:
            return BInput::KEY_F9;
        case XK_F10:
            return BInput::KEY_F10;
        case XK_F11:
            return BInput::KEY_F11;
        case XK_F12:
            return BInput::KEY_F12;
        case XK_F13:
            return BInput::KEY_F13;
        case XK_F14:
            return BInput::KEY_F14;
        case XK_F15:
            return BInput::KEY_F15;
        case XK_F16:
            return BInput::KEY_F16;
        case XK_F17:
            return BInput::KEY_F17;
        case XK_F18:
            return BInput::KEY_F18;
        case XK_F19:
            return BInput::KEY_F19;
        case XK_F20:
            return BInput::KEY_F20;
        case XK_F21:
            return BInput::KEY_F21;
        case XK_F22:
            return BInput::KEY_F22;
        case XK_F23:
            return BInput::KEY_F23;
        case XK_F24:
            return BInput::KEY_F24;

        case XK_Num_Lock:
            return BInput::KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return BInput::KEY_SCROLL;

        case XK_KP_Equal:
            return BInput::KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return BInput::KEY_LSHIFT;
        case XK_Shift_R:
            return BInput::KEY_RSHIFT;
        case XK_Control_L:
            return BInput::KEY_LCONTROL;
        case XK_Control_R:
            return BInput::KEY_RCONTROL;
        case XK_Alt_L:
            return BInput::KEY_LALT;
        case XK_Alt_R:
            return BInput::KEY_RALT;

        case XK_semicolon:
            return BInput::KEY_SEMICOLON;
        case XK_plus:
            return BInput::KEY_EQUAL;
        case XK_comma:
            return BInput::KEY_COMMA;
        case XK_minus:
            return BInput::KEY_MINUS;
        case XK_period:
            return BInput::KEY_PERIOD;
        case XK_slash:
            return BInput::KEY_SLASH;
        case XK_grave:
            return BInput::KEY_GRAVE;

        case XK_0:
            return BInput::KEY_0;
        case XK_1:
            return BInput::KEY_1;
        case XK_2:
            return BInput::KEY_2;
        case XK_3:
            return BInput::KEY_3;
        case XK_4:
            return BInput::KEY_4;
        case XK_5:
            return BInput::KEY_5;
        case XK_6:
            return BInput::KEY_6;
        case XK_7:
            return BInput::KEY_7;
        case XK_8:
            return BInput::KEY_8;
        case XK_9:
            return BInput::KEY_9;

        case XK_a:
        case XK_A:
            return BInput::KEY_A;
        case XK_b:
        case XK_B:
            return BInput::KEY_B;
        case XK_c:
        case XK_C:
            return BInput::KEY_C;
        case XK_d:
        case XK_D:
            return BInput::KEY_D;
        case XK_e:
        case XK_E:
            return BInput::KEY_E;
        case XK_f:
        case XK_F:
            return BInput::KEY_F;
        case XK_g:
        case XK_G:
            return BInput::KEY_G;
        case XK_h:
        case XK_H:
            return BInput::KEY_H;
        case XK_i:
        case XK_I:
            return BInput::KEY_I;
        case XK_j:
        case XK_J:
            return BInput::KEY_J;
        case XK_k:
        case XK_K:
            return BInput::KEY_K;
        case XK_l:
        case XK_L:
            return BInput::KEY_L;
        case XK_m:
        case XK_M:
            return BInput::KEY_M;
        case XK_n:
        case XK_N:
            return BInput::KEY_N;
        case XK_o:
        case XK_O:
            return BInput::KEY_O;
        case XK_p:
        case XK_P:
            return BInput::KEY_P;
        case XK_q:
        case XK_Q:
            return BInput::KEY_Q;
        case XK_r:
        case XK_R:
            return BInput::KEY_R;
        case XK_s:
        case XK_S:
            return BInput::KEY_S;
        case XK_t:
        case XK_T:
            return BInput::KEY_T;
        case XK_u:
        case XK_U:
            return BInput::KEY_U;
        case XK_v:
        case XK_V:
            return BInput::KEY_V;
        case XK_w:
        case XK_W:
            return BInput::KEY_W;
        case XK_x:
        case XK_X:
            return BInput::KEY_X;
        case XK_y:
        case XK_Y:
            return BInput::KEY_Y;
        case XK_z:
        case XK_Z:
            return BInput::KEY_Z;   

        default:
            return BInput::MAX_KEYS;
    }


}
}
