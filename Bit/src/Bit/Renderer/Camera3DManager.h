#pragma once 
#include "Bit/Renderer/Camera.h"
#include <unordered_map>

#define DEFAULT_CAMERA_NAME "defaultcamera"
namespace BitEngine
{

class Camera3DManager
{
public:
    Camera3DManager(){};
    ~Camera3DManager(){};

    Camera3D* CreateCamera(const char* cameraName);
    Camera3D* GetDefaultCamera();
    void ReleaseCamera(const char* cameraName);

private:
    std::unordered_map<const char*, Camera3D> m_Cameras;
    Camera3D m_DefaultCamera;
};
}
