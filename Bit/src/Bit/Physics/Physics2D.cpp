#include "Physics2D.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/types.h"
#include <cstring>
#include <zlib.h>

namespace BitEngine
{


struct Physics2DState
{
    b2WorldId m_World; // maybe change in future to support many worlds for different scenes idk!
    BMath::Vec2 m_Gravity; 
    b2ContactEvents m_ContactEvents;

    f32 m_FixedTimeStep; 
    i32 m_SubSteps;
    f32 m_Accumulator;
};
static Physics2DState* physicsState = 0;

b8 Physics2DInitialize(u64* memoryRequirements, void* state)
{
    *memoryRequirements = sizeof(Physics2DState);
    if(!state)
    {
        return true;
    }
    physicsState = (Physics2DState*)state;
    memset(state, 0, sizeof(Physics2DState));

    physicsState->m_FixedTimeStep = 1.0f / 60.0f;
    physicsState->m_SubSteps = 4;
    physicsState->m_Accumulator = 0.0f;
    return true;
    
}
void Physics2DShutdown(void* state)
{
    if(physicsState && b2World_IsValid(physicsState->m_World))
    {
        b2DestroyWorld(physicsState->m_World);
    }
    physicsState = nullptr;
    BIT_LOG_INFO("Physics2D Shutdown");
}
b2WorldId Physics2DCreateWorld(const BMath::Vec2& gravity)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(gravity.x, gravity.y);

    physicsState->m_World = b2CreateWorld(&worldDef);
    return physicsState->m_World;
}

void Physics2DDestroyWorld()
{
    b2DestroyWorld(physicsState->m_World);
}

b2BodyId Physics2DCreateBody(PhysicsBodyType bodyType, const BMath::Vec3& position)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();

    switch (bodyType) 
    {
        case PhysicsBodyType::Static:
            bodyDef.type = b2_staticBody;
            break;
        case PhysicsBodyType::Dynamic:
            bodyDef.type = b2_dynamicBody;
            break;
        case PhysicsBodyType::Kinematic:
            bodyDef.type = b2_kinematicBody;
            break;

      break;
    }
    bodyDef.position = b2Vec2(position.x, position.y);
    b2BodyId bodyId = b2CreateBody(physicsState->m_World, &bodyDef);

    return bodyId;
}

b2Circle Physics2DCreateCircleShape(const BMath::Vec2& center, f32 radius)
{
    b2Circle circle = { .center = b2Vec2(center.x, center.y), .radius = radius };
    return circle;
}

b2Polygon Physics2DCreateBoxShape(f32 width, f32 height, const BMath::Vec2& offset, f32 angle)
{
    b2Rot rotation = b2MakeRot(angle);
    b2Polygon box = b2MakeOffsetBox(width * 0.5f, height * 0.5f, b2Vec2(offset.x, offset.y), rotation);
    return box;
}

b2Capsule Physics2DCreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius)
{
    b2Capsule capsule;
    capsule.center1 = b2Vec2(center1.x, center1.y);
    capsule.center2 = b2Vec2(center2.x, center2.y);
    capsule.radius = radius;
    return capsule;
}

b2ShapeId Physics2DAddCircle(b2BodyId body, const b2Circle& circle, 
                    float density, 
                    float friction,
                    float restitution, 
                    PhysicsCategories categoryType, PhysicsCategories categoryToCollideWith)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.filter.categoryBits = categoryType;
    def.filter.maskBits = categoryToCollideWith;
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreateCircleShape(body, &def, &circle);
}

b2ShapeId Physics2DAddBox(b2BodyId body, const b2Polygon& box,
                    float density, 
                    float friction,
                    float restitution, 
                    PhysicsCategories categoryType, PhysicsCategories categoryToCollideWith)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.filter.categoryBits = categoryType;
    def.filter.maskBits = categoryToCollideWith;
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreatePolygonShape(body, &def, &box);
}
b2ShapeId Physics2DAddCapsule(b2BodyId body, const b2Capsule& capsule, 
                    float density, 
                    float friction,
                    float restitution, 
                    PhysicsCategories categoryType, PhysicsCategories categoryToCollideWith)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.filter.categoryBits = categoryType;
    def.filter.maskBits = categoryToCollideWith;
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreateCapsuleShape(body, &def, &capsule);
}

void Physics2DSetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity)
{
    b2Body_SetLinearVelocity(bodyID, b2Vec2(velocity.x, velocity.y));
}

BMath::Vec2 Physics2DGetLinearVelocity(b2BodyId bodyID)
{
    b2Vec2 vel = b2Body_GetLinearVelocity(bodyID);
    return BMath::Vec2(vel.x, vel.y);
}
void Physics2DUpdate(f32 deltaTime)
{
    physicsState->m_Accumulator += deltaTime;

    while(physicsState->m_Accumulator >= physicsState->m_FixedTimeStep)
    {
        b2World_Step(physicsState->m_World, physicsState->m_FixedTimeStep, physicsState->m_SubSteps);
        physicsState->m_Accumulator -= physicsState->m_FixedTimeStep;
    }
}

f32 Physics2DCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context)
{
    CastRayContext* myContext = (CastRayContext*)context;

    if(shapeId.index1 == myContext->IgnoreShapeID.index1)
        return 1.0f;
    if (fraction < myContext->Fraction)
    {
        myContext->ShapeID = shapeId;
        myContext->Point = point;
        myContext->Normal = normal;
        myContext->Fraction = fraction;
    }

    return fraction;
}
CastRayContext Physics2DCastRay(const BMath::Vec2& origin, const BMath::Vec2& translation, b2ShapeId ignoreShapeID)
{
    CastRayContext context = {};
    context.Fraction = 1.0f;
    context.IgnoreShapeID = ignoreShapeID;
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2World_CastRay(physicsState->m_World, b2Vec2(origin.x, origin.y), b2Vec2(translation.x, translation.y), filter, Physics2DCastCallback, &context);
    return context;
}

f32 Physics2DGetFixedTimeStep() { return physicsState->m_FixedTimeStep; }

b2ContactEvents Physics2DGetContactEvents() { return b2World_GetContactEvents(physicsState->m_World); }

BMath::Vec2 Physics2DGetPosition(b2BodyId bodyID) { b2Vec2 pos = b2Body_GetPosition(bodyID); return {pos.x, pos.y}; }
void Physics2DSetPosition(b2BodyId bodyID, BMath::Vec2 position) { b2Body_SetTransform(bodyID, b2Vec2(position.x, position.y), b2Body_GetRotation(bodyID)); }
f32 Physics2DGetRotation(b2BodyId bodyID) { b2Rot q = b2Body_GetRotation(bodyID); return atan2f(q.s, q.c); }
}
