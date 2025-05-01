#include "Texture.h"
#include <glad/glad.h>
#include "ImageLoader.h"
#include "Bit/Core/Core.h"


namespace BitEngine 
{

Texture::Texture(const std::string& filePath)
    : m_FilePath(filePath)
{

    m_PixelData = ImageLoader::LoadImage(m_FilePath, m_Width, m_Height, m_Channels);
    GLCall(glGenTextures(1, &m_ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_PixelData.data()));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

}

void Texture::Bind(unsigned int slot)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(0, m_ID));
}

Texture::~Texture()
{

}
}
