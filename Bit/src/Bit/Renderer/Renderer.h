#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/Geometry.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/ResourceTypes.h"

namespace BitEngine
{
class VertexBuffer;
class IndexBuffer;
class VertexArray;

struct RenderData
{
    BMath::Mat4 Model;
    Geometry* Gmtry;
};

b8 RendererInitialize();
b8 RendererBeginFrame(const BMath::Mat4& viewProjectionMatrix);
b8 RendererEndFrame();
void CreateMaterialShader(Material* material, const char* shaderPath);
void RendererSubmit(RenderData data);
void RendererShutdown();

}
