#include <glad/glad.h>
#include "Texture.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Backend/OpenGL/OpenGLTexture.h"

namespace BitEngine
{

Texture* Texture::Create(uint32_t width, uint32_t height, u32 dataFormat )
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLTexture(width, height, dataFormat);
      break;
    }
    return nullptr;
}
Texture* Texture::Create(const char* name, const char* path, u32 dataFormat )
{

    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLTexture(name, path, dataFormat);
      break;
    }
    return nullptr;
}

}
