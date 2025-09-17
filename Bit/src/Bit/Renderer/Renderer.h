#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/ResourceTypes.h"
namespace BitEngine
{

struct Geometry
{

};
b8 RendererInitialize();
b8 RendererBeginFrame(const BMath::Mat4& viewProjectionMatrix);
void CreateMaterialShader(Material* material, const char* shaderPath);
void RendererSubmit(Material* material, Geometry* geometry);
b8 RendererEndFrame();
void RendererShutdown();

}
