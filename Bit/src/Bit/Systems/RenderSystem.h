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
            CSprite& spriteComponent = m_EntityManager->GetComponent<CSprite>(entity);
            UpdateUVs(spriteComponent);
            renderer.DrawQuad(transformComponent.Position, transformComponent.Scale,
                    spriteComponent
                    );
        }
    }

private:
    void UpdateUVs(CSprite& sprite)
    {

        const uint8_t columns = sprite.Width / sprite.FrameWidth;
        const uint8_t rows = sprite.Height / sprite.FrameHeight;

        const uint8_t x = sprite.CurrentFrame % columns;
        const uint8_t y = sprite.CurrentFrame / columns;

        float u1,v1,u2,v2;

        u1 = (float)x / columns; 
        v1 = (float)y / rows;
        u2 = u1 + (1.0f / columns);
        v2 = v1 + (1.0f / rows);
                
        sprite.UVs[0] = u1; sprite.UVs[1] = v1;
        sprite.UVs[2] = u2; sprite.UVs[3] = v1;
        sprite.UVs[4] = u2; sprite.UVs[5] = v2;
        sprite.UVs[6] = u1; sprite.UVs[7] = v2;
    }
    

};

}




