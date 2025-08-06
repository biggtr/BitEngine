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
    CalculateProjectionMatrix();
    CalculateViewMatrix();
}
CCamera* CameraManager::GetActiveCamera() 
{
    return m_ActiveCamera;
}
void CameraManager::CalculateProjectionMatrix()
{
    BIT_LOG_DEBUG("=== CAMERAMANAGER ORTHO CALLED ===");
    BIT_LOG_DEBUG("Camera values: Left=%.2f, Right=%.2f, Bottom=%.2f, Top=%.2f, Near=%.4f, Far=%.2f",
                  m_ActiveCamera->Left, m_ActiveCamera->Right, m_ActiveCamera->Bottom, 
                  m_ActiveCamera->Top, m_ActiveCamera->ZNear, m_ActiveCamera->ZFar);
    
    m_ActiveCamera->ProjectionMatrix = BMath::Matrix4x4::Ortho(m_ActiveCamera->Left,
            m_ActiveCamera->Right, 
            m_ActiveCamera->Bottom, 
            m_ActiveCamera->Top, 
            m_ActiveCamera->ZNear, 
            m_ActiveCamera->ZFar);
            
    BIT_LOG_DEBUG("CameraManager matrix: [0]=%.6f, [5]=%.6f, [10]=%.6f, [14]=%.6f",
                  m_ActiveCamera->ProjectionMatrix.Data[0], 
                  m_ActiveCamera->ProjectionMatrix.Data[5],
                  m_ActiveCamera->ProjectionMatrix.Data[10], 
                  m_ActiveCamera->ProjectionMatrix.Data[14]);
}void CameraManager::CalculateViewMatrix()
{
    m_ActiveCamera->ViewMatrix = BMath::Matrix4x4::Rotate(-m_ActiveCamera->Rotation.z) *
        BMath::Matrix4x4::Translate(-m_ActiveCamera->Position.x,
                -m_ActiveCamera->Position.y, -m_ActiveCamera->Position.z);
}
void CameraManager::SetPosition(BMath::vec3& newPosition)
{
    m_ActiveCamera->Position = newPosition;
    CalculateViewMatrix();
}

void CameraManager::SetRotation(BMath::vec3& newRotation)
{
    m_ActiveCamera->Rotation = newRotation;
    CalculateViewMatrix();
}
}
