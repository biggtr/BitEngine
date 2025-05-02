#pragma once 
#include "Bit/Renderer/Texture.h"
namespace BitEngine 
{

class OpenGLTexture : public Texture
{

public:
    OpenGLTexture(const std::string& path);
    virtual void Bind(unsigned int slot) const override;
    virtual void Unbind() const override;
    
private:
    unsigned int m_ID;
    std::string m_FilePath;
    int m_Width, m_Height, m_Channels;
    std::vector<uint8_t> m_PixelData;
};

}
