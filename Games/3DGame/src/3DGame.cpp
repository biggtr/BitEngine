#include "3DGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Material.h"

BitEngine::Material* frenselMaterial;
BitEngine::Geometry* cubeGeometry;
BMath::Vec3 cubePosition;
BMath::Vec3 cubeRotation;
BMath::Mat4 cubeTransform;
BMath::Mat4 planeTransform;
BMath::Vec4 viewVec;
void Game3D::Initialize()
{
    BIT_LOG_DEBUG("3d game init");
    cubePosition = {0,0,-10};

    cubeGeometry = m_Renderer3D->GetGeometryManager()->CreateCube("bluecube", 1.0f, {1.0f, 1.0f, 1.0f, 1.0f});

    
    BitEngine::Shader* frenselShader = m_Renderer3D->GetShaderManager()->LoadShader("frenselShader", "assets/shaders/FrenselOutline.glsl");
    frenselMaterial = m_Renderer3D->GetMaterialManager()->CreateMaterial("frenselMaterial", "frenselShader");

    cubeGeometry->SetMaterial(frenselMaterial);
    
}
void Game3D::Update(f32 deltaTime)
{
    
    cubeRotation.x += 0.7 * deltaTime; 
    cubeRotation.z += 0.7 * deltaTime; 
    cubeTransform = BMath::Mat4CreateTransform(cubePosition, {5,5,5}, cubeRotation);
    frenselMaterial->SetVec3("u_CameraPos", ActiveWorldCamera->GetPosition());
}
void Game3D::Render3D()
{

    m_Renderer3D->Submit(cubeGeometry, frenselMaterial, cubeTransform);
}
void Game3D::RenderUI()
{

}

