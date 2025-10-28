#pragma once
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/Renderer/Renderer2D.h"
#include "System.h"
#include <vector>
#include <algorithm>
#include <string>
namespace BitEngine
{

class RenderSystem : public System
{

public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
    }
    SYSTEM_CLASS_TYPE(RENDER);

public:
    
    void Update(Renderer2D& renderer) 
    {
        std::vector<Entity> RenderableEntities;
        for(const Entity& entity : GetEntities())
        {
            if(m_EntityManager->HasComponent<SpriteComponent>(entity) || 
                m_EntityManager->HasComponent<Circle2DComponent>(entity) ||
                m_EntityManager->HasComponent<Box2DColliderComponent>(entity)) 
            {
                RenderableEntities.push_back(entity);
            }
        }
        std::sort(RenderableEntities.begin(), RenderableEntities.end(),
                [this](const Entity& a, const Entity& b){
                    TransformComponent& transformA = m_EntityManager->GetComponent<TransformComponent>(a);
                    TransformComponent& transformB = m_EntityManager->GetComponent<TransformComponent>(b);
                   float aZ = transformA.Position.z;
                   float bZ = transformB.Position.z;
                   return aZ < bZ;
                });
        for(const Entity& entity : RenderableEntities)
        {
            TransformComponent& transformComponent = m_EntityManager->GetComponent<TransformComponent>(entity);
            if(m_EntityManager->HasComponent<SpriteComponent>(entity))
            {
                SpriteComponent& spriteComponent = m_EntityManager->GetComponent<SpriteComponent>(entity);
                if(spriteComponent.STexture)
                {
                    UpdateUVs(spriteComponent);

                    renderer.DrawQuad(transformComponent.Position, transformComponent.Scale,
                            spriteComponent
                            );
                }
                else
                {
                    renderer.DrawQuad(transformComponent.Position, transformComponent.Scale,
                            spriteComponent.Color
                            );
                }
            }
            if(m_EntityManager->HasComponent<Circle2DComponent>(entity))
            {
                Circle2DComponent& circleComponent = m_EntityManager->GetComponent<Circle2DComponent>(entity);
                BMath::Mat4 transform = BMath::Mat4CreateTransform(transformComponent.Position,  BMath::Vec3(2.0f, 2.0f, 0.0f) * circleComponent.Radius);
                renderer.DrawCircle(transform, circleComponent.Color, circleComponent.Thickness, circleComponent.Fade);
            }
            if(m_EntityManager->HasComponent<Box2DColliderComponent>(entity))
            {

                Box2DColliderComponent& boxColliderComponent= m_EntityManager->GetComponent<Box2DColliderComponent>(entity);
                // BIT_LOG_DEBUG("boxCollider.size.x : %.2f, size.y : %.2f", boxColliderComponent.Size.x, boxColliderComponent.Size.y);
                // BIT_LOG_DEBUG("transformComponent.Position.x : %.2f, pos.y : %.2f", transformComponent.Position.x, transformComponent.Position.y);
                renderer.DrawRect(transformComponent.Position, boxColliderComponent.Size,
                        {1.0f, 0.0f, 0.0f, 1.0f}
                        );
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




