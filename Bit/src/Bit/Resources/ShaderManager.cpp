#include "ShaderManager.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Shader.h"

namespace BitEngine
{

ShaderManager::ShaderManager()
{
    CreateDefaultShaders();
}
ShaderManager::~ShaderManager()
{
    Clear();
}


Shader* ShaderManager::LoadShader(const std::string& name, const std::string& filepath)
{
    if(HasShader(name))
    {
        BIT_LOG_WARN("This Shader %s Already Exists..", name.c_str());
        return GetShader(name);
    }

    if(!FileExists(filepath))
    {
        BIT_LOG_ERROR("Shader File is not found..");
        return m_FallBackShader;
    }
    //TODO: Add Chached shaders and hot realoding in here 

    ShaderSource sources = ParseShaderFile(filepath);
    Shader* shader = CompileShader(name, sources);
    m_Shaders[name] = shader;
    return m_Shaders[name];
}
Shader* ShaderManager::LoadShaderFromSource(const std::string& name, 
        const std::string& vertexSrc, 
        const std::string& fragmentSrc)
{
    if(HasShader(name))
    {
        BIT_LOG_WARN("This Shader %s Already Exists..", name.c_str());
        return GetShader(name);
    }
    ShaderSource sources; 
    sources.VertexShaderSource = vertexSrc;
    sources.FragmentShaderSource = fragmentSrc;
    Shader* shader = CompileShader(name, sources);
    m_Shaders[name] = shader;
    return m_Shaders[name];
}


Shader* ShaderManager::GetShader(const std::string& name)
{
    if(!HasShader(name))
    {
        BIT_LOG_WARN("Shader %s Not Exists..", name.c_str());
        return m_FallBackShader;
    }
    return m_Shaders[name];
}
b8 ShaderManager::RemoveShader(const std::string& name)
{
    if(!HasShader(name))
    {
        BIT_LOG_WARN("Shader %s Not Exists..", name.c_str());
        return false;
    }
    return m_Shaders.erase(name);
}
b8 ShaderManager::HasShader(const std::string& name) const
{
    return m_Shaders.contains(name);
}
void ShaderManager::Clear()
{
    for(auto& shader : m_Shaders)
    {
        delete shader.second;
    }
    delete m_FallBackShader;
}




std::vector<std::string> ShaderManager::GetShaderNames() const
{
    std::vector<std::string> shaderNames;
    for(auto& shader : m_Shaders)
    {
        shaderNames.push_back(shader.first);
    }
    return shaderNames;
}



void ShaderManager::PrintAllShaders() const
{

}
void ShaderManager::PrintShaderInfo(const std::string& name) const
{

}
void ShaderManager::LogStatistics() const
{

}

b8 ShaderManager::CacheShader(const std::string& name, const ShaderSource& source)
{
    return false;
}
b8 ShaderManager::LoadCachedShader(const std::string& name, ShaderSource& outSource)
{
    return false;
}

std::string ShaderManager::LoadFileContent(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) 
    {
        BIT_LOG_ERROR("Could not open file: %s", filepath.c_str());
        return "";
    }
    std::string buffer;
    std::string line;
    while(std::getline(file, line))
    {
        buffer.append(line);
    }
    file.close();
    return buffer;
}
b8 ShaderManager::FileExists(const std::string& filepath) const
{
    return std::filesystem::exists(filepath);
}

void ShaderManager::CreateDefaultShaders()
{

}
    
void ShaderManager::LoadBuiltinShaders()
{

}
//TODO: Refactor this and make it more robust and support other shader types
ShaderSource ShaderManager::ParseShaderFile(const std::string& filepath)
{
    SHADER_TYPE currentShader = SHADER_TYPE::NONE;
    ShaderSource shaderSources;
    if(!FileExists(filepath))
    {
        BIT_LOG_ERROR("Failed To Open The Shader File!.. ");
        return {};
    }
    std::string shaderFile = LoadFileContent(filepath);
    std::istringstream stream(shaderFile);
    std::string currentLine;
    while (std::getline(stream, currentLine)) 
    {
        if(currentLine.find("Vertex") != std::string::npos) 
        {
            currentShader = SHADER_TYPE::VERTEX;
            shaderSources.VertexShaderSource.append("#version 330 core\n");
            continue;
        }
        else if(currentLine.find("Fragment") != std::string::npos)
        {
            currentShader = SHADER_TYPE::FRAGMENT;
            shaderSources.FragmentShaderSource.append("#version 330 core\n");
            continue;
        }
        switch (currentShader) 
        {
            case SHADER_TYPE::VERTEX:
                shaderSources.VertexShaderSource += currentLine + '\n';
                break;
            case SHADER_TYPE::FRAGMENT:
                shaderSources.FragmentShaderSource += currentLine + '\n';
                break;
            case SHADER_TYPE::NONE:
                //Ignoring Lines before the shader tag
                break;
        }
    }
    return  shaderSources;
}
Shader* ShaderManager::CompileShader(const std::string& name, const ShaderSource& source)
{
    Shader* shader = Shader::Create(name, source);
    m_Shaders[name] = shader;
    return m_Shaders[name];
}
    


}
