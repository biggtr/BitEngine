#pragma  once 
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BCollision.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "System.h"

#define METER_PER_PIXEL 100
namespace BitEngine 
{

class Physics2DSystem : public System
{
public:
    Physics2DSystem()
    {
        RequireComponent<Rigid2DBodyComponent>();
        RequireComponent<TransformComponent>();
    }
    SYSTEM_CLASS_TYPE(PHYSICS2D);

    void OnEntityAdded(const Entity& entity) override
    {
        // BIT_LOG_DEBUG("OnEntityAdded Is Called");
        if(m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
        {
            Rigid2DBodyComponent& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
            TransformComponent& transform = m_EntityManager->GetComponent<TransformComponent>(entity);
            u32 shapeIndex = 0;

            if(m_EntityManager->HasComponent<Box2DColliderComponent>(entity))
            {
                Box2DColliderComponent& boxCollider = m_EntityManager->GetComponent<Box2DColliderComponent>(entity);
                shapeIndex = BPhysics2DCreateBoxShape(boxCollider.Size.x, boxCollider.Size.y);
                BIT_LOG_DEBUG("boxCollider.size.x : %.2f, size.y : %.2f", boxCollider.Size.x, boxCollider.Size.y);
            }
            else if(m_EntityManager->HasComponent<Circle2DColliderComponent>(entity))
            {
                Circle2DColliderComponent& circleCollider = m_EntityManager->GetComponent<Circle2DColliderComponent>(entity);
                shapeIndex = BPhysics2DCreateCircleShape(circleCollider.Radius);

            }
            rigidBody.BodyIndex = BPhysics2DCreateBody(
                shapeIndex, 
                transform.Position, 
                rigidBody.Mass, 
                rigidBody.Restitution,
                rigidBody.BodyType
            );
            
            BIT_LOG_DEBUG("Entity with id : added and body index is %d", entity.GetID(), rigidBody.BodyIndex);
        }
    }

    void Update(f32 deltaTime)
    {
        for(u32 i = 0; i < m_Entities.size(); ++i)
        {
            const Entity& entity = m_Entities[i];
            Rigid2DBodyComponent& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
            TransformComponent& transform = m_EntityManager->GetComponent<TransformComponent>(entity);

            BBody& body = BPhysics2DGetBody(rigidBody.BodyIndex);
            
            if(body.BodyType == BODY_DYNAMIC)
            {
                // BPhysics2DEnableWeight(body, -9.8f * METER_PER_PIXEL);
                
                BMath::Vec3 friction = BPhysics2DGenerateFrictionForce(body, 300.0f);
                BPhysics2DAddForce(body, friction);
                BPhysics2DLinearIntegrate(body, deltaTime);
            }
            else if(body.BodyType == BODY_KINEMATIC)
            {
                body.Position += body.Velocity * deltaTime;
            }
            
            transform.Position = body.Position;
            transform.Rotation = body.Rotation;
        }
    }
    void ApplyImpulse(const Entity& entity, const BMath::Vec3& impulse)
    {
        if(!m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
            return;
            
        auto& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
        BBody& body = BPhysics2DGetBody(rigidBody.BodyIndex);
        
        if(body.BodyType == BODY_DYNAMIC)
        {
            BPhysics2DApplyImpulse(body, impulse);
        }
    }
    
    void SetKinematicVelocity(const Entity& entity, const BMath::Vec3& velocity)
    {
        if(!m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
            return;
            
        auto& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
        BBody& body = BPhysics2DGetBody(rigidBody.BodyIndex);
        
        if(body.BodyType == BODY_KINEMATIC)
        {
            body.Velocity = velocity;
        }
    }
    
    void SetPosition(const Entity& entity, const BMath::Vec3& position)
    {
        if(!m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
            return;
            
        auto& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
        BBody& body = BPhysics2DGetBody(rigidBody.BodyIndex);
        
        if(body.BodyType != BODY_DYNAMIC)
        {
            body.Position = position;
        }
    }
    
    BMath::Vec3 GetVelocity(const Entity& entity)
    {
        if(!m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
            return BMath::Vec3(0.0f);
            
        auto& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
        BBody& body = BPhysics2DGetBody(rigidBody.BodyIndex);
        return body.Velocity;
    }

};
}
