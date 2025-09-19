#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RendererAPI.h"

namespace BitEngine
{

Shader* Shader::Create(const std::string& name, const ShaderSource& sources)
{

    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLShader(name, sources);
      break;
    }
    return nullptr;
}

}
