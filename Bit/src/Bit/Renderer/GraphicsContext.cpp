#include "GraphicsContext.h"
#include "Bit/Core/Platform.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "RendererAPI.h"
namespace BitEngine
{

GraphicsContext* GraphicsContext::Create(void* window)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLContext((PlatformWindow*)window);
      break;
    }
    return nullptr;
}

}
