#pragma once
#ifdef BPLATFORM_LINUX

#include "Platform/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GL/glx.h>
#include <X11/X.h>
namespace BitEngine
{

struct LinuxWindowRequirements 
{
    XVisualInfo* visualInfo;
    Colormap colormap;
    unsigned long valueMask;
    int depth;
    GLXFBConfig fbconfig;
};
class OpenGLContextLinux : public GraphicsContext
{
private:
    Display* m_Display;
    Window m_Window;
    i32 m_Screen;

    LinuxWindowRequirements m_Req;
    GLXContext m_GLContext;
public:
    OpenGLContextLinux(Display* display, Window window, int screen, const LinuxWindowRequirements& req);
    ~OpenGLContextLinux();
    virtual b8 Initialize() override;
    virtual void SwapBuffers() override;

    static LinuxWindowRequirements GetWindowRequirements(Display* display, int screen);
    static void FreeWindowRequirements(Display* display, const LinuxWindowRequirements& req);
    
private:
    static GLXFBConfig SelectBestFBConfig(Display* display, i32 screen);
};
}
#endif // BPLATFORM_LINUX
