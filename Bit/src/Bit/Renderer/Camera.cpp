#include "Camera.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Scene/Compontents.h"
#include <algorithm>

namespace BitEngine
{

Camera2DManager::Camera2DManager()
{

}
Camera2DManager::~Camera2DManager()
{
    
}

void Camera2DManager::SetActiveCamera(Camera2DComponent* camera)
{
    m_ActiveCamera = camera;
    CalculateViewMatrix();
}
Camera2DComponent* Camera2DManager::GetActiveCamera() 
{
    return m_ActiveCamera;
}
void Camera2DManager::CalculateViewMatrix()
{
    if(m_ActiveCamera->ViewDirty)
    {
        m_ActiveCamera->ViewMatrix = BMath::Mat4::Rotate(0.0f, 0.0f, -m_ActiveCamera->Rotation) *
            BMath::Mat4::Translate(-m_ActiveCamera->Position.x,
                    -m_ActiveCamera->Position.y, -m_ActiveCamera->Position.z);
    }
    m_ActiveCamera->ViewDirty = false;
}
void Camera2DManager::SetPosition(BMath::Vec3& newPosition)
{
    m_ActiveCamera->Position = newPosition;
    m_ActiveCamera->ViewDirty = true;
    CalculateViewMatrix();
}

void Camera2DManager::SetRotation(f32 newRotation)
{
    m_ActiveCamera->Rotation = newRotation;
    m_ActiveCamera->ViewDirty = true;
    CalculateViewMatrix();
}

////////////////////////////////////////////////////////////////////////////////
/// Camera3D
////////////////////////////////////////////////////////////////////////////////

Camera3D Camera3DCreate()
{
    Camera3D c;
    Camera3DReset(&c);

    return c;
}
void Camera3DReset(Camera3D *c)
{
    c->Position = BMath::Vec3Zero();
    c->EulerRotation = BMath::Vec3Zero();
    c->ViewMatrix = {};
    c->IsDirty = false;
}
BMath::Mat4 Camera3DGetViewMatrix(Camera3D* c)
{
    if(c->IsDirty)
    {
        BMath::Mat4 rotation = BMath::Mat4::Rotate(c->EulerRotation.x, c->EulerRotation.y, c->EulerRotation.z);
        BMath::Mat4 translation = BMath::Mat4::Translate(c->Position.x, c->Position.y, c->Position.z);
        BMath::Mat4 viewmatrix = rotation * translation;
        c->ViewMatrix = BMath::Mat4::Inverse(viewmatrix);
        c->IsDirty = false;
    }
    return c->ViewMatrix;
}

void Camera3DSetPosition(Camera3D* c, const BMath::Vec3& position)
{
    c->Position = position;
    c->IsDirty = true;
}
BMath::Vec3 Camera3DGetPosition(Camera3D* c)
{
    return c->Position;
}

void Camera3DSetRotation(Camera3D* c, const BMath::Vec3& rotation)
{
    c->EulerRotation = rotation;
    c->IsDirty = true;
}
BMath::Vec3 Camera3DGetRotation(Camera3D* c)
{
    return c->Position;
}

void Camera3DMoveForward(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Forward(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}
void Camera3DMoveBackward(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Backward(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}
void Camera3DMoveLeft(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Left(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}
void Camera3DMoveRight(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Right(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}
void Camera3DMoveUp(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Right(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}
void Camera3DMoveDown(Camera3D* c, f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Down(c->ViewMatrix);
    direction = direction * amount;
    c->Position = c->Position + direction;
    c->IsDirty = true;
}

void Camera3DAdjustYaw(Camera3D* c, f32 amount)
{
    c->EulerRotation.y += amount;
    c->IsDirty = true;
}
void Camera3DAdjustPitch(Camera3D* c, f32 amount)
{
    c->EulerRotation.x += amount;
    static const f32 limit = DegToRad(89);
    c->EulerRotation.x = std::clamp(c->EulerRotation.x, -limit, limit);
    c->IsDirty = true;
}
}
