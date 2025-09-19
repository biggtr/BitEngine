#pragma once
#include "Bit/Math/Matrix.h"
#include <string>
#include <unordered_map>

namespace BitEngine
{

class Shader
{
    
public:
       
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& uniformName, int uniformValue) = 0;
    virtual void SetIntArray(const std::string& uniformName, int* uniformValue, u32 count) = 0;
    virtual void SetFloat(const std::string&  uniformName, float v1) = 0;
    virtual void SetFloat2(const std::string& uniformName, const BMath::Vec2& vec2) = 0;
    virtual void SetFloat3(const std::string& uniformName, const BMath::Vec3& vec3) = 0;
    virtual void SetFloat4(const std::string& uniformName, const BMath::Vec4& vec4) = 0;
    virtual void SetMat4(const std::string& uniformName, const BMath::Mat4& mat) = 0;

    virtual const std::string& GetName() = 0;
    virtual void SetName(const std::string& name) = 0;

    virtual u32 GetID() = 0;
    static Shader* Create(const std::string& path);
};

}
