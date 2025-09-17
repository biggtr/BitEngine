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

ShaderLibrary::ShaderLibrary()
{
    m_Shaders = {};
}
ShaderLibrary::~ShaderLibrary()
{
    for(auto& shader : m_Shaders)
    {
        delete shader.second;
    }
    m_Shaders.clear();
}

void ShaderLibrary::Add(const char* name, Shader* shader)
{
    shader->SetName(name);
    u32 shaderID = shader->GetID();
    
    m_Shaders[shaderID] = shader;
}
void ShaderLibrary::Add(Shader* shader)
{
    u32 shaderID = shader->GetID();
    m_Shaders[shaderID] = shader;
}

Shader* ShaderLibrary::Load(const char* filePath)
{
    Shader* shader = Shader::Create(filePath);
    Add(shader);
    return shader;
}
Shader* ShaderLibrary::Load(const char* name, const char* filePath)
{
    Shader* shader = Shader::Create(filePath);
    Add(name, shader);
    return shader;
}

Shader* ShaderLibrary::Get(u32 shaderID)
{
    return m_Shaders[shaderID];
}
}
