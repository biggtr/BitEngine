#include "ComputeShader.h"
#include "Bit/Core/Core.h"
#include "Backend/OpenGL/OpenGLComputeShader.h"
#include "RendererAPI.h"

namespace BitEngine
{


ComputeShader* ComputeShader::Create(const char* path)
{

    switch (RendererAPI::GetAPI()) 
    {
        case RENDERER_API::NONE:
            return nullptr;
        case RENDERER_API::OPENGL:
            return new OpenGLComputeShader(path);
          break;
    }
    return nullptr;
}
}
