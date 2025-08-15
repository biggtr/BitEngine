#include <cstring>
#include <glad/glad.h>
#include "OpenGLContext.h"
#include <GL/glx.h>
#include <GL/gl.h>
#include "Bit/Core/Logger.h"
#include <X11/Xutil.h>
#include <X11/Xlib.h>

namespace BitEngine
{

static bool bGLInitErrorRaised = false;
static int glInitErrorHandler(Display* display, XErrorEvent* ev) {
    bGLInitErrorRaised = true;
    return 0;
}
static bool glCheckExtensionSupported(const char* lpExtensionString, const char* lpExtensionName)
{
    unsigned long int dwCurrentStart = 0;
    unsigned long int dwCurrentEnd = 0;
    unsigned long int extStrLen;

    if((lpExtensionName == NULL) || (lpExtensionString == NULL)) 
    {
        return false;
    }

    extStrLen = strlen(lpExtensionString);

    if((lpExtensionString[0] == 0x00) || (extStrLen == 0)) 
    {
        return false;
    }

    for(; dwCurrentEnd <= extStrLen; dwCurrentEnd = dwCurrentEnd + 1) 
    {
        if((lpExtensionString[dwCurrentEnd] != ' ') && (lpExtensionString[dwCurrentEnd] != 0x00)) 
        {
            continue;
        }
        if(strncmp(lpExtensionName, &(lpExtensionString[dwCurrentStart]), dwCurrentEnd - dwCurrentStart) == 0) 
        {
            return true;
        }
        if(lpExtensionString[dwCurrentEnd] == 0x00) 
        {
            return false;
        }
        dwCurrentStart = dwCurrentEnd + 1;
    }

    return false;
}
    
OpenGLContext::OpenGLContext(PlatformWindow* window)
    :m_Window(window)
{
}

b8 OpenGLContext::Initialize()
{
    Display* display = (Display*)m_Window->Linux.Display;
    u32 window = m_Window->Linux.Window;
    u32 screen = m_Window->Linux.Screen;
    GLXFBConfig cfgChoosen = m_Window->Linux.FBConfig;
    XVisualInfo* vi = (XVisualInfo*)m_Window->Linux.VisualInfo;
    int glxMajor, glxMinor;
    if (!glXQueryVersion(display, &glxMajor, &glxMinor)) 
    {
        BIT_LOG_ERROR("GLX not supported\n");
        XCloseDisplay(display);
        return false;
    }
    BIT_LOG_INFO("GLX version: %d.%d\n", glxMajor, glxMinor);

    const char *glxExts = glXQueryExtensionsString(display, screen);
    
    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig,
                                                        GLXContext, Bool, const int*);

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    if(!glCheckExtensionSupported(glxExts, "GLX_ARB_create_context"))
    {
        BIT_LOG_ERROR("GLX_ARB_create_context is not supported\n");
        XFree(vi);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
        return false;
    }

    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
    
    if(!glXCreateContextAttribsARB)
    {
        BIT_LOG_ERROR("Failed to query function pointer to glXCreateContextAttribsARB");
        XFree(vi);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
        return false;
    }

    GLXContext context = 0;
    
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&glInitErrorHandler);
        
    int context_attribs[][7] = 
    {
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 6,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        },
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        },
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        }
    };
    
    const char* version_names[] = {"4.6", "4.0", "3.3"};
    for (int i = 0; i < 3; i++) 
    {
        bGLInitErrorRaised = false;
        
        context = glXCreateContextAttribsARB(display, cfgChoosen, 0, True, context_attribs[i]);
        XSync(display, False);
        
        if (!bGLInitErrorRaised && context)
        {
            BIT_LOG_INFO("Successfully created OpenGL %s core context", version_names[i]);
            break;
        }
        
        if (context) 
        {
            glXDestroyContext(display, context);
            context = 0;
        }
    }
    
    XSetErrorHandler(oldHandler);
    
    if (!context) 
    {
        BIT_LOG_ERROR("Could not create OpenGL context\n");
        XDestroyWindow(display, window);
        XFree(vi);
        XCloseDisplay(display);
        return false;
    }
    
    if (!glXIsDirect(display, context)) 
    {
        BIT_LOG_ERROR("Using indirect GLX rendering context");
    } 
    else 
    {
        BIT_LOG_INFO("Using direct GLX rendering context\n");
    }
    
    if (!glXMakeCurrent(display, window, context)) 
    {
        BIT_LOG_ERROR("Could not make GLX context current");
        glXDestroyContext(display, context);
        XDestroyWindow(display, window);
        XFree(vi);
        XCloseDisplay(display);
        return false;
    }
    if (!gladLoadGLLoader((GLADloadproc)glXGetProcAddress))
    {
        BIT_LOG_ERROR("Failed to initialize GLAD OpenGL loader");
        return false;
    }
    
    BIT_LOG_INFO("GLAD initialized successfully");
    BIT_LOG_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
    BIT_LOG_INFO("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    return true;
}

void OpenGLContext::SwapBuffers()
{
    glXSwapBuffers((Display*)m_Window->Linux.Display, m_Window->Linux.Window);
}

}
