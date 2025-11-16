// Bit/ECS/Systems/Physics2DSystem.h
#pragma once
#include "System.h"
#include "Bit/Math/Vector.h"
#include <box2d/box2d.h>
#include <unordered_map>

namespace BitEngine
{
    class Physics2DSystem : public System
    {
    public:
        SYSTEM_CLASS_TYPE(PHYSICS2D)

        Physics2DSystem();
        ~Physics2DSystem();

        void OnEntityAdded(const Entity& entity) override;
        void RemoveEntity(const Entity& entity) override;
        void Update(f32 deltaTime);

        // Simple API for your game
        void SetGravity(f32 x, f32 y);
        void SetVelocity(Entity entity, const BMath::Vec3& velocity);
        BMath::Vec3 GetVelocity(Entity entity);
        b8 IsGrounded(Entity entity);

    private:
        void CreateBody(const Entity& entity);
        void DestroyBody(Entity entityId);
        void SyncToBox2D(const Entity& entity);
        void SyncFromBox2D(const Entity& entity);
        void UpdateGroundContacts();

    private:
        b2WorldId m_WorldId;
        f32 m_FixedTimeStep = 1.0f / 60.0f;
        f32 m_Accumulator = 0.0f;
        i32 m_SubSteps = 4;
        
        std::unordered_map<Entity, i32> m_GroundContacts;
    };
}
