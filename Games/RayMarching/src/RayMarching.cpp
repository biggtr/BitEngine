#include "RayMarching.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
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
    static f32 accumelatedTime= 0.0f;
    accumelatedTime += deltaTime;
    f32 vpW = (f32)m_Renderer3D->GetViewportWidth();
    f32 vpH = (f32)m_Renderer3D->GetViewportHeight();
    raymarchingMaterial->SetFloat("screenx", vpW);
    raymarchingMaterial->SetFloat("screeny", vpH);
    raymarchingMaterial->SetFloat("time", accumelatedTime);

    i32 x, y;
    BitEngine::InputGetMousePosition(&x, &y);
    raymarchingMaterial->SetVec2("mouseInput", {(f32)x, (f32)y});
}
void RayMarching::Render3D()
{
    m_Renderer3D->Submit(shape);
}
void RayMarching::RenderUI()
{

}

