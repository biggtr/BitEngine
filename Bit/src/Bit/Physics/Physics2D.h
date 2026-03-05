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

b8 Physics2DInitialize(u64* memoryRequirements, void* state);
void Physics2DShutdown(void* state);
b2WorldId Physics2DCreateWorld(const BMath::Vec2& gravity = {0.0f, -10.0f});
void Physics2DDestroyWorld();
b2BodyId Physics2DCreateBody(PhysicsBodyType bodyType, const BMath::Vec3& position);

b2Circle Physics2DCreateCircleShape(const BMath::Vec2& center, f32 radius);
b2Polygon Physics2DCreateBoxShape(f32 width, f32 height, const BMath::Vec2& offset, f32 angle = 0.0f);
b2Capsule Physics2DCreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius);


b2ShapeId Physics2DAddCircle(b2BodyId body, const b2Circle& circle, 
                float density = 1.0f,
                float friction = 0.3f,
                float restitution = 0.0f,
                PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

b2ShapeId Physics2DAddBox(b2BodyId body, const b2Polygon& box,
                float density = 1.0f,
                float friction = 0.3f,
                float restitution = 0.0f,
                PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

b2ShapeId Physics2DAddCapsule(b2BodyId body, const b2Capsule& capsule, 
                float density = 1.0f,
                float friction = 0.3f,
                float restitution = 0.0f,
                PhysicsCategories categoryType = PhysicsCategories::NONE, PhysicsCategories categoryToCollideWith = PhysicsCategories::NONE);

void Physics2DSetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity);
BMath::Vec2 Physics2DGetLinearVelocity(b2BodyId bodyID);

BMath::Vec2 Physics2DGetPosition(b2BodyId bodyID); 
void Physics2DSetPosition(b2BodyId bodyID, BMath::Vec2 position); 
f32 Physics2DGetRotation(b2BodyId bodyID); 

void Physics2DUpdate(f32 deltaTime);
f32 Physics2DGetFixedTimeStep(); 

b2ContactEvents Physics2DGetContactEvents(); 

f32 Physics2DCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context);

CastRayContext Physics2DCastRay(const BMath::Vec2& origin, const BMath::Vec2& translation, b2ShapeId ignoreShapeID);


};


