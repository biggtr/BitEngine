#include <glad/glad.h>
#include "OpenGLContext.h"
#include <GL/glx.h>
#include "Bit/Core/Logger.h"
#include <X11/Xutil.h>
#include <X11/Xlib.h>

namespace BitEngine
{


    
OpenGLContext::OpenGLContext(PlatformWindow* window)
    :m_Window(window)
{
}

b8 OpenGLContext::Initialize()
{
    Display* display = (Display*)m_Window->Linux.Display;
    XVisualInfo* visual = (XVisualInfo*)m_Window->Linux.VisualInfo;
    u32 window = m_Window->Linux.Window;

    m_GLContext = glXCreateContext(display, visual, NULL, GL_TRUE);
    glXMakeCurrent(display, window, m_GLContext);


    
    if (!gladLoadGLLoader((GLADloadproc)glXGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }  
    BIT_LOG_INFO("OpenGL Version : %d", GL_VERSION);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void OpenGLContext::SwapBuffers()
{
    glXSwapBuffers((Display*)m_Window->Linux.Display, m_Window->Linux.Window);
}

void OpenGLContext::Shutdown()
{

}
}
