#include "3DGame.h"
#include "Bit/Core/Logger.h"

BitEngine::Geometry* cubeGeometry;
BMath::Vec3 cubePosition;
BMath::Vec3 cubeRotation;
BMath::Mat4 cubeTransform;
void Game3D::Initialize()
{
    cubePosition = {0,0,-10};

    BIT_LOG_DEBUG("Creating cube from 3dgame...");
    cubeGeometry = Renderer3D().GetGeometryManager()->CreateCube("cube");
    Renderer3D().GetShaderManager()->LoadShader("PhongShader.glsl", "assets/shaders/PhongShader.glsl");
    BitEngine::Material* cubeMaterial = Renderer3D().GetMaterialManager()->CreateMaterial("cubeMaterial", "PhongShader.glsl");
    cubeGeometry->SetMaterial(cubeMaterial);
}
void Game3D::Update(f32 deltaTime)
{
    cubeRotation.x *= deltaTime;
    cubeTransform = BMath::Mat4CreateTransform(cubePosition, {1,1,1}, cubeRotation);
}
void Game3D::Render()
{
    Renderer3D().Submit(cubeGeometry, cubeTransform);
}
void Game3D::UIRender()
{

}

