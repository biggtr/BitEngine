#include "PlatformLinux.h"
#include "Bit/Core/Logger.h"
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>


namespace BitEngine
{
    
    
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
    XSetCloseDownMode((Display*)m_PlatformWindow->Linux.Display, DestroyAll);
    
}
void PlatformLinux::ProcessEvents()
{
    XEvent event;
    while(XPending((Display*)m_PlatformWindow->Linux.Display))
    {
        XNextEvent((Display*)m_PlatformWindow->Linux.Display, &event);
        switch (event.type) 
        {
            case KeyPress:
            {
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
}
