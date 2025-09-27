#include "GraphicsContext.h"
#include "Bit/Core/Platform.h"
#include "Platform/OpenGL/OpenGLContextLinux.h"
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
            return new OpenGLContextLinux();
      break;
    }
    return nullptr;
}

}
