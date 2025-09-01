#pragma  once 
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BCollision.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/System.h"

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
                shapeIndex = BPhysics2D::BCreateBoxShape(boxCollider.Size.x, boxCollider.Size.y);
            }
            else if(m_EntityManager->HasComponent<Circle2DColliderComponent>(entity))
            {
                Circle2DColliderComponent& circleCollider = m_EntityManager->GetComponent<Circle2DColliderComponent>(entity);
                shapeIndex = BPhysics2D::BCreateCircleShape(circleCollider.Radius);
            }
            rigidBody.BodyIndex = BPhysics2D::CreateBody(shapeIndex, transform.Position, rigidBody.Mass);
            // BIT_LOG_DEBUG("Entity with id : added and body index is %d", entity.GetID(), rigidBody.BodyIndex);
        }
    }

    void Update(float deltaTime)
    {
        for(u32 i = 0; i < m_Entities.size();  ++i)
        {
            const Entity& entityA = m_Entities[i];
            Rigid2DBodyComponent& rigidBodyA = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entityA);
            TransformComponent& transformA = m_EntityManager->GetComponent<TransformComponent>(entityA);

            BPhysics2D::BBody& bodyA = BPhysics2D::GetBody(rigidBodyA.BodyIndex);
            // BPhysics2D::EnableWeight(bodyA, -9.8f * METER_PER_PIXEL);
            BPhysics2D::LinearIntegrate(bodyA, deltaTime);

            transformA.Position = bodyA.Position;
            transformA.Rotation = bodyA.Rotation;
        }
    }

    void ApplyImpulse(const Entity& entity, const BMath::Vec3& impulse)
    {
        if(!m_EntityManager->HasComponent<Rigid2DBodyComponent>(entity))
            return;
        auto& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
        
        BPhysics2D::BBody& body = BPhysics2D::GetBody(rigidBody.BodyIndex);
        BPhysics2D::ApplyImpulse(body, impulse);
    }

};
}
