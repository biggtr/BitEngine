#include "CameraManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Camera.h"
#include <cstring>


namespace BitEngine
{
CameraManager::CameraManager()
    : m_DefaultCamera(DEFAULT_CAMERA_NAME)
{
}

Camera* CameraManager::CreateCamera(const std::string& name, CAMERA_TYPE type)
{
    if(name == DEFAULT_CAMERA_NAME) 
    {
        return &m_DefaultCamera;
    }
    if(m_Cameras.contains(name))
    {
        BIT_LOG_ERROR("This camera is already created");
        return nullptr;
    }
    Camera newCamera(name);
    newCamera.SetType(type);
    m_Cameras.insert_or_assign(name, newCamera);
    return &m_Cameras.find(name)->second;
}
Camera* CameraManager::GetDefaultCamera()
{
    return &m_DefaultCamera;
}
void CameraManager::ReleaseCamera(const std::string& cameraName)
{
    if(!m_Cameras.contains(cameraName))
    {
        BIT_LOG_ERROR("We Dont Have this camera already");
        return;
    }
    m_Cameras.erase(cameraName);
}
}
