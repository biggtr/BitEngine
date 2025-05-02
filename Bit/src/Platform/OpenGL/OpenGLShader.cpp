#include "OpenGLShader.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
namespace BitEngine
{

enum class SHADER_TYPE
{
    NONE, VERTEX, FRAGMENT 
};

OpenGLShader::OpenGLShader(const std::string& path)
{
    m_ShaderSources = ParseShader(path);
    CreateProgram();
}

void OpenGLShader::Bind() const 
{
    GLCall(glUseProgram(m_ID));
}

void OpenGLShader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void OpenGLShader::SetInt(const std::string& uniformName, int uniformValue)
{
    SetUniformInt(uniformName, uniformValue);
}
void OpenGLShader::SetFloat(const std::string& uniformName, float v1)
{
    SetUniformFloat(uniformName, v1);
}
void OpenGLShader::SetFloat2(const std::string& uniformName, float v1, float v2)
{
    SetUniformFloat2(uniformName, v1, v2);

}
void OpenGLShader::SetFloat3(const std::string& uniformName, float v1, float v2, float v3)
{

    SetUniformFloat3(uniformName, v1, v2, v3);
}
void OpenGLShader::SetFloat4(const std::string& uniformName, float v1, float v2, float v3, float v4)
{
    SetUniformFloat4(uniformName, v1, v2, v3, v4);
}

void OpenGLShader::SetUniformInt(const std::string& uniformName, int value)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1i(uniformLocation, value));
}
void OpenGLShader::SetUniformFloat(const std::string& uniformName, float v1)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1f(uniformLocation, v1));
}
void OpenGLShader::SetUniformFloat2(const std::string& uniformName, float v1, float v2)
{

    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform2f(uniformLocation, v1, v2));

}
void OpenGLShader::SetUniformFloat3(const std::string& uniformName, float v1, float v2, float v3)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform3f(uniformLocation, v1, v2, v3));
}
void OpenGLShader::SetUniformFloat4(const std::string& uniformName, float v1, float v2, float v3, float v4)
{

    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform4f(uniformLocation, v1, v2, v3, v4));
}

int OpenGLShader::GetUniformLocation(const std::string& uniformName)
{
    GLCall(int uniformLocation = glGetUniformLocation(m_ID, uniformName.c_str()));
    if(uniformLocation == -1)
    {
        BIT_CORE_ERROR("No such a thing as uniform location ");
    }
    return uniformLocation; 
}

unsigned int OpenGLShader::CompileShader(unsigned int shaderType,const std::string& shaderSource)
{
    GLCall(unsigned int shader = glCreateShader(shaderType));
    const char* source = shaderSource.c_str();
    
    GLCall(glShaderSource(shader, 1, &source, NULL));
    GLCall(glCompileShader(shader));

    int success;
    char infoLog[512];
    GLCall(glGetShaderiv(shader,GL_COMPILE_STATUS, &success));
    if(!success)
    {
        GLCall(glGetShaderInfoLog(shader, 512, NULL, infoLog))
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}
ShaderSources OpenGLShader::ParseShader(const std::string& path)
{
    SHADER_TYPE currentShader = SHADER_TYPE::NONE;
    ShaderSources shaderSources;
    std::ifstream shaderFile(path);
    if(!shaderFile.is_open())
    {
        std::cerr << "Failed To Open The Shader File!.. ";
    }
    std::string currentLine;
    while (std::getline(shaderFile, currentLine)) 
    {
        if(currentLine.find("Vertex") != std::string::npos) 
        {
            currentShader = SHADER_TYPE::VERTEX;
            shaderSources.VertexShaderSource.append("#version 330 core\n");
            continue;
        }
        else if(currentLine.find("Fragment") != std::string::npos)
        {
            currentShader = SHADER_TYPE::FRAGMENT;
            shaderSources.FragmentShaderSource.append("#version 330 core\n");
            continue;
        }
        switch (currentShader) 
        {
            case SHADER_TYPE::VERTEX:
                shaderSources.VertexShaderSource += currentLine + '\n';
                break;
            case SHADER_TYPE::FRAGMENT:
                shaderSources.FragmentShaderSource += currentLine + '\n';
                break;
            case SHADER_TYPE::NONE:
                //Ignoring Lines before the shader tag
                break;
        }
    }
    return  shaderSources;
}

void OpenGLShader::CreateProgram()
{
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, m_ShaderSources.VertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, m_ShaderSources.FragmentShaderSource);
    unsigned int shaderProgram = glCreateProgram();
    GLCall(glAttachShader(shaderProgram , vertexShader));
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glLinkProgram(shaderProgram));
    GLint success;
    GLCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
    if(success == GL_FALSE)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "SHADER LINKING ERROR:\n" << infoLog << std::endl;
        GLCall(glDeleteProgram(shaderProgram));
        return;
    }
    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
    m_ID = shaderProgram;

}


}


