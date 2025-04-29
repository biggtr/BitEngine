#pragma once

#include "Bitpch.h"

namespace BitEngine
{
struct ShaderSources
{
    std::string VertexShaderSource;
    std::string FragmentShaderSource;
};

class Shader
{
private:
    unsigned int m_ID;
    
public:
    Shader(std::string path);

    void Bind();

    void Unbind();

    void SetUniform1i(const std::string& uniformName, int uniformValue);
    void SetUniform4f();
    void SetUniform3f();

private: 
    unsigned int CompileShader(unsigned int shaderType,const std::string& shaderSource);
    ShaderSources ParseShader(const std::string path);
    int GetUniformLocation(const std::string& uniformName);


};

}
