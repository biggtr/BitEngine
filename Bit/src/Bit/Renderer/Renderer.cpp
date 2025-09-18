#include "Renderer.h"
#include "Bit/Core/Defines.h"
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Renderer/Material.h"

#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/ResourceTypes.h"
#include <array>
#include <cstdlib>
#include <vector>

namespace BitEngine
{

#define MAX_GEOMETRY_NUMBER 4096
#define MAX_MATERIAL_NUMBER 4096
struct RendererState
{
    BMath::Mat4 ViewProjectionMat;
    RenderCommand* RenderCmd;
    ShaderLibrary* ShaderLib;
    std::array<Geometry*, MAX_GEOMETRY_NUMBER> Geometries;
    std::array<Material*, MAX_MATERIAL_NUMBER> Materials;
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
    material->InternalID = shader->GetID();

}
void RendererSubmit(RenderData data)
{
    Geometry* geometry = data.Gmtry;
    Material* material = geometry->GetMaterial();
    
    Shader* shader = rendererState->ShaderLib->Get(material->InternalID);
    shader->SetMat4("u_ViewProjection", rendererState->ViewProjectionMat);
    for(auto& property : material->Properties)
    {
        switch (property.second.Type) 
        {

        case MATERIAL_VALUE_TYPE::FLOAT:
            shader->SetFloat(property.first, property.second.value);
        case MATERIAL_VALUE_TYPE::FLOAT2:
            shader->SetFloat2(property.first, property.second.float2);
        case MATERIAL_VALUE_TYPE::FLOAT3:
            shader->SetFloat3(property.first, property.second.float3);
        case MATERIAL_VALUE_TYPE::FLOAT4:
            shader->SetFloat4(property.first, property.second.float4);
          break;
        }
    }
    
    VertexArray* vao = geometry->GetVAO();

    rendererState->RenderCmd->DrawIndexed(vao, vao->GetIndexBuffer()->GetCount());
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
