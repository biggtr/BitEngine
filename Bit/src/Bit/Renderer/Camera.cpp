#include "Camera.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Scene/Compontents.h"

namespace BitEngine
{

CameraManager::CameraManager()
{

}
CameraManager::~CameraManager()
{
    
}

void CameraManager::SetActiveCamera(CCamera* camera)
{
    m_ActiveCamera = camera;
    CalculateViewMatrix();
}
CCamera* CameraManager::GetActiveCamera() 
{
    return m_ActiveCamera;
}
void CameraManager::CalculateViewMatrix()
{
    if(m_ActiveCamera->ViewDirty)
    {
        m_ActiveCamera->ViewMatrix = BMath::Mat4::Rotate(0.0f, 0.0f, -m_ActiveCamera->Rotation) *
            BMath::Mat4::Translate(-m_ActiveCamera->Position.x,
                    -m_ActiveCamera->Position.y, -m_ActiveCamera->Position.z);
    }
    m_ActiveCamera->ViewDirty = false;
}
void CameraManager::SetPosition(BMath::Vec3& newPosition)
{
    m_ActiveCamera->Position = newPosition;
    m_ActiveCamera->ViewDirty = true;
    CalculateViewMatrix();
}

void CameraManager::SetRotation(f32 newRotation)
{
    m_ActiveCamera->Rotation = newRotation;
    m_ActiveCamera->ViewDirty = true;
    CalculateViewMatrix();
}
}
