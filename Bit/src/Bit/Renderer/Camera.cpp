#include "Camera.h"
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
}
const CCamera& CameraManager::GetActiveCamera() const
{
    return *m_ActiveCamera;
}
void CameraManager::CalculateProjectionMatrix()
{
    m_ActiveCamera->ProjectionMatrix = BMath::Matrix4x4::Ortho(m_ActiveCamera->Left,
            m_ActiveCamera->Right, 
            m_ActiveCamera->Bottom, 
            m_ActiveCamera->Top, 
            m_ActiveCamera->ZNear, 
            m_ActiveCamera->ZFar);
}
void CameraManager::CalculateViewMatrix()
{

}
}
