#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "System.h"
namespace BitEngine
{

class UIRenderSystem : public System
{

public:
    UIRenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }
    SYSTEM_CLASS_TYPE(UI_RENDER);

public:
    
    void Update(Renderer2D& renderer) 
    {
        for(const Entity& entity : GetEntities())
        {
            const TransformComponent& transformComponent = m_EntityManager->GetComponent<TransformComponent>(entity);
            SpriteComponent& spriteComponent = m_EntityManager->GetComponent<SpriteComponent>(entity);
            if(spriteComponent.IsUI)
            {
                UpdateUVs(spriteComponent);
                renderer.DrawQuad(transformComponent.Position, transformComponent.Scale,transformComponent.Rotation.z ,
                        spriteComponent.STexture, spriteComponent.UVs
                        );
                if(m_EntityManager->HasComponent<BoxCollider2DComponent>(entity))
                {

                    BoxCollider2DComponent& boxColliderComponent = m_EntityManager->GetComponent<BoxCollider2DComponent>(entity);
                    renderer.DrawRect(transformComponent.Position, {boxColliderComponent.Width, boxColliderComponent.Height, 0},
                            {1.0f, 0.0f, 0.0f, 1.0f}
                            );
                }
            }
                
        }
    }

private:
    void UpdateUVs(SpriteComponent& sprite)
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




