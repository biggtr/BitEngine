#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"

namespace BitEngine
{

Shader* Shader::Create(const char* path)
{

    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLShader(path);
      break;
    }
    return nullptr;
}
}
