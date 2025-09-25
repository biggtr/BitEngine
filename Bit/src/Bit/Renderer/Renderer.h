#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/Geometry.h"
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/Shader.h"
#include "Bit/Resources/GeometryManager.h"
#include "Bit/Resources/MaterialManager.h"
#include "Bit/Resources/ShaderManager.h"

namespace BitEngine
{
class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Mesh;

struct RenderingItem
{
    Geometry* GeometryPtr;
    Material* MaterialPtr;
    BMath::Mat4 Transform;
};
class Renderer
{
private:
    RenderCommand* m_RenderCommand;
    GeometryManager* m_GeometryManager;
    MaterialManager* m_MaterialManager;
    ShaderManager* m_ShaderManager;

    ////// RENDER STATE //////
    BMath::Mat4 m_ViewMatrix;
    BMath::Mat4 m_ProjectionMatrix;
    BMath::Mat4 m_ViewProjectionMatrix;

    std::vector<RenderingItem> m_RenderQueue;

    // === STATISTICS ===
    struct RenderStats
    {
        u32 DrawCalls = 0;
        u32 Triangles = 0;
        u32 Vertices = 0;
        u32 GeometriesSubmitted = 0;
    };
    RenderStats m_RenderStats;
public:
    Renderer();
    ~Renderer();

    b8 Initialize();
    void Shutdown();

    b8 BeginFrame(BMath::Mat4& ViewProjection, const BMath::Vec4& clearColor = { 0.23f, 0.0f, 1.0, 1.0});
    b8 EndFrame();

    void Submit(Mesh* mesh);
    void Submit(Geometry* geometry);
    void Submit(Geometry* geometry, Material* material);
    void Submit(Geometry* geometry, const BMath::Mat4& transform);
    void Submit(Geometry* geometry, Material* material, const BMath::Mat4& transform);

    void SetClearColor(const BMath::Vec4& color);
    void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
    void Clear();
    void SetViewport(u32 x, u32 y, u32 width, u32 height);
    
    const RenderStats& GetStats() const { return m_RenderStats; }
    void ResetStats()
    {
        m_RenderStats.DrawCalls = m_RenderStats.Triangles = m_RenderStats.Vertices = m_RenderStats.GeometriesSubmitted = 0;
    }

    GeometryManager* GetGeometryManager() { return m_GeometryManager; }
    MaterialManager* GetMaterialManager() { return m_MaterialManager; }
    ShaderManager* GetShaderManager() { return m_ShaderManager; }
private:


    void ProcessRenderQueue();
    void SortRenderQueue();
    void RenderItem(const RenderingItem& item);
    bool ValidateRenderState() const;
};

}
