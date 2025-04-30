#include "Shader.h"
#include "Bit/Core/Logger.h"
#include "glad/glad.h"
#include "Bit/Core/Core.h"
namespace BitEngine
{

enum class SHADER_TYPE
{
    NONE, VERTEX, FRAGMENT 
};
Shader::Shader(const std::string path)
{
    m_ShaderSources = ParseShader(path);
    CreateProgram();
}

void Shader::Bind()
{
    GLCall(glUseProgram(m_ID));
}

void Shader::Unbind()
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& uniformName, int uniformValue)
{
    int uniformLocation = GetUniformLocation(uniformName);
    GLCall(glUniform1i(uniformLocation, uniformValue));
}

int Shader::GetUniformLocation(const std::string& uniformName)
{
    GLCall(int uniformLocation = glGetUniformLocation(m_ID, uniformName.c_str()));
    if(uniformLocation == -1)
    {
        BIT_CORE_ERROR("No such a thing as uniform location ");
    }
    return uniformLocation; 
}

unsigned int Shader::CompileShader(unsigned int shaderType,const std::string& shaderSource)
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
ShaderSources Shader::ParseShader(const std::string path)
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

void Shader::CreateProgram()
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


