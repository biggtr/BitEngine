#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
namespace BitEngine
{

class RenderSystem : public System
{

public:
    RenderSystem()
    {
        RequireComponent<CTransform>();
    }
    SYSTEM_CLASS_TYPE(RENDER);

public:
    
    void Update(Renderer2D& renderer) 
    {
        for(const Entity& entity : GetEntities())
        {
            const CTransform& transformComponent = m_EntityManager->GetComponent<CTransform>(entity);
            renderer.DrawQuad(transformComponent.Position, 
                    transformComponent.Scale,
                    {1.0f, 0.0f, 0.0f, 1.0f});
        }
    }


    
    

};

}




