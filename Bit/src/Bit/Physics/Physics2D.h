#pragma once
#include <vector>
#include "Bit/Math/Vector.h"
#include "box2d/box2d.h"

namespace BitEngine
{

enum class PhysicsBodyType
{
    Static,
    Dynamic,
    Kinematic
};
enum class PhysicsShapeType
{
    Circle,
    Polygon,
    Capsule
};
class Physics2D
{

public:
    Physics2D(){}
    ~Physics2D(){}

    b2WorldId CreateWorld(const BMath::Vec2& gravity = {0.0f, -10.0f});
    void DestroyWorld();
    b2BodyId CreateBody(PhysicsBodyType bodyType, const BMath::Vec3& position);

    b2Circle CreateCircleShape(const BMath::Vec2& center, f32 radius);
    b2Polygon CreateBoxShape(f32 width, f32 height);
    b2Capsule CreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius);


    b2ShapeId AddCircle(b2BodyId body, const b2Circle& circle,
                        float density = 1.0f,
                        float friction = 0.3f,
                        float restitution = 0.0f);

    b2ShapeId AddBox(b2BodyId body, const b2Polygon& box,
                     float density = 1.0f,
                     float friction = 0.3f,
                     float restitution = 0.0f);

    b2ShapeId AddCapsule(b2BodyId body, const b2Capsule& capsule,
                         float density = 1.0f,
                         float friction = 0.3f,
                         float restitution = 0.0f);

    void SetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity);
    BMath::Vec2 GetLinearVelocity(b2BodyId bodyID);

    BMath::Vec2 GetPosition(b2BodyId bodyID) { b2Vec2 pos = b2Body_GetPosition(bodyID); return {pos.x, pos.y}; }
    f32 GetRotation(b2BodyId bodyID) { b2Rot q = b2Body_GetRotation(bodyID); return atan2f(q.s, q.c); }

    void Step();
    f32 GetFixedTimeStep() { return m_FixedTimeStep; }
private:
    b2WorldId m_World; // maybe change in future to support many worlds for different scenes idk!
    BMath::Vec2 m_Gravity; 

    f32 m_FixedTimeStep = 1.0f / 60.0f;
    i32 m_SubSteps = 4;
};

}

