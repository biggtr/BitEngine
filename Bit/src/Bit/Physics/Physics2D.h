#pragma once
#include <vector>
#include "Bit/Math/Vector.h"
#include "Bit/Core/Defines.h"
#include "Bit/Physics/PhysicsTypes.h"
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

struct CastRayContext
{
    b2ShapeId ShapeID;
    b2ShapeId IgnoreShapeID;
    b2Vec2 Point;
    b2Vec2 Normal;
    f32 Fraction;
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
    b2Polygon CreateBoxShape(f32 width, f32 height, const BMath::Vec2& offset, f32 angle = 0.0f);
    b2Capsule CreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius);


    b2ShapeId AddCircle(b2BodyId body, const b2Circle& circle, 
                    float density = 1.0f,
                    float friction = 0.3f,
                    float restitution = 0.0f,
                    PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

    b2ShapeId AddBox(b2BodyId body, const b2Polygon& box,
                    float density = 1.0f,
                    float friction = 0.3f,
                    float restitution = 0.0f,
                    PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

    b2ShapeId AddCapsule(b2BodyId body, const b2Capsule& capsule, 
                    float density = 1.0f,
                    float friction = 0.3f,
                    float restitution = 0.0f,
                    PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

    void SetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity);
    BMath::Vec2 GetLinearVelocity(b2BodyId bodyID);

    BMath::Vec2 GetPosition(b2BodyId bodyID) { b2Vec2 pos = b2Body_GetPosition(bodyID); return {pos.x, pos.y}; }
    void SetPosition(b2BodyId bodyID, BMath::Vec2 position) { b2Body_SetTransform(bodyID, (b2Vec2){position.x, position.y}, b2Body_GetRotation(bodyID)); }
    f32 GetRotation(b2BodyId bodyID) { b2Rot q = b2Body_GetRotation(bodyID); return atan2f(q.s, q.c); }

    void Step();
    f32 GetFixedTimeStep() { return m_FixedTimeStep; }

    b2ContactEvents GetContactEvents() { return b2World_GetContactEvents(m_World); }

    static f32 CastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context);
 
    CastRayContext CastRay(const BMath::Vec2& origin, const BMath::Vec2& translation, b2ShapeId ignoreShapeID);
private:
    b2WorldId m_World; // maybe change in future to support many worlds for different scenes idk!
    BMath::Vec2 m_Gravity; 
    b2ContactEvents m_ContactEvents;

    f32 m_FixedTimeStep = 1.0f / 60.0f;
    i32 m_SubSteps = 4;
};

}

