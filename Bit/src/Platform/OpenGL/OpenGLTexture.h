#pragma once 
#include "Bit/Renderer/Texture.h"
#include <cstdint>
#include <sys/types.h>
namespace BitEngine 
{

class OpenGLTexture : public Texture
{

public:
    OpenGLTexture(uint32_t width, uint32_t height);
    OpenGLTexture(const char* path);
    virtual u32 GetID() const override;
    virtual void Bind(unsigned int slot = 0) const override;
    virtual void Unbind() const override;
    virtual int GetWidth() const override { return m_Width; }; 
    virtual int GetHeight() const override { return m_Height; };
    virtual void SetData(void* data, uint32_t size) override;
private:
    u32 m_ID;
    const char* m_FilePath;
    int m_Width, m_Height, m_Channels;
    unsigned int  m_DataFormat; 
};

}
