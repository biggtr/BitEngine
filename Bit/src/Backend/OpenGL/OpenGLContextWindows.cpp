#ifdef BPLATFORM_WINDOWS
#include "OpenGLContextWindows.h"
#include "Bit/Core/Logger.h"

#include <glad/glad.h>
#include <windows.h>
#include <wingdi.h>

// WGL Extensions for modern context creation
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

namespace BitEngine {

    OpenGLContextWindows::OpenGLContextWindows(HWND windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }
b8 OpenGLContextWindows::Initialize()
{
    m_DeviceContext = GetDC(m_WindowHandle);
    if (!m_DeviceContext)
    {
        BIT_LOG_FATAL("Failed to get device context from window handle!");
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(m_DeviceContext, &pfd);
    if (pixelFormat == 0)
    {
        BIT_LOG_FATAL("Failed to choose pixel format!");
        return false;
    }

    if (!SetPixelFormat(m_DeviceContext, pixelFormat, &pfd))
    {
        BIT_LOG_FATAL("Failed to set pixel format!");
        return false;
    }
    
    HGLRC tempContext = wglCreateContext(m_DeviceContext);
    if (tempContext == NULL)
    {
        BIT_LOG_FATAL("Failed to create temporary WGL context!");
        return false;
    }
    
    wglMakeCurrent(m_DeviceContext, tempContext);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (wglCreateContextAttribsARB)
    {
        const int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0 // Null terminator
        };
        
        m_RenderingContext = wglCreateContextAttribsARB(m_DeviceContext, 0, attribs);
        
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);

        if (m_RenderingContext == NULL)
        {
            BIT_LOG_FATAL("Failed to create modern WGL context. Your GPU might not support OpenGL 3.3+.");
            return false;
        }

        wglMakeCurrent(m_DeviceContext, m_RenderingContext);
    }
    else
    {
        m_RenderingContext = tempContext;
    }


    if (!gladLoadGL())
    {
        BIT_LOG_FATAL("Failed to initialize GLAD!");
        return false;
    }

    BIT_LOG_INFO("OpenGL Context Initialized (WGL)");
    BIT_LOG_INFO("  Vendor: %s", glGetString(GL_VENDOR));
    BIT_LOG_INFO("  Renderer: %s", glGetString(GL_RENDERER));
    BIT_LOG_INFO("  Version: %s", glGetString(GL_VERSION));
    return true;
}
void OpenGLContextWindows::SwapBuffers()
{
    ::SwapBuffers(m_DeviceContext);
}
}
#endif
