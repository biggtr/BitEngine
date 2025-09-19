#pragma once

#include "Bit/Renderer/Shader.h"
#include <vector>
namespace BitEngine
{

class ShaderManager 
{
private:
    std::unordered_map<std::string, Shader*> m_Shaders;
    Shader* m_FallBackShader;
public:
    ShaderManager();
    ~ShaderManager();

    Shader* LoadShader(const std::string& name, const std::string& filepath);
    Shader* LoadShaderFromSource(const std::string& name, 
                                                  const std::string& vertexSrc, 
                                                  const std::string& fragmentSrc);
    
    Shader* GetShader(const std::string& name);
    b8 RemoveShader(const std::string& name);
    bool HasShader(const std::string& name) const;
    void Clear();
    
    void LoadBuiltinShaders();
    
    std::vector<std::string> GetShaderNames() const;
    u32 GetShaderCount() const { return static_cast<u32>(m_Shaders.size()); }
    
    void PrintAllShaders() const;
    void PrintShaderInfo(const std::string& name) const;
    void LogStatistics() const;
    
private:
    ShaderSource ParseShaderFile(const std::string& filepath);
    Shader* CompileShader(const std::string& name, const ShaderSource& source);
    
    b8 CacheShader(const std::string& name, const ShaderSource& source);
    b8 LoadCachedShader(const std::string& name, ShaderSource& outSource);
    
    std::string LoadFileContent(const std::string& filepath);
    b8 FileExists(const std::string& filepath) const;
    
    void CreateDefaultShaders();
};
}

