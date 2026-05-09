#include "3DGame.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Transform.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Material.h"

BitEngine::Material* material;
BitEngine::Geometry* cubeGeometry;
BitEngine::Mesh* customModel;
BMath::Vec3 cubePosition;
BMath::Vec3 cubeRotation;
BMath::Mat4 cubeTransform;
BMath::Mat4 planeTransform;
BMath::Vec4 viewVec;
void Game3D::Initialize()
{
    cubePosition = {0,0,0};

    cubeGeometry = m_Renderer3D->GetGeometryManager()->CreateCube("cube", 8.0f);
    cubeGeometry->Translate(cubePosition);
    cubeGeometry->Scale({4,1,1});
    m_Renderer3D->GetShaderManager()->LoadShader("phongShader", "assets/shaders/PhongShader.glsl");
    material = m_Renderer3D->GetMaterialManager()->CreateMaterial("PhongMaterial", "phongShader");
    cubeGeometry->SetMaterial(material);

    customModel = m_Renderer3D->GetGeometryManager()->LoadFromFile("CustomModel", "assets/models/guitar/scene.gltf");
    BMath::Transform t = BMath::TransformCreate(cubePosition, {20,20,20}, {BMath::DegToRad(180),0,0});
    customModel->SetTransform(t);


}
void Game3D::Update(f32 deltaTime)
{

}
void Game3D::Render3D()
{
    m_Renderer3D->Submit(customModel);
    // m_Renderer3D->Submit(cubeGeometry, material);
}
void Game3D::RenderUI()
{

}

