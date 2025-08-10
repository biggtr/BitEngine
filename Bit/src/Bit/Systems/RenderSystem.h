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
        RequireComponent<CSprite>();
    }
    SYSTEM_CLASS_TYPE(RENDER);

public:
    
    void Update(Renderer2D& renderer) 
    {
        for(const Entity& entity : GetEntities())
        {
            const CTransform& transformComponent = m_EntityManager->GetComponent<CTransform>(entity);
            const CSprite& spriteComponent = m_EntityManager->GetComponent<CSprite>(entity);
            renderer.DrawQuad(transformComponent.Position,
                    transformComponent.Scale,
                    spriteComponent.m_Texture
                    );
        }
    }


    
    

};

}




