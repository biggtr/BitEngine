#include "3DGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Material.h"

BitEngine::Material* material;
BitEngine::Geometry* cubeGeometry;
BMath::Vec3 cubePosition;
BMath::Vec3 cubeRotation;
BMath::Mat4 cubeTransform;
BMath::Mat4 planeTransform;
BMath::Vec4 viewVec;
void Game3D::Initialize()
{
    cubePosition = {0,0,-2};

    cubeGeometry = m_Renderer3D->GetGeometryManager()->CreateCube("cube", 8.0f);
    cubeGeometry->Translate(cubePosition);
    cubeGeometry->Scale({4,1,1});
    m_Renderer3D->GetShaderManager()->LoadShader("phongShader", "assets/shaders/PhongShader.glsl");
    material = m_Renderer3D->GetMaterialManager()->CreateMaterial("PhongMaterial", "phongShader");
    cubeGeometry->SetMaterial(material);

}
void Game3D::Update(f32 deltaTime)
{

}
void Game3D::Render3D()
{
    m_Renderer3D->Submit(cubeGeometry, material);
}
void Game3D::RenderUI()
{

}

