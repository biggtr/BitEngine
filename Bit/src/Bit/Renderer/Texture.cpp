#include "Texture.h"
#include <glad/glad.h>
#include "ImageLoaders/BMPLoader.h"


namespace BitEngine 
{

Texture::Texture(const std::string& filePath)
    : m_FilePath(filePath)
{

    BMPLoader imageLoader;
    m_PixelData = imageLoader.Load(m_FilePath, m_Width, m_Height, m_Channels);
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_PixelData.data());
    glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
    glBindTexture(0, m_ID);
}

Texture::~Texture()
{

}
}
