#pragma  once 
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/System.h"
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
        BIT_LOG_DEBUG("OnEntityAdded Is Called");
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
                Circle2DComponent& circleCollider = m_EntityManager->GetComponent<Circle2DComponent>(entity);
                shapeIndex = BPhysics2D::BCreateCircleShape(circleCollider.Radius);
            }
            rigidBody.BodyIndex = BPhysics2D::CreateBody(shapeIndex, transform.Position, rigidBody.Type == Rigid2DBodyComponent::BodyType::Static ? 0.0f : rigidBody.Mass);
            BIT_LOG_DEBUG("Entity with id : added and body index is %d", entity.GetID(), rigidBody.BodyIndex);
        }
    }

    void Update(float deltaTime)
    {
        for(const Entity& entity : m_Entities)
        {
            Rigid2DBodyComponent& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
            TransformComponent& transform = m_EntityManager->GetComponent<TransformComponent>(entity);

            BPhysics2D::BBody& body = BPhysics2D::GetBody(rigidBody.BodyIndex);
            BPhysics2D::EnableWeight(body, 9.8f);
            BPhysics2D::LinearIntegrate(body, deltaTime);

            transform.Position = body.Position;
            transform.Rotation = body.Rotation;
        }
    }

};
}
