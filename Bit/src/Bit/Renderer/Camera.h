#pragma once

#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
namespace BitEngine
{

class CCamera;
class CameraManager
{
private:
    CCamera* m_ActiveCamera;

public:
    CameraManager();
    ~CameraManager();

    void SetActiveCamera(CCamera* camera);
    CCamera* GetActiveCamera();
    
    void CalculateProjectionMatrix();
    void SetProjectionMatrix();

    void CalculateViewMatrix();
    void SetViewMatrix();

    void SetPosition(BMath::vec3& position);
    void SetRotation(BMath::vec3& rotation);
};
}
