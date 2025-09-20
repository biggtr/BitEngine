#include "Camera.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Scene/Compontents.h"
#include <algorithm>

namespace BitEngine
{

Camera::Camera(const std::string& name)
    : m_Name(name)
{
    Reset();
}
void Camera::Reset()
{
    m_Type = CAMERA_TYPE::NONE;
    m_Position = BMath::Vec3Zero();
    m_EulerRotation = BMath::Vec3Zero();
    m_ViewMatrix = {};
    IsDirty = false;
}
void Camera::SetType(CAMERA_TYPE type)
{
    m_Type = type;
}
CAMERA_TYPE Camera::GetType() const 
{
    return m_Type;
}
BMath::Mat4 Camera::GetViewMatrix()
{
    if(IsDirty)
    {
        BMath::Mat4 rotation = BMath::Mat4::Rotate(m_EulerRotation.x, m_EulerRotation.y, m_EulerRotation.z);
        BMath::Mat4 translation = BMath::Mat4::Translate(m_Position.x, m_Position.y, m_Position.z);
        BMath::Mat4 viewmatrix = rotation * translation;
        m_ViewMatrix = BMath::Mat4::Inverse(viewmatrix);
        IsDirty = false;
    }
    return m_ViewMatrix;
}

void Camera::SetPosition(const BMath::Vec3& position)
{
    m_Position = position;
    IsDirty = true;
}
BMath::Vec3 Camera::GetPosition()
{
    return m_Position;
}

void Camera::SetRotation(const BMath::Vec3& rotation)
{
    m_EulerRotation = rotation;
    IsDirty = true;
}
BMath::Vec3 Camera::GetRotation()
{
    return m_EulerRotation;
}

void Camera::MoveForward(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Forward(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}
void Camera::MoveBackward(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Backward(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}
void Camera::MoveLeft(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Left(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}
void Camera::MoveRight(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Right(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}
void Camera::MoveUp(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Right(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}
void Camera::MoveDown(f32 amount)
{
    BMath::Vec3 direction = BMath::Mat4::Down(m_ViewMatrix);
    direction = direction * amount;
    m_Position = m_Position + direction;
    IsDirty = true;
}

void Camera::AdjustYaw(f32 amount)
{
    m_EulerRotation.y += amount;
    IsDirty = true;
}
void Camera::AdjustPitch(f32 amount)
{
    m_EulerRotation.x += amount;
    static const f32 limit = DegToRad(89);
    m_EulerRotation.x = std::clamp(m_EulerRotation.x, -limit, limit);
    IsDirty = true;
}
}
