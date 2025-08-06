#pragma once
#include "Bit/Math/Matrix.h"
#include <string>

namespace BitEngine
{

class Shader
{
    
public:
       
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const char* uniformName, int uniformValue) = 0;
    virtual void SetFloat(const char*  uniformName, float v1) = 0;
    virtual void SetFloat2(const char* uniformName, const BMath::vec2& vec2) = 0;
    virtual void SetFloat3(const char* uniformName, const BMath::vec3& vec3) = 0;
    virtual void SetFloat4(const char* uniformName, const BMath::vec4& vec4) = 0;
    virtual void SetMat4(const char* uniformName, const BMath::Matrix4x4& mat) = 0;

    static Shader* Create(const char* path);
};

}
