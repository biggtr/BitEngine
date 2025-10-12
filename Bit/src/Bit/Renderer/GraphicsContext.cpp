#include "GraphicsContext.h"
#include "Backend/OpenGL/OpenGLContextLinux.h"
#include "Backend/OpenGL/OpenGLContextWindows.h"
#include "RendererAPI.h"
namespace BitEngine
{

GraphicsContext* GraphicsContext::Create()
{
    switch (RendererAPI::GetAPI()) 
    {
        case RENDERER_API::NONE:
            return nullptr;
        case RENDERER_API::OPENGL:
            {

            #ifdef BPLATFORM_LINUX
                return new OpenGLContextLinux();
            #elif BPLATFORM_WINDOWS
                return new OpenGLContextWindows();
            #endif
            }
    }

    return nullptr;
}

}
