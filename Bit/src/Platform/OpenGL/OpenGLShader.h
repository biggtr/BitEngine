#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
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

    virtual void SetInt(const char* uniformName, int uniformValue) override;
    virtual void SetIntArray(const char* uniformName, int* uniformValue, u32 count) override;
    virtual void SetFloat(const char* uniformName, float v1) override;
    virtual void SetFloat2(const char* uniformName, const BMath::Vec2& vec2) override;
    virtual void SetFloat3(const char* uniformName, const BMath::Vec3& vec3) override;
    virtual void SetFloat4(const char* uniformName, const BMath::Vec4& vec4) override;
    virtual void SetMat4(const char* uniformName, const BMath::Mat4& mat) override;
    virtual const char* GetName() override;
    virtual u32 GetID() override;
    virtual void SetName(const char* name) override;

private: 
    unsigned int CompileShader(unsigned int shaderType,const std::string& shaderSource);
    ShaderSources ParseShader(const std::string& path);
    void CreateProgram();
    int GetUniformLocation(const std::string& uniformName);
    ShaderSources m_ShaderSources;


private:
    unsigned int m_ID;
    const char* m_Name;
};

}
