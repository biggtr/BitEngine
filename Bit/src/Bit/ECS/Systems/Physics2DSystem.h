#pragma once
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Physics/Physics2D.h"
#include "System.h"
#include "Bit/Math/Vector.h"
#include "box2d/collision.h"
#include <random>
#include <unordered_map>
#include <vector>

namespace BitEngine
{
class Physics2DSystem : public System
{
public:
    SYSTEM_CLASS_TYPE(PHYSICS2D)

    Physics2DSystem(Physics2D* physics2D)
    {
        RequireComponent<TransformComponent>();
        RequireComponent<Rigidbody2DComponent>();

        m_Physics2D = physics2D;
    }
    ~Physics2DSystem(){}

    void OnEntityAdded(const Entity& entity) override
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return;

        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);
        b2BodyId bodyID = m_Physics2D->CreateBody(rigidbody.Type, rigidbody.Position);
        
        if(m_EntityManager->HasComponent<BoxCollider2DComponent>(entity))
        {
            auto& boxCollider = m_EntityManager->GetComponent<BoxCollider2DComponent>(entity);
            b2Polygon polygon = m_Physics2D->CreateBoxShape(boxCollider.Width, boxCollider.Height);
            boxCollider.shapeId = m_Physics2D->AddBox(bodyID, polygon, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution);
        }

        else if(m_EntityManager->HasComponent<CircleCollider2DComponent>(entity))
        {
            auto& circleCollider = m_EntityManager->GetComponent<CircleCollider2DComponent>(entity);
            b2Circle circle = m_Physics2D->CreateCircleShape(circleCollider.center, circleCollider.radius);
            circleCollider.shapeId = m_Physics2D->AddCircle(bodyID, circle, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution);
        }

        else if(m_EntityManager->HasComponent<CapsuleCollider2DComponent>(entity))
        {
            auto& capsuleCollider = m_EntityManager->GetComponent<CapsuleCollider2DComponent>(entity);
            b2Capsule capsule = m_Physics2D->CreateCapsuleShape(capsuleCollider.center1, capsuleCollider.center2, capsuleCollider.radius);
            capsuleCollider.shapeId = m_Physics2D->AddCapsule(bodyID, capsule, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution);
        }
        

    }

    void RemoveEntity(const Entity& entity) override
    {

    }
    void Update(f32 deltaTime)
    {
        m_Accumulator += deltaTime;
        f32 fixedTimeStep = m_Physics2D->GetFixedTimeStep();

        while(m_Accumulator >= fixedTimeStep)
        {
            m_Physics2D->Step();
            m_Accumulator -= fixedTimeStep;
        }
        SyncTransforms();
    }

    void SyncTransforms()
    {
        for (auto entity : m_Entities)
        {
            auto& transform = m_EntityManager->GetComponent<TransformComponent>(entity);
            auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);

            BMath::Vec2 pos = m_Physics2D->GetPosition(rigidbody.BodyId);
            float rot = m_Physics2D->GetRotation(rigidbody.BodyId);

            transform.Position = { pos.x, pos.y, transform.Position.z };
            transform.Rotation.z = rot;
        }
    }
    void SetGravity(f32 x, f32 y)
    {

    }
    void SetVelocity(const Entity& entity, const BMath::Vec3& velocity)
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return;
        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);
        m_Physics2D->SetLinearVelocity(rigidbody.BodyId, velocity);
    }
    BMath::Vec2 GetVelocity(const Entity& entity)
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return {};
        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);
        return m_Physics2D->GetLinearVelocity(rigidbody.BodyId);
    }
    b8 IsGrounded(Entity entity)
    {

    }

private:
    Physics2D* m_Physics2D;
    f32 m_Accumulator = 0.0f;
     
};
}
