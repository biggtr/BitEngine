#include "OpenGLShader.h"
#include <glad/glad.h>
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
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

void OpenGLShader::SetInt(const char* uniformName, int uniformValue)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1i(uniformLocation, uniformValue));
}
void OpenGLShader::SetIntArray(const char* uniformName, int* uniformValue, u32 count)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1iv(uniformLocation, count, uniformValue));
}
void OpenGLShader::SetFloat(const char* uniformName, float v1)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1f(uniformLocation, v1));
}
void OpenGLShader::SetFloat2(const char* uniformName, const BMath::Vec2& vec2)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform2f(uniformLocation, vec2.x, vec2.y));
}
void OpenGLShader::SetFloat3(const char* uniformName, const BMath::Vec3& vec3)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform3f(uniformLocation, vec3.x, vec3.y, vec3.z));
}
void OpenGLShader::SetFloat4(const char* uniformName, const BMath::Vec4& vec4)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform4f(uniformLocation, vec4.x, vec4.y, vec4.z, vec4.w));
}
void OpenGLShader::SetMat4(const char* uniformName, const BMath::Mat4& mat)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, mat.Data));
}


int OpenGLShader::GetUniformLocation(const std::string& uniformName)
{
    GLCall(int uniformLocation = glGetUniformLocation(m_ID, uniformName.c_str()));
    if(uniformLocation == -1)
    {
        // BIT_CORE_ERROR("No such a thing as uniform location ");
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


