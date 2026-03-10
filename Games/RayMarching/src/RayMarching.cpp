#include "RayMarching.h"
#include "Bit/Math/BMath.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Geometry.h"

BitEngine::Geometry* shape;
BitEngine::Material* raymarchingMaterial;
void RayMarching::Initialize()
{
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::PRESPECTIVE);
    shape = m_Renderer3D->GetGeometryManager()->CreateQuad("custom quad", 2.0f);
    m_Renderer3D->GetShaderManager()->LoadShader("raymarchingShader", "assets/shaders/raymarching.glsl");
    raymarchingMaterial = m_Renderer3D->GetMaterialManager()->CreateMaterial("raymarchingMaterial", "raymarchingShader");
    raymarchingMaterial->SetVec2("screenRes", {(f32)m_AppConfig.width, (f32)m_AppConfig.height});;
    shape->SetMaterial(raymarchingMaterial);
    
}
void RayMarching::Update(f32 deltaTime)
{
    f32 vpW = (f32)m_Renderer3D->GetViewportWidth();
    f32 vpH = (f32)m_Renderer3D->GetViewportHeight();
    BIT_LOG_DEBUG("ratio: %.4f", vpW / vpH); 
    raymarchingMaterial->SetFloat("screenx", vpW);
    raymarchingMaterial->SetFloat("screeny", vpH);
}
void RayMarching::Render3D()
{
    m_Renderer3D->Submit(shape);
}
void RayMarching::RenderUI()
{

}

