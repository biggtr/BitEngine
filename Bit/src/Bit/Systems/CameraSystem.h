#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Camera.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
namespace BitEngine
{

class CameraSystem: public System
{
private: 
    Camera2DManager* m_CameraManager;

public:
    CameraSystem(Camera2DManager* cameraManager)
    {
        m_CameraManager = cameraManager;
        RequireComponent<Camera2DComponent>();
    }
    SYSTEM_CLASS_TYPE(CAMERA);

public:
    
    void Update(float deltaTime)
    {

        (void)deltaTime;
        for(const Entity& entity : m_Entities)
        {
            Camera2DComponent& camera = m_EntityManager->GetComponent<Camera2DComponent>(entity);
            m_CameraManager->SetActiveCamera(&camera);

        }
    }

};

}




