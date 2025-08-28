#pragma once
#include "Bit/Math/Vector.h"
namespace BitEngine
{

class Camera2DComponent;
class CameraManager
{
private:
    Camera2DComponent* m_ActiveCamera;

public:
    CameraManager();
    ~CameraManager();

    void SetActiveCamera(Camera2DComponent* camera);
    Camera2DComponent* GetActiveCamera();
    

    void CalculateViewMatrix();
    void SetViewMatrix();

    void SetPosition(BMath::Vec3& position);
    void SetRotation(f32 rotation);
};
}
