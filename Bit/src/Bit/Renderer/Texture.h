#include "Bitpch.h"

namespace BitEngine 
{

class Texture
{

public:
    Texture(const std::string& filePath);
    ~Texture();

    void Bind();
    void Unbind();
    
private:
    unsigned int m_ID;
    std::string m_FilePath;
    int m_Width, m_Height, m_Channels;
    std::vector<uint8_t> m_PixelData;
};

}
