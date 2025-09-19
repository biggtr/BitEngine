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

    struct RenderItem
    {
        Geometry* GeometryPtr;
        Material* MaterialPtr;
        BMath::Mat4 Transform;
    };
    std::vector<RenderItem> m_RenderQueue;

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

    b8 BeginFrame(const BMath::Vec4& clearColor, BMath::Mat4& ViewProjection);
    b8 EndFrame();

    void Submit(Geometry* geometry);
    void Submit(Geometry* geometry, const BMath::Mat4& transform);
    void Submit(Geometry* geometry, Material* material);
    void Submit(Geometry* geometry, Material* material, const BMath::Mat4& transform);

    void SetClearColor(const BMath::Vec4& color);
    void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
    void SetViewport(u32 width, u32 height);
    
    const RenderStats& GetStats() const { return m_RenderStats; }
    void Reset()
    {
        m_RenderStats.DrawCalls = m_RenderStats.Triangles = m_RenderStats.Vertices = m_RenderStats.GeometriesSubmitted = 0;
    }
    void ResetStats() { Reset(); }
private:


    void ProcessRenderQueue();
    void SortRenderQueue();
    void RenderItem(const RenderItem& item);
    f32 CalculateDistanceToCamera(const BMath::Vec3& position) const;
    bool ValidateRenderState() const;
    void UpdateViewProjectionMatrix();
};

}
