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

    virtual void SetInt(const char* uniformName, int uniformValue) = 0;
    virtual void SetIntArray(const char* uniformName, int* uniformValue, u32 count) = 0;
    virtual void SetFloat(const char*  uniformName, float v1) = 0;
    virtual void SetFloat2(const char* uniformName, const BMath::Vec2& vec2) = 0;
    virtual void SetFloat3(const char* uniformName, const BMath::Vec3& vec3) = 0;
    virtual void SetFloat4(const char* uniformName, const BMath::Vec4& vec4) = 0;
    virtual void SetMat4(const char* uniformName, const BMath::Mat4& mat) = 0;

    virtual const char* GetName() = 0;
    virtual void SetName(const char* name) = 0;

    virtual u32 GetID() = 0;
    static Shader* Create(const char* path);
};
class ShaderLibrary
{
public:
    ShaderLibrary();
    ~ShaderLibrary();
    
    void Add(const char* name, Shader* shader);
    void Add(Shader* shader);

    Shader* Load(const char* filePath);
    Shader* Load(const char* name, const char* filePath);

    Shader* Get(u32 shaderID);


private:
    std::unordered_map<u32, Shader*> m_Shaders;
};

}
