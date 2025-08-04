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
            const CSprite& spriteComponent = m_EntityManager->GetComponent<CSprite>(entity);
            renderer.DrawSprite({11.2f, 0.2f, 0.0f}, {11.2f, 11.2f, 0.0f},
                        spriteComponent.m_Texture);
        }
    }


    
    

};

}




