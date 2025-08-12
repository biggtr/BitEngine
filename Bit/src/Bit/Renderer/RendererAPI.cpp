#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace BitEngine
{

// Default RendererAPI 
RENDERER_API RendererAPI::s_API = RENDERER_API::OPENGL;

RendererAPI* RendererAPI::Create()
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLRendererAPI();
          break;
    }
    return nullptr;
}
}
