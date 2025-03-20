#include <cstddef>
#include <iostream>
#include "Shader.h"
#include <fstream>
#include <string>
#include "glad/glad.h"

namespace BitEngine
{

    enum class SHADER_TYPE
    {
        NONE, VERTEX, FRAGMENT
    };
    Shader::Shader(const std::string path)
    {
        ShaderSources sources = ParseShader(path);

        unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, sources.VertexShaderSource);
        unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, sources.FragmentShaderSource);
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        glUseProgram(m_ID);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);  
    }

    void Shader::Bind()
    {
        glUseProgram(m_ID);
    }

    void UnBind()
    {
        glUseProgram(0);
    }
    
    unsigned int Shader::CompileShader(unsigned int shaderType,const std::string& shaderSource)
    {
        unsigned int shader = glCreateShader(shaderType);
        const char* source = shaderSource.c_str();
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader,GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return shader;
    }
    ShaderSources ParseShader(const std::string path)
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
            if(currentLine == "#Vertex") 
            {
                currentShader = SHADER_TYPE::VERTEX;
                continue;
            }
            else if(currentLine == "#Fragment")
            {
                currentShader = SHADER_TYPE::FRAGMENT;
                continue;
            }
            switch (currentShader) 
            {
                case SHADER_TYPE::VERTEX:
                    shaderSources.VertexShaderSource += currentLine + '\n';
                case SHADER_TYPE::FRAGMENT:
                    shaderSources.FragmentShaderSource += currentLine + '\n';
                case SHADER_TYPE::NONE:
                    //Ignoring Lines before the shader tag
                    break;
            }
        }
        return  shaderSources;
    }


}


