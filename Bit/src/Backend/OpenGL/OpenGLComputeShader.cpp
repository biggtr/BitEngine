#include "OpenGLComputeShader.h"
#include <glad/glad.h>
#include <string>

namespace BitEngine
{
OpenGLComputeShader::OpenGLComputeShader(const std::string& path)
{
    u32 computeShader = glCreateShader(GL_COMPUTE_SHADER);
    const char* computeShaderSource = ParseComputeShader(path).c_str();
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    u32 id = glCreateProgram();
    glAttachShader(id, computeShader);
    glLinkProgram(id);
}
OpenGLComputeShader::~OpenGLComputeShader()
{

}
void OpenGLComputeShader::Dispatch() 
{
    
}

const std::string& OpenGLComputeShader::GetName() 
{

}
void OpenGLComputeShader::SetName(const std::string& name) 
{

}

u32 OpenGLComputeShader::GetID()  { return m_ID; }
}

