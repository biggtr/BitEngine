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
};
class OpenGLContextLinux : public GraphicsContext
{
private:
    Display* m_Display;
    Window m_Window;
    i32 m_Screen;

    GLXFBConfig m_FBConfig;
    GLXContext m_GLContext;
public:
    OpenGLContextLinux();
    ~OpenGLContextLinux();    
    static LinuxWindowRequirements GetWindowRequirements(void* display, int screen);
    static void FreeWindowRequirements(Display* display, const LinuxWindowRequirements& req);
    virtual b8 Initialize(void* display, void* window, i32 screen) override;
    virtual void SwapBuffers() override;

private:
    static GLXFBConfig SelectBestFBConfig(Display* display, i32 screen);
};
}
#endif // BPLATFORM_LINUX
