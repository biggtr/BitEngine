#include "GraphicsContext.h"
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
            return new OpenGLContext((GLFWwindow*)window);
      break;
    }
    return nullptr;
}

}
