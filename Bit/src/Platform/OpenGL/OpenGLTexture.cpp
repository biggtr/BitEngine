#include "OpenGLTexture.h"
#include <algorithm>
#include <cstdint>
#include <glad/glad.h>
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "stb_image.h"
namespace BitEngine 
{
OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
    :   m_Width(width), m_Height(height), m_Channels(4)
{
    m_DataFormat = GL_RGBA;
    GLCall(glGenTextures(1, &m_ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}
OpenGLTexture::OpenGLTexture(const char* path)
    : m_FilePath(path)
{
    int width, height, nrChannels = 0;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if(data)
    {
        m_Width = width;
        m_Height = height;
        m_Channels = nrChannels;
        BIT_LOG_DEBUG("texture Width : %d, height : %d, channels : %d ", width, height, nrChannels);
    }
    m_DataFormat = m_Channels == 4 ? GL_RGBA : GL_RGB;
    GLCall(glGenTextures(1, &m_ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_DataFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));
    // GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    stbi_image_free(data);
}

void OpenGLTexture::SetData(void* data, uint32_t size) 
{
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_DataFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));
}
void OpenGLTexture::Bind(unsigned int slot) const 
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void OpenGLTexture::Unbind() const
{
    GLCall(glBindTexture(0, m_ID));
}


}
