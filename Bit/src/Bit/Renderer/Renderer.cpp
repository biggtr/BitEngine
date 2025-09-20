#include "Renderer.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/RenderCommand.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Resources/GeometryManager.h"
#include "Bit/Resources/MaterialManager.h"
#include "Bit/Resources/ShaderManager.h"
#include <algorithm>
#include <cstdlib>
namespace BitEngine
{


Renderer::Renderer()
    : m_RenderCommand(nullptr)
    , m_GeometryManager(nullptr)
    , m_MaterialManager(nullptr)
    , m_ShaderManager(nullptr)
    , m_ViewMatrix()
    , m_ProjectionMatrix()
    , m_ViewProjectionMatrix()
{

}
Renderer::~Renderer()
{
    Shutdown();
}

b8 Renderer::Initialize()
{
    m_RenderCommand = new RenderCommand();
    if(!m_RenderCommand->Initialize())
    {
        BIT_LOG_ERROR("Failed to initialize RenderCommand");
        return false;
    }
    m_GeometryManager = new GeometryManager();
    
    m_ShaderManager = new ShaderManager();
    m_MaterialManager = new MaterialManager(m_ShaderManager);

    m_ShaderManager->LoadBuiltinShaders();
    m_GeometryManager->LoadBuiltinPrimitives();
    m_MaterialManager->LoadBuiltinMaterials();

    // Set Default clear color 
    SetClearColor(0.2f, 0.4f, 0.0f, 1.0f);
    return true;
}
void Renderer::Shutdown()
{
    m_RenderQueue.clear();
    delete m_MaterialManager;
    delete m_ShaderManager;
    delete m_GeometryManager;
    delete m_RenderCommand;

    m_MaterialManager = nullptr;
    m_ShaderManager = nullptr;
    m_GeometryManager = nullptr;
    m_RenderCommand = nullptr;
}

b8 Renderer::BeginFrame(const BMath::Vec4& clearColor, BMath::Mat4& ViewProjection)
{
    if(!ValidateRenderState())
    {
        return false;
    }
    m_ViewProjectionMatrix = ViewProjection;

    SetClearColor(clearColor);
    Clear();

    m_RenderQueue.clear();
    ResetStats();
    return true;
}
b8 Renderer::EndFrame()
{
    ProcessRenderQueue();
    return true;
}

void Renderer::Submit(Geometry* geometry)
{
    Submit(geometry, geometry->GetMaterial(), geometry->GetTransform());
}
void Renderer::Submit(Geometry* geometry, Material* material)
{
    geometry->SetMaterial(material);
    Submit(geometry, material, geometry->GetTransform());
}

void Renderer::Submit(Geometry* geometry, const BMath::Mat4& transform)
{
    Submit(geometry, geometry->GetMaterial(), transform);
}
void Renderer::Submit(Geometry* geometry, Material* material, const BMath::Mat4& transform)
{
    if (!ValidateRenderState() || !geometry) return;
    
    if (!material) 
    {
        material = m_MaterialManager->GetMaterial("Default");
    }
    
    if (!material) 
    {
        BIT_LOG_ERROR("No material available for geometry %s", geometry->GetName().c_str());
        return;
    }
    
    if (!geometry->IsUploaded()) 
    {
        geometry->UploadToGPU();
    }
    
    struct RenderItem item;
    item.GeometryPtr = geometry;
    item.MaterialPtr = material;
    item.Transform = transform;
    
    m_RenderQueue.push_back(item);
    m_RenderStats.GeometriesSubmitted++;
}

void Renderer::SetClearColor(f32 r, f32 g, f32 b, f32 a)
{
    SetClearColor({r, g, b, a});
}
void Renderer::SetClearColor(const BMath::Vec4& color)
{
    m_RenderCommand->SetClearColor(color);
}
void Renderer::Clear()
{
    m_RenderCommand->Clear();
}
void Renderer::SetViewport(u32 x, u32 y, u32 width, u32 height)
{
    m_RenderCommand->SetViewport(x, y, width, height);
}

void Renderer::ProcessRenderQueue()
{
    if(m_RenderQueue.empty())
        return;
    SortRenderQueue();
    for(const auto& item : m_RenderQueue)
    {
        RenderItem(item);
    }
}
void Renderer::SortRenderQueue()
{
    std::sort(m_RenderQueue.begin(), m_RenderQueue.end(),
            [](const struct RenderItem& a, const struct RenderItem& b)
                {
                    return a.MaterialPtr < b.MaterialPtr;
                }
            );
}
void Renderer::RenderItem(const struct RenderItem& item)
{
    Geometry* geometry = item.GeometryPtr;
    Material* material = item.MaterialPtr;
    BMath::Mat4 transform = item.Transform;
    if (!geometry || !material) return;

    VertexArray* vao = geometry->GetVertexArray();
    if(!vao) return;

    material->Bind();
    auto shader = material->GetShader();
    if (shader) 
    {
        shader->SetMat4("u_ViewProjection", m_ViewProjectionMatrix);
        shader->SetMat4("u_Model", transform);
    }
    u32 indexCount = geometry->GetIndexCount();
    m_RenderCommand->DrawIndexed(vao, indexCount);

    m_RenderStats.DrawCalls++;
    m_RenderStats.Triangles += indexCount / 3;
    m_RenderStats.GeometriesSubmitted++;
    m_RenderStats.Vertices += indexCount;
}
bool Renderer::ValidateRenderState() const
{
    return m_RenderCommand && m_GeometryManager && m_MaterialManager && m_ShaderManager;
}
}
