#include "VertexArray.h"
#include "Backend/OpenGL/OpenGLVertexArray.h"
#include "RendererAPI.h"


namespace BitEngine
{

VertexArray* VertexArray::Create()
{
    switch (RendererAPI::GetAPI()) 
    {

    case RENDERER_API::NONE:
        return nullptr;
    case RENDERER_API::OPENGL:
            return new OpenGLVertexArray();
      break;
    }
    return nullptr;
}
}
