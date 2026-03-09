#include "RayMarching.h"
#include "Bit/Math/BMath.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Geometry.h"

BitEngine::Geometry* shape;
void RayMarching::Initialize()
{
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::PRESPECTIVE);
    shape = m_Renderer3D->GetGeometryManager()->CreateCube("custom cube", 2.0f);
    shape->Rotate(BMath::DegToRad(90.0f), {1.0f, 0.0f, 0.0});
    m_Renderer3D->GetShaderManager()->LoadShader("raymarchingShader", "assets/shaders/raymarching.glsl");
    BitEngine::Material* raymarchingMaterial = m_Renderer3D->GetMaterialManager()->CreateMaterial("raymarchingMaterial", "raymarchingShader");
    shape->SetMaterial(raymarchingMaterial);
}
void RayMarching::Update(f32 deltaTime)
{
    
}
void RayMarching::Render3D()
{
    m_Renderer3D->Submit(shape);
}
void RayMarching::RenderUI()
{

}

