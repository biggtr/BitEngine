#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
namespace BitEngine
{


enum class CAMERA_TYPE 
{
    ORTHO,
    PRESPECTIVE,
    NONE
};
class Camera
{
private:
    std::string m_Name;
    CAMERA_TYPE m_Type;
    BMath::Mat4 m_ViewMatrix;
    BMath::Vec3 m_Position;
    BMath::Vec3 m_EulerRotation;
    b8 IsDirty;

public:
    Camera(const std::string& name);
    ~Camera() = default;
    void Reset();
    BMath::Mat4 GetViewMatrix();

    void SetType(CAMERA_TYPE type);
    CAMERA_TYPE GetType() const;

    void SetPosition(const BMath::Vec3& position);
    BMath::Vec3 GetPosition();

    void SetRotation(const BMath::Vec3& rotation);
    BMath::Vec3 GetRotation();

    void MoveForward(f32 amount);
    void MoveBackward(f32 amount);
    void MoveLeft(f32 amount);
    void MoveRight(f32 amount);
    void MoveUp(f32 amount);
    void MoveDown(f32 amount);

    void AdjustYaw(f32 amount);
    void AdjustPitch(f32 amount);
};
}


