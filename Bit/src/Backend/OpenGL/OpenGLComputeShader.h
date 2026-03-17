#pragma once
#include "Bit/Renderer/ComputeShader.h"
#include <string>


#pragma once

namespace BitEngine
{

class OpenGLComputeShader : public ComputeShader
{
public:
    OpenGLComputeShader(const std::string& path);
    ~OpenGLComputeShader();
    virtual void Dispatch() override;

    virtual const std::string& GetName() override;
    virtual void SetName(const std::string& name) override;

    virtual u32 GetID() override; 

private:
    std::string ParseComputeShader(const std::string& path);
    u32 m_ID;

};
}
