#include <cstring>
#include <glad/glad.h>
#include "OpenGLContextLinux.h"
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
    
OpenGLContextLinux::OpenGLContextLinux()
    : m_VisualInfo(nullptr), m_ColorMap(0), m_GLContext(nullptr)
{
}

b8 OpenGLContextLinux::Initialize(void* display, void* window, i32 screen)
{

    m_Display = (Display*)display;
    m_Window= (Window*)window;
    m_Screen = screen; 
    int glxMajor, glxMinor;
    if (!glXQueryVersion(m_Display, &glxMajor, &glxMinor)) 
    {
        BIT_LOG_ERROR("GLX not supported\n");
        XCloseDisplay(m_Display);
        return false;
    }
    BIT_LOG_INFO("GLX version: %d.%d\n", glxMajor, glxMinor);

    const char *glxExts = glXQueryExtensionsString(m_Display, m_Screen);


    typedef GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig,
            GLXContext, Bool, const int*);
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    if(!glCheckExtensionSupported(glxExts, "GLX_ARB_create_context"))
    {
        BIT_LOG_ERROR("GLX_ARB_create_context is not supported\n");
        return false;
    }

    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    if(!glXCreateContextAttribsARB)
    {
        BIT_LOG_ERROR("Failed to query function pointer to glXCreateContextLinuxAttribsARB");
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
        
        context = glXCreateContextAttribsARB(m_Display, m_FBConfig, 0, True, context_attribs[i]);
        XSync(m_Display, False);
        
        if (!bGLInitErrorRaised && context)
        {
            BIT_LOG_INFO("Successfully created OpenGL %s core context", version_names[i]);
            break;
        }
        
        if (context) 
        {
            glXDestroyContext(m_Display, context);
            context = 0;
        }
    }

    XSetErrorHandler(oldHandler);

    if (!context) 
    {
        BIT_LOG_ERROR("Could not create OpenGL context\n");
        return false;
    }

    if (!glXIsDirect(m_Display, context)) 
    {
        BIT_LOG_ERROR("Using indirect GLX rendering context");
    } 
    else 
    {
        BIT_LOG_INFO("Using direct GLX rendering context\n");
    }

    if (!glXMakeCurrent(m_Display, *m_Window, context)) 
    {
        BIT_LOG_ERROR("Could not make GLX context current");
        glXDestroyContext(m_Display, context);
        XDestroyWindow(m_Display, *m_Window);
        XCloseDisplay(m_Display);
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

GraphicsContext::WindowRequirements OpenGLContextLinux::GetWindowRequirements(void* display, i32 screen) 
{
    m_Display = (Display*)display;
    m_Screen = screen;

    m_FBConfig = SelectBestFBConfig(m_Display, screen);
    m_VisualInfo = glXGetVisualFromFBConfig(m_Display, m_FBConfig);

    Window root = RootWindow(m_Display, m_Screen);
    m_ColorMap = XCreateColormap(m_Display, root, m_VisualInfo->visual, AllocNone);
    
    WindowRequirements req;
    req.visualInfo = m_VisualInfo;
    req.colormap = m_ColorMap;
    req.valueMask = CWEventMask | CWBackPixel | CWColormap;
    req.depth = m_VisualInfo->depth;
    req.visual = m_VisualInfo->visual;

    return req;
}
GLXFBConfig OpenGLContextLinux::SelectBestFBConfig(Display* display, i32 screen)
{
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
    GLXFBConfig* fbConfigs = glXChooseFBConfig(m_Display, m_Screen, attributes, &frameBuffersCount);

    GLXFBConfig cfgChoosen;
    i32 indexBestConfig = 0;
    i32 bestSamples = 0;
    for(i32 i = 0; i < frameBuffersCount; ++i)
    {
        XVisualInfo* vi = glXGetVisualFromFBConfig(m_Display, fbConfigs[i]);
        if(vi)
        {
            i32 sampleBuffers = 0;
            i32 samples = 0;
            glXGetFBConfigAttrib(m_Display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            glXGetFBConfigAttrib(m_Display, fbConfigs[i], GLX_SAMPLES, &samples);
            BIT_LOG_INFO("\t%02lx, Sample buffers: %d, Samples: %d\n", vi->visualid, sampleBuffers, samples);
            if((bestSamples < samples) && (sampleBuffers > 0))
            {
                bestSamples = samples;
                indexBestConfig = i;
            }
            XFree(vi);
        }
    }
    cfgChoosen = fbConfigs[indexBestConfig];
    XFree(fbConfigs);

    return cfgChoosen;
}
void OpenGLContextLinux::SwapBuffers()
{
    glXSwapBuffers(m_Display, *m_Window);
}

OpenGLContextLinux::~OpenGLContextLinux()    
{
    if (m_GLContext)
    {
        glXMakeCurrent(m_Display, None, nullptr);
        glXDestroyContext(m_Display, m_GLContext);
    }
    
    if (m_ColorMap)
    {
        XFreeColormap(m_Display, m_ColorMap);
    }
    
    if (m_VisualInfo)
    {
        XFree(m_VisualInfo);
    }
}
}
