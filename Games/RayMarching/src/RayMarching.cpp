#include "RayMarching.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Geometry.h"

BitEngine::Geometry* shape;
BitEngine::Material* raymarchingMaterial;
BMath::Vec3 cameraPos = {0,2,-6};
BMath::Vec2 playerInput = BMath::Vec2One();
f32 maxSpeed = 10.0f;
f32 maxAcceleration = 25.0f;
BMath::Vec3 velocity = BMath::Vec3Zero();

float GetHorizontalMovement()
{
    float value = 0.0;

    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D)) value += 1.0f;
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_A)) value -= 1.0f;
    return value;
}
float GetVerticalMovement()
{
    float value = 0.0;

    if(BitEngine::InputIsKeyDown(BitEngine::KEY_W)) value += 1.0f;
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_S)) value -= 1.0f;
    return value;
}
float GetUpDownMovement()
{
    float value = 0.0;

    if(BitEngine::InputIsKeyDown(BitEngine::KEY_E)) value += 1.0f;
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_Q)) value -= 1.0f;
    return value;
}
void RayMarching::Initialize()
{
    ActiveWorldCamera->SetType(BitEngine::CAMERA_TYPE::PRESPECTIVE);
    shape = m_Renderer3D->GetGeometryManager()->CreateQuad("custom quad", 2.0f);
    m_Renderer3D->GetShaderManager()->LoadShader("raymarchingShader", "assets/shaders/raymarching.glsl");
    raymarchingMaterial = m_Renderer3D->GetMaterialManager()->CreateMaterial("raymarchingMaterial", "raymarchingShader");
    raymarchingMaterial->SetVec2("screenRes", {(f32)m_AppConfig.width, (f32)m_AppConfig.height});;
    shape->SetMaterial(raymarchingMaterial);
    
    ActiveWorldCamera->SetPosition(cameraPos);
}
void RayMarching::Update(f32 deltaTime)
{
    playerInput.x = GetHorizontalMovement();
    playerInput.y = GetVerticalMovement();
    playerInput = Vec2ClampMagnitude(playerInput, 1.0f); 
    playerInput = BMath::Vec2ClampMagnitude(playerInput, 1.0f);

    f32 maxSpeedChange = maxAcceleration * deltaTime;
    BMath::Vec3 desiredVelocity = BMath::Vec3(playerInput.x, GetUpDownMovement(), playerInput.y) * maxSpeed;
    velocity.x = BMath::MoveTowards(velocity.x, desiredVelocity.x, maxSpeedChange);
    velocity.z = BMath::MoveTowards(velocity.z, desiredVelocity.z, maxSpeedChange);
    velocity.y = BMath::MoveTowards(velocity.y, desiredVelocity.y, maxSpeedChange);

    BMath::Vec3 displacement = velocity * deltaTime;
    cameraPos += displacement;
    ActiveWorldCamera->SetPosition(cameraPos);
    static f32 accumelatedTime= 0.0f;
    accumelatedTime += deltaTime;
    f32 vpW = (f32)m_Renderer3D->GetViewportWidth();
    f32 vpH = (f32)m_Renderer3D->GetViewportHeight();
    raymarchingMaterial->SetFloat("screenx", vpW);
    raymarchingMaterial->SetFloat("screeny", vpH);
    raymarchingMaterial->SetFloat("time", accumelatedTime);
    raymarchingMaterial->SetVec3("cameraPos", ActiveWorldCamera->GetPosition());

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

