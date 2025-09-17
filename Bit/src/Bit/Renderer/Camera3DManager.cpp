#include "Camera3DManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Camera.h"
#include <cstring>


namespace BitEngine
{
Camera3DManager::Camera3DManager()
{
    m_DefaultCamera = Camera3DCreate();
}

Camera3D* Camera3DManager::CreateCamera(const char* cameraName)
{
    if(strcmp(cameraName, DEFAULT_CAMERA_NAME) == 0) // 0 -> equal
    {
        return &m_DefaultCamera;
    }
    if(m_Cameras.contains(cameraName))
    {
        BIT_LOG_ERROR("This camera is already created");
        return nullptr;
    }
    m_Cameras[cameraName] = Camera3DCreate();
    return &m_Cameras[cameraName];
}
Camera3D* Camera3DManager::GetDefaultCamera()
{
    return &m_DefaultCamera;
}
void Camera3DManager::ReleaseCamera(const char* cameraName)
{
    if(!m_Cameras.contains(cameraName))
    {
        BIT_LOG_ERROR("We Dont Have this camera already");
        return;
    }
    m_Cameras.erase(cameraName);
}
}
