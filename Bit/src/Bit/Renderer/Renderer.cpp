#include "Renderer.h"
#include "Bit/Core/Defines.h"
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/ResourceTypes.h"
#include <cstdlib>

namespace BitEngine
{

struct RendererState
{
    BMath::Mat4 ViewProjectionMat;
    RenderCommand* RenderCmd;
    ShaderLibrary* ShaderLib;
};
static RendererState* rendererState = 0;
b8 RendererInitialize()
{
    rendererState = (RendererState*)malloc(sizeof(RendererState));
    rendererState->ShaderLib = new ShaderLibrary();
    rendererState->RenderCmd = new RenderCommand();
    RendererAPI::SetAPI(RENDERER_API::OPENGL);
    if(!rendererState->RenderCmd->Init())
    {
        BIT_LOG_ERROR("failed to Initialize RenderCommand");
        return false;
    }

    return true;
}
b8 RendererBeginFrame(const BMath::Mat4& viewProjectionMatrix)
{
    rendererState->RenderCmd->SetClearColor({0.4f, 0.4, 0.0f, 1.0f});
    rendererState->RenderCmd->Clear();
    rendererState->ViewProjectionMat = viewProjectionMatrix;
    return true;
}
void RendererCreateMaterialShader(Material* material, const char* shaderPath)
{
    Shader* shader = rendererState->ShaderLib->Load(shaderPath);
    material->ShaderID = shader->GetID();
}
void RendererSubmit(Material* material, Geometry* geometry)
{
    shader = material.getshader();
    shader.bind();
    shader.setviewprojmat(rendererState->ViewProjectionMat);
    rendererState->RenderCmd->DrawIndexed(geometry.vao, geometry.vao.ibo.count);
}
b8 RendererEndFrame()
{

    return true;
}
void RendererShutdown()
{
    delete rendererState->ShaderLib;
    free(rendererState);
}


}
