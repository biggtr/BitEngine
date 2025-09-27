#pragma once
#include "Bit/Core/Platform.h"
#include "Bit/Renderer/GraphicsContext.h"
#include <GL/glx.h>
#include <X11/X.h>
struct GLFWwindow;
namespace BitEngine
{

class OpenGLContextLinux : public GraphicsContext
{
private:
    Display* m_Display;
    Window* m_Window;
    i32 m_Screen;

    XVisualInfo* m_VisualInfo;
    Colormap m_ColorMap;

    GLXFBConfig m_FBConfig;
    GLXContext m_GLContext;
public:
    OpenGLContextLinux();
    ~OpenGLContextLinux();    
    virtual WindowRequirements GetWindowRequirements(void* display, int screen) override;
    virtual b8 Initialize(void* display, void* window, i32 screen) override;
    virtual void SwapBuffers() override;

private:
    GLXFBConfig SelectBestFBConfig(Display* display, i32 screen);
};
}
