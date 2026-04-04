#pragma once
#include "Bit/Renderer/ComputeShader.h"
#include <string>


#pragma once

namespace BitEngine
{

class OpenGLComputeShader : public ComputeShader
{
public:
    OpenGLComputeShader(const char* path);
    ~OpenGLComputeShader();
    virtual void Dispatch(u32 workGroupSizeX, u32 workGroupSizeY, u32 workGroupSizeZ) override;

    virtual const char* GetPath() override { return m_Path; }

    virtual u32 GetID() override { return m_ID; }; 

private:
    char* ParseComputeShader(const char* path);
    u32 CompileShader();
    void CreateProgram();
    u32 m_ID;
    const char* m_Path;
    
    u32 m_WorkGroupSizeX;
    u32 m_WorkGroupSizeY;
    u32 m_WorkGroupSizeZ;

};
}
