#include "OpenGLComputeShader.h"
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <glad/glad.h>
#include <numeric>
#include <string>

namespace BitEngine
{
OpenGLComputeShader::OpenGLComputeShader(const char* path)
{
    m_Path = path;
    CreateProgram();
}
u32 OpenGLComputeShader::CompileShader()
{
    u32 computeShader = glCreateShader(GL_COMPUTE_SHADER);
    char* computeShaderSource = ParseComputeShader(m_Path);
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
        BIT_LOG_ERROR("ERROR::SHADER::Compute::COMPILATION_FAILED\n %s", infoLog);
    }
    free(computeShaderSource);

    return computeShader;
}
void OpenGLComputeShader::CreateProgram()
{
    u32 computeShader = CompileShader();

    m_ID = glCreateProgram();
    glAttachShader(m_ID, computeShader);
    glLinkProgram(m_ID);

    int success;
    char infoLog[512];

    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        BIT_LOG_ERROR("SHADER LINKING ERROR:\n%s", infoLog);
    }

    glDeleteShader(computeShader); 
}
OpenGLComputeShader::~OpenGLComputeShader()
{

}
void OpenGLComputeShader::Dispatch(u32 workGroupSizeX, u32 workGroupSizeY, u32 workGroupSizeZ) 
{
   glUseProgram(m_ID);
   glDispatchCompute(workGroupSizeX, workGroupSizeY, workGroupSizeZ);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

char* OpenGLComputeShader::ParseComputeShader(const char* path)
{
    FILE* f = fopen(path, "r");
    if(f == NULL)
    {
        BIT_LOG_ERROR("Failed to open the file %s", path);
    }
    fseek(f, 0, SEEK_END);
    u64 fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* computeShaderSource = (char*)malloc(sizeof(char) * fileSize + 1);
    
    char c;
    i32 i = 0;
    printf("computeshader: \n");
    while((c = fgetc(f)) != EOF)
    {
        computeShaderSource[i++] = c;
        printf("%c", c);
    }
    computeShaderSource[i] = '\0';

    fclose(f);
    return computeShaderSource;
}

}

