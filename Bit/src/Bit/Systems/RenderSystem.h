#pragma once
#include "Bit/Core/Core.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Renderer/Renderer2D.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
#include <vector>
#include <algorithm>
#include <strings.h>
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
        std::vector<Entity> RenderableEntities;
        std::vector<Entity> CircleEntities;
        for(const Entity& entity : GetEntities())
        {
            if(m_EntityManager->HasComponent<CSprite>(entity)) 
            {
                RenderableEntities.push_back(entity);
            }
            if(m_EntityManager->HasComponent<CCircle>(entity))
            {
                CircleEntities.push_back(entity);
            }
        }
        std::sort(RenderableEntities.begin(), RenderableEntities.end(),
                [this](const Entity& a, const Entity& b){
                    CTransform& transformA = m_EntityManager->GetComponent<CTransform>(a);
                    CTransform& transformB = m_EntityManager->GetComponent<CTransform>(b);
                   float aZ = transformA.Position.z;
                   float bZ = transformB.Position.z;
                   return aZ < bZ;
                });
        std::sort(CircleEntities.begin(), CircleEntities.end(),
                [this](const Entity& a, const Entity& b){
                    CTransform& transformA = m_EntityManager->GetComponent<CTransform>(a);
                    CTransform& transformB = m_EntityManager->GetComponent<CTransform>(b);
                   float aZ = transformA.Position.z;
                   float bZ = transformB.Position.z;
                   return aZ < bZ;
                });
        for(const Entity& entity : CircleEntities)
        {
            CTransform& transformComponent = m_EntityManager->GetComponent<CTransform>(entity);
            if(m_EntityManager->HasComponent<CCircle>(entity))
            {
                CCircle& circleComponent = m_EntityManager->GetComponent<CCircle>(entity);
                BMath::Mat4 transform = BMath::Mat4::CreateTransform(transformComponent.Position, transformComponent.Scale);
                renderer.DrawCircle(transform, circleComponent.Color, circleComponent.Thickness, circleComponent.Fade);
            }
        }
        for(const Entity& entity : RenderableEntities)
        {
            CTransform& transformComponent = m_EntityManager->GetComponent<CTransform>(entity);
            if(m_EntityManager->HasComponent<CSprite>(entity))
            {
                CSprite& spriteComponent = m_EntityManager->GetComponent<CSprite>(entity);
                UpdateUVs(spriteComponent);
                renderer.DrawQuad(transformComponent.Position, transformComponent.Scale,
                        spriteComponent
                        );
            }
            if(m_EntityManager->HasComponent<CBoxCollider>(entity))
            {

                CBoxCollider& boxColliderComponent = m_EntityManager->GetComponent<CBoxCollider>(entity);
                renderer.DrawRect(transformComponent.Position, boxColliderComponent.Size,
                        {1.0f, 0.0f, 0.0f, 1.0f}
                        );
            }
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




