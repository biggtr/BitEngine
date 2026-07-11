#pragma once 
#include "Bit/Renderer/Texture.h"
#include <cstdint>
namespace BitEngine 
{

class OpenGLTexture : public Texture
{

public:
    OpenGLTexture(uint32_t width, uint32_t height, u32 dataFormat);
    OpenGLTexture(const char* name, const char* path, u32 dataFormat);
    virtual u32 GetID() const override;
    virtual const char* GetName() const override { return m_Name.c_str(); };
    virtual const char* GetPath() const override { return m_FilePath.c_str(); };
    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind() const override;
    virtual int GetWidth() const override { return m_Width; }; 
    virtual int GetHeight() const override { return m_Height; };
    virtual void SetData(void* data, uint32_t size) override;
private:
    u32 m_ID;
    std::string m_Name;
    std::string m_FilePath;
    int m_Width, m_Height, m_Channels;
    unsigned int  m_DataFormat; 
};

}
