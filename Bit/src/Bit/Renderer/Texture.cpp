#include "Texture.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Backend/OpenGL/OpenGLTexture.h"

namespace BitEngine
{

Texture* Texture::Create(uint32_t width, uint32_t height)
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLTexture(width, height);
      break;
    }
    return nullptr;
}
Texture* Texture::Create(const char* path)
{

    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLTexture(path);
      break;
    }
    return nullptr;
}

}
