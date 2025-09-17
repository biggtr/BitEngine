#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
namespace BitEngine
{

class Camera2DComponent;
class Camera2DManager
{
private:
    Camera2DComponent* m_ActiveCamera;

public:
    Camera2DManager();
    ~Camera2DManager();

    void SetActiveCamera(Camera2DComponent* camera);
    Camera2DComponent* GetActiveCamera();
    

    void CalculateViewMatrix();
    void SetViewMatrix();

    void SetPosition(BMath::Vec3& position);
    void SetRotation(f32 rotation);
};

struct Camera3D
{
    BMath::Mat4 ViewMatrix;
    BMath::Vec3 Position;
    BMath::Vec3 EulerRotation;
    b8 IsDirty;
};
Camera3D Camera3DCreate();
void Camera3DReset(Camera3D* c);
BMath::Mat4 Camera3DGetViewMatrix(Camera3D* c);

void Camera3DSetPosition(Camera3D* c, const BMath::Vec3& position);
BMath::Vec3 Camera3DGetPosition(Camera3D* c);

void Camera3DSetRotation(Camera3D* c, const BMath::Vec3& rotation);
BMath::Vec3 Camera3DGetRotation(Camera3D* c);

void Camera3DMoveForward(Camera3D* c, f32 amount);
void Camera3DMoveBackward(Camera3D* c, f32 amount);
void Camera3DMoveLeft(Camera3D* c, f32 amount);
void Camera3DMoveRight(Camera3D* c, f32 amount);
void Camera3DMoveUp(Camera3D* c, f32 amount);
void Camera3DMoveDown(Camera3D* c, f32 amount);

void Camera3DAdjustYaw(Camera3D* c, f32 amount);
void Camera3DAdjustPitch(Camera3D* c, f32 amount);
}
