#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/ShaderManager.h"

namespace BitEngine
{


class OpenGLShader : public Shader
{
    
public:
    OpenGLShader(const std::string& name, const ShaderSource& sources);

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const std::string& uniformName, int uniformValue) override;
    virtual void SetIntArray(const std::string& uniformName, int* uniformValue, u32 count) override;
    virtual void SetFloat(const std::string& uniformName, float v1) override;
    virtual void SetFloat2(const std::string& uniformName, const BMath::Vec2& vec2) override;
    virtual void SetFloat3(const std::string& uniformName, const BMath::Vec3& vec3) override;
    virtual void SetFloat4(const std::string& uniformName, const BMath::Vec4& vec4) override;
    virtual void SetMat4(const std::string& uniformName, const BMath::Mat4& mat) override;
    virtual const std::string& GetName() override;
    virtual u32 GetID() override;
    virtual void SetName(const std::string& name) override;

private: 
    unsigned int CompileShader(unsigned int shaderType,const std::string& shaderSource);
    ShaderSource ParseShader(const std::string& path);
    void CreateProgram();
    int GetUniformLocation(const std::string& uniformName);
    ShaderSource m_ShaderSources;


private:
    unsigned int m_ID;
    std::string m_Name;
};

}
