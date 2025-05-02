#pragma once
#include "Bit/Renderer/Shader.h"

namespace BitEngine
{

struct ShaderSources
{
    std::string VertexShaderSource;
    std::string FragmentShaderSource;
};

class OpenGLShader : public Shader
{
    
public:
    OpenGLShader(const std::string& path);

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const std::string& uniformName, int uniformValue) override;
    virtual void SetFloat(const std::string& uniformName, float v1) override;
    virtual void SetFloat2(const std::string& uniformName, float v1, float v2) override;
    virtual void SetFloat3(const std::string& uniformName, float v1, float v2, float v3) override;
    virtual void SetFloat4(const std::string& uniformName, float v1, float v2, float v3, float v4) override;

    void SetUniformInt(const std::string& uniformName, int value);
    void SetUniformFloat(const std::string& uniformName, float v1);
    void SetUniformFloat2(const std::string& uniformName, float v1, float v2);
    void SetUniformFloat3(const std::string& uniformName, float v1, float v2, float v3);
    void SetUniformFloat4(const std::string& uniformName, float v1, float v2, float v3, float v4);
private: 
    unsigned int CompileShader(unsigned int shaderType,const std::string& shaderSource);
    ShaderSources ParseShader(const std::string& path);
    void CreateProgram();
    int GetUniformLocation(const std::string& uniformName);
    ShaderSources m_ShaderSources;


private:
    unsigned int m_ID;
};

}
