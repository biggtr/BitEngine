#pragma once
#include <string>

namespace BitEngine
{

class Shader
{
    
public:
    
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& uniformName, int uniformValue) = 0;
    virtual void SetFloat(const std::string& uniformName, float v1) = 0;
    virtual void SetFloat2(const std::string& uniformName, float v1, float v2) = 0;
    virtual void SetFloat3(const std::string& uniformName, float v1, float v2, float v3) = 0;
    virtual void SetFloat4(const std::string& uniformName, float v1, float v2, float v3, float v4) = 0;

    static Shader* Create(const std::string& path);
};

}
