#pragma once 
#include "Bit/Renderer/Camera.h"
#include <unordered_map>

#define DEFAULT_CAMERA_NAME "defaultcamera"

namespace BitEngine
{

class CameraManager
{
public:
    CameraManager();
    ~CameraManager() {};

    Camera* CreateCamera(const std::string& name, CAMERA_TYPE type);
    Camera* GetDefaultCamera();
    void ReleaseCamera(const std::string& cameraName);

private:
    std::unordered_map<std::string, Camera> m_Cameras;
    Camera m_DefaultCamera;
};
}
