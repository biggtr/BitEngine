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
    const CCamera& GetActiveCamera() const;
    void CalculateProjectionMatrix();
    void CalculateViewMatrix();
    void SetPosition(BMath::Vector3& position);
    void SetRotation(BMath::Vector3& rotation);
    void GetPosition();
    void GetRotation();
};
}
