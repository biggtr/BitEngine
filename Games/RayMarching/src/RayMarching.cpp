#include "RayMarching.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Renderer/Geometry.h"

BitEngine::Geometry* shape;
BitEngine::Material* raymarchingMaterial;
BMath::Vec3 cameraPos = {0,0,3};
BMath::Vec3 cameraForward = BMath::Vec3Zero();
BMath::Vec3 cameraRight = BMath::Vec3Zero();
BMath::Vec3 cameraUp = BMath::Vec3Zero();
BMath::Vec2 playerInput = BMath::Vec2One();
f32 maxSpeed = 20.0f;
f32 maxAcceleration = 25.0f;
BMath::Vec3 velocity = BMath::Vec3Zero();
static f32 yaw = -90;
static f32 pitch = 0;
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

    i32 mouseX, mouseY;
    static f32 lastMouseX = m_AppConfig.width / 2.0f;
    static f32 lastMouseY = m_AppConfig.height / 2.0f; 
    BitEngine::InputGetMousePosition(&mouseX, &mouseY);

    f32 mouseXOffset = lastMouseX - mouseX;
    f32 mouseYOffset = lastMouseY - mouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    const f32 sensitivity = 0.1f;
    mouseXOffset *= sensitivity;
    mouseYOffset *= sensitivity;
    yaw += mouseXOffset;
    pitch += mouseYOffset;
    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;
    cameraForward.x = BMath::Cos(yaw) * BMath::Cos(pitch);
    cameraForward.y = BMath::Sin(pitch);
    cameraForward.z = BMath::Sin(yaw) * BMath::Cos(pitch);
    BMath::Vec3 worldUp = {0.0f, 1.0f, 0.0f};
    cameraRight = BMath::Vec3Normalize(BMath::Vec3Cross(worldUp, cameraForward));
    cameraUp = BMath::Vec3Normalize(BMath::Vec3Cross(cameraForward, cameraRight));

    f32 maxSpeedChange = maxAcceleration * deltaTime;
    BMath::Vec3 desiredVelocity = ((cameraRight * playerInput.x) + (cameraForward * playerInput.y) + (worldUp * GetUpDownMovement()) )* maxSpeed;
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
    raymarchingMaterial->SetVec3("cameraForward", cameraForward);
    raymarchingMaterial->SetVec3("cameraRight", cameraRight);
    raymarchingMaterial->SetVec3("cameraUp", cameraUp);

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

