#pragma once
#include "Bit/Core/Core.h"
#include "Bit/Core/Input.h"
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
        m_IsWireframeOn = false;
    }
    SYSTEM_CLASS_TYPE(RENDER);

public:
    
    void Update(Renderer2D& renderer) 
    {
        if(InputIsKeyDown(KEY_W) && !InputWasKeyDown(KEY_W))
        {
            m_IsWireframeOn = !m_IsWireframeOn; 
        }
        std::vector<Entity> RenderableEntities;
        for(const Entity& entity : GetEntities())
        {
            if(m_EntityManager->HasComponent<SpriteComponent>(entity) || 
                m_EntityManager->HasComponent<Circle2DComponent>(entity) ||
                m_EntityManager->HasComponent<BoxCollider2DComponent>(entity)) 
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

                    renderer.DrawQuad(transformComponent.Position, transformComponent.Scale, transformComponent.Rotation.z,
                            spriteComponent.STexture, spriteComponent.UVs
                            );
                }
                else
                {
                    renderer.DrawQuad(transformComponent.Position, transformComponent.Scale, transformComponent.Rotation.z,
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
            if(m_IsWireframeOn)
            {
                if(m_EntityManager->HasComponent<BoxCollider2DComponent>(entity))
                {

                    BoxCollider2DComponent& boxColliderComponent= m_EntityManager->GetComponent<BoxCollider2DComponent>(entity);
                    renderer.DrawRect({transformComponent.Position.x + boxColliderComponent.offset.x, transformComponent.Position.y + boxColliderComponent.offset.y, 0.0f}, {boxColliderComponent.Width, boxColliderComponent.Height, 0},
                            0.0f,
                            boxColliderComponent.DebugColor
                            );
                }
                if(m_EntityManager->HasComponent<CircleCollider2DComponent>(entity))
                {
                    CircleCollider2DComponent& circleCollider = m_EntityManager->GetComponent<CircleCollider2DComponent>(entity);
                    BMath::Mat4 transform = BMath::Mat4CreateTransform(transformComponent.Position,  BMath::Vec3(2.0f, 2.0f, 0.0f) * circleCollider.radius);
                    renderer.DrawCircle(transform, {1.0f, 1.0f, 1.0f, 1.0f});
                }

                if(m_EntityManager->HasComponent<CapsuleCollider2DComponent>(entity))
                {
                    CapsuleCollider2DComponent& capsuleCollider = m_EntityManager->GetComponent<CapsuleCollider2DComponent>(entity);
                    renderer.DrawCapsule({capsuleCollider.center1.x + transformComponent.Position.x, capsuleCollider.center1.y + transformComponent.Position.y},
                            {capsuleCollider.center2.x + transformComponent.Position.x, capsuleCollider.center2.y + transformComponent.Position.y},
                            capsuleCollider.radius, capsuleCollider.color);
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
    
    b8 m_IsWireframeOn;

};

}




