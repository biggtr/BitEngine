#include "Texture.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace BitEngine
{

Texture* Texture::Create(const std::string &path)
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
