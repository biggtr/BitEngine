#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Physics/Physics2D.h"
#include "Bit/Physics/PhysicsTypes.h"
#include "System.h"
#include "Bit/Math/Vector.h"
#include "box2d/box2d.h"
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

        if(!b2Body_IsValid(rigidbody.BodyId))
        {
            rigidbody.BodyId = m_Physics2D->CreateBody(rigidbody.Type, rigidbody.Position);
        }
        BIT_LOG_DEBUG("rigidbody id : %d", rigidbody.BodyId);
    }

    void CreateCircleShape(const Entity& entity, const BMath::Vec2& center, f32 radius, b8 isPrimary = false,
                    PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE)
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return; 

        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);

        if(!b2Body_IsValid(rigidbody.BodyId))
        {
            rigidbody.BodyId = m_Physics2D->CreateBody(rigidbody.Type, rigidbody.Position);
        }

        b2Circle circle = m_Physics2D->CreateCircleShape(center, radius);
        b2ShapeId newShape = m_Physics2D->AddCircle(rigidbody.BodyId, circle, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution);

        rigidbody.ShapeIds.push_back(newShape);
        

        MultiColliderComponent colliderData = {};
        colliderData.Type = PhysicsColliderType::CIRCLE;
        colliderData.CircleCollider2D.center = center;
        colliderData.CircleCollider2D.radius = radius;
        colliderData.CircleCollider2D.shapeId = newShape;
        
        rigidbody.MultiColliderComponents.push_back(colliderData); 

        if(isPrimary || rigidbody.ShapeIds.size() == 1)
        {
            rigidbody.PrimaryId = newShape;
        }
    }
    void CreateBoxShape(const Entity& entity, f32 width, f32 height, const BMath::Vec2& offset = {0.0f, 0.0f}, f32 angle = 0.0f, b8 isPrimary = false,
                PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE)
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return;

        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);

        if(!b2Body_IsValid(rigidbody.BodyId))
        {
            rigidbody.BodyId = m_Physics2D->CreateBody(rigidbody.Type, rigidbody.Position);
        }

        b2Polygon polygon = m_Physics2D->CreateBoxShape(width, height, offset, angle);

        b2ShapeId newShape = m_Physics2D->AddBox(rigidbody.BodyId, polygon, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution, categoryType, categoryToCollideWith); 
        rigidbody.ShapeIds.push_back(newShape);
        

        MultiColliderComponent colliderData = {};
        colliderData.Type = PhysicsColliderType::BOX;  
        colliderData.BoxCollider2D.Width = width;
        colliderData.BoxCollider2D.Height = height;
        colliderData.BoxCollider2D.Rotation = angle;
        colliderData.BoxCollider2D.offset = offset;
        colliderData.BoxCollider2D.shapeId = newShape;
    
    rigidbody.MultiColliderComponents.push_back(colliderData);

        if(isPrimary || rigidbody.ShapeIds.size() == 1)
        {
            rigidbody.PrimaryId = newShape;
        }
    }

    void CreateCapsuleShape(const Entity& entity, const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius, b8 isPrimary = false,
            PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE)
    {
        if(!m_EntityManager->HasComponent<Rigidbody2DComponent>(entity))
            return;

        auto& rigidbody = m_EntityManager->GetComponent<Rigidbody2DComponent>(entity);

        if(!b2Body_IsValid(rigidbody.BodyId))
        {
            rigidbody.BodyId = m_Physics2D->CreateBody(rigidbody.Type, rigidbody.Position);
        }

        b2Capsule capsule = m_Physics2D->CreateCapsuleShape(center1, center2, radius);

        b2ShapeId newShape  = m_Physics2D->AddCapsule(rigidbody.BodyId, capsule, rigidbody.Density, rigidbody.Friction, rigidbody.Restitution);

        rigidbody.ShapeIds.push_back(newShape);
        
        MultiColliderComponent colliderData = {};
        colliderData.Type = PhysicsColliderType::CAPSULE;
        colliderData.CapsuleCollider2D.center1 = center1;
        colliderData.CapsuleCollider2D.center2 = center2;
        colliderData.CapsuleCollider2D.radius = radius;
        colliderData.CapsuleCollider2D.shapeId = newShape;
        
        rigidbody.MultiColliderComponents.push_back(colliderData);

        if(isPrimary || rigidbody.ShapeIds.size() == 1)
        {
            rigidbody.PrimaryId = newShape;
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
