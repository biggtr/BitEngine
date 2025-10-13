#include "Platform.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <stdlib.h>

#ifdef BPLATFORM_LINUX
#include <X11/X.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace BitEngine
{



static KEYS TranslateKeysymToPlatformKey(KeySym key);

struct InternalState
{
    Display* Dpy;
    i32 Screen;
    u32 Window;
};

b8 PlatformStartup(PlatformState* platformState, const char* applicationName, i32 x, i32 y, u32 width, u32 height)
{
    platformState->InternalState = malloc(sizeof(InternalState));
    InternalState* internalState = (InternalState*)platformState->InternalState;
    internalState->Dpy = XOpenDisplay(NULL);
    if(!internalState->Dpy)
    {
        CERROR("Couldn't Create a Display on linux..!");
        return false;
    }
    internalState->Screen = XDefaultScreen(internalState->Dpy);



    XVisualInfo visualInfo;
    if(!XMatchVisualInfo(internalState->Dpy, internalState->Screen, 24, TrueColor, &visualInfo))
    {
        CERROR("Couldn't found appropriate 24 TrueColor visual info");
        return false;
    }
    u32 rootWindow = RootWindow(internalState->Dpy, internalState->Screen);
    Colormap colormap = XCreateColormap(internalState->Dpy, rootWindow, visualInfo.visual, AllocNone);
    


    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(XSetWindowAttributes));
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | 
                      ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                      StructureNotifyMask | FocusChangeMask;
    attr.colormap = colormap;
    attr.background_pixel = BlackPixel(internalState->Dpy, internalState->Screen);

    u64 valueMask = CWEventMask | CWBackPixel | CWColormap;

    platformState->Width = width;
    platformState->Height = height;
    internalState->Window = XCreateWindow(internalState->Dpy, 
            rootWindow, x, y, width, height, 0,
            visualInfo.depth, InputOutput, visualInfo.visual, valueMask, &attr);

    if(!internalState->Window)
    {
        CERROR("Failed To Create A Window");
        XCloseDisplay(internalState->Dpy);
        return false;
    }
    // register event for handling the xlib window closing 
    Atom wmDelete = XInternAtom(internalState->Dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(internalState->Dpy, internalState->Window, &wmDelete, 1);
    
    XAutoRepeatOff(internalState->Dpy);

    XStoreName(internalState->Dpy, internalState->Window, applicationName);
    XMapWindow(internalState->Dpy, internalState->Window);
    XFlush(internalState->Dpy);
    return true;

}

void PlatformShutdown(PlatformState* platformState)
{
    CDEBUG("Platform Shutting Down.")
    InternalState* internalState = (InternalState*)platformState->InternalState;
    XAutoRepeatOn(internalState->Dpy);
    XUnmapWindow(internalState->Dpy, internalState->Window);
    XDestroyWindow(internalState->Dpy, internalState->Window);
    XCloseDisplay(internalState->Dpy);
    free(internalState);
}
b8 PlatformPumpMessages(PlatformState* platformState)
{
    InternalState* internalState = (InternalState*)platformState->InternalState;
    XEvent event;
    while(XPending(internalState->Dpy))
    {
        XNextEvent(internalState->Dpy, &event);
        switch (event.type) 
        {
            case ConfigureNotify:
                {
                    XConfigureEvent* configureEvent = (XConfigureEvent*)&event;
                    u16 width = configureEvent->width;
                    u16 height = configureEvent->height;
                    if(width != platformState->Width || height != platformState->Height)
                    {
                        platformState->Width = width;
                        platformState->Height = height;
                        CDEBUG("Width: %d, Height: %d", width, height);
                        EventContext context{};
                        context.U16[0] = width;
                        context.U16[1] = height;
                        EventFire(EVENT_CODE_WINDOW_RESIZED, 0, context);
                    }
                    break;
                }
            case KeyPress:
            case KeyRelease:
            {
                KeySym keySym = XLookupKeysym(&event.xkey, 0);
                b8 pressed = event.type == KeyPress ? true : false;
                KEYS key = TranslateKeysymToPlatformKey(keySym);
                InputProcessKey(key, pressed);
                break;
            }
            case ButtonPress:
            case ButtonRelease:
            {
                MOUSE_BUTTONS button;
                switch (event.xbutton.button) 
                {
                    case Button1:
                        button = MOUSE_BUTTON_LEFT;
                        break;
                    case Button2:
                        button = MOUSE_BUTTON_MIDDLE;
                        break;
                    case Button3:
                        button = MOUSE_BUTTON_RIGHT;
                        break;
                    case Button4:
                        button = MOUSE_BUTTON_MIDDLE;
                        InputProcessMouseWheel(1);
                        break;
                    case Button5:
                        button = MOUSE_BUTTON_MIDDLE;
                        InputProcessMouseWheel(-1);
                        break;
                }
                b8 pressed = event.type == ButtonPress ? true : false;
                InputProcessMouseButton(button,  pressed);
                break;
            }
            case MotionNotify:
            {
                i16 x, y;
                x = event.xmotion.x;
                y = event.xmotion.y;
                InputProcessMouseMove(&x, &y);
                break;
            }
            case ClientMessage:
            {
                Atom wmDelete = XInternAtom(internalState->Dpy, "WM_DELETE_WINDOW", False);
                if ((Atom)event.xclient.data.l[0] == wmDelete) {
                    EventFire(EVENT_CODE_APPLICATION_QUIT, 0, {});
                }
                break;
            }

        }

    }
    return true;
}

void* PlatformAllocate(u64 size, b8 aligned)
{
    (void)aligned;
    void* block = malloc(size);
    return block;
}
void PlatformFree(void* block, b8 aligned)
{
    (void)aligned;
    free(block);
}
void PlatformZeroMemory(void* block, u64 size)
{
    memset(block, 0, size);
}
void PlatformSetMemory(void* dest, i32 value, u64 size)
{
    memset(dest, value, size);
}
void PlatformCopyMemory(void *dest, const void *source, u64 size)
{
    memcpy(dest, source, size);
}

f64 PlatformGetAbsoluteTime()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    return ts.tv_sec + ts.tv_nsec * 0.000000001;
}
void PlatformSleep(u64 ms)
{
    if(ms >= 1000)
    {
        sleep(ms);
    }
    else
    {
        usleep((ms % 1000) * 1000);
    }
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
#endif
