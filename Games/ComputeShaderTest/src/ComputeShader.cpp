#include <glad/glad.h>
#include "ComputeShader.h"
#include "Bit/Math/Random.h"
#include <Bit/Renderer/ComputeShader.h>

void ComputeShaderTest::Initialize() 
{
    BitEngine::ComputeShader* compute = BitEngine::ComputeShader::Create("assets/shaders/ComputeShader.glsl");
    const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
    unsigned int texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, 
                 GL_FLOAT, NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    compute->Dispatch(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);


}
void ComputeShaderTest::Update(float deltaTime) 
{

}
void ComputeShaderTest::Render3D() 
{

}
void ComputeShaderTest::RenderUI()
{

}
