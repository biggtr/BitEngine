#include "3DGame.h"
#include "Bit/Core/Logger.h"

BitEngine::Geometry* cubeGeometry;
BitEngine::Geometry* planeGeometry;
BMath::Vec3 cubePosition;
BMath::Vec3 planePosition;
BMath::Vec3 cubeRotation;
BMath::Mat4 cubeTransform;
BMath::Mat4 planeTransform;
void Game3D::Initialize()
{
    cubePosition = {0,0,-10};
    planePosition = {5,0,-10};

    cubeGeometry = m_Renderer3D->GetGeometryManager()->GetGeometry("cube");
    planeGeometry= m_Renderer3D->GetGeometryManager()->CreatePlane("plane");
    BitEngine::Material* material = m_Renderer3D->GetMaterialManager()->CreateMaterial("cubeMaterial", "PhongShader.glsl");
    m_AssetManager->AddTexture("texture", "assets/textures/icon_chest.png");
    material->SetTexture("u_texture", m_AssetManager->GetTexture("texture"));
    cubeGeometry->SetMaterial(material);
    planeGeometry->SetMaterial(material);
}
void Game3D::Update(f32 deltaTime)
{
    cubeRotation.x += 3.0f * deltaTime;
    cubeRotation.y += 3.0f * deltaTime;
    cubeTransform = BMath::Mat4CreateTransform(cubePosition, {2,2,2}, cubeRotation);
    planeTransform = BMath::Mat4CreateTransform(planePosition, {2,2,2}, cubeRotation);
}
void Game3D::Render()
{
    m_Renderer3D->Submit(cubeGeometry, cubeTransform);
    m_Renderer3D->Submit(planeGeometry, planeTransform);
}
void Game3D::UIRender()
{

}

