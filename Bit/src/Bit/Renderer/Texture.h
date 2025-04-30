#pragma once
#include "Bitpch.h"

namespace BitEngine 
{

class Texture
{

public:
    Texture(const std::string& filePath);
    ~Texture();

    void Bind(unsigned int slot);
    void Unbind();
    
    unsigned int GetID() { return m_ID; }
private:
    unsigned int m_ID;
    std::string m_FilePath;
    int m_Width, m_Height, m_Channels;
    std::vector<uint8_t> m_PixelData;
};

}
