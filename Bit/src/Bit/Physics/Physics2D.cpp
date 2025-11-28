#include "Physics2D.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Vector.h"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/types.h"
#include <cstring>


namespace BitEngine
{

b2WorldId Physics2D::CreateWorld(const BMath::Vec2& gravity)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(gravity.x, gravity.y);

    m_World = b2CreateWorld(&worldDef);
    return m_World;
}

void Physics2D::DestroyWorld()
{
    b2DestroyWorld(m_World);
}

b2BodyId Physics2D::CreateBody(PhysicsBodyType bodyType, const BMath::Vec3& position)
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
    b2BodyId bodyId = b2CreateBody(m_World, &bodyDef);

    return bodyId;
}

b2Circle Physics2D::CreateCircleShape(const BMath::Vec2& center, f32 radius)
{
    b2Circle circle = { .center = b2Vec2(center.x, center.y), .radius = radius };
    return circle;
}

b2Polygon Physics2D::CreateBoxShape(f32 width, f32 height, const BMath::Vec2& offset, f32 angle)
{
    b2Rot rotation = b2MakeRot(angle);
    b2Polygon box = b2MakeOffsetBox(width * 0.5f, height * 0.5f, b2Vec2(offset.x, offset.y), rotation);
    return box;
}

b2Capsule Physics2D::CreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius)
{
    b2Capsule capsule;
    capsule.center1 = b2Vec2(center1.x, center1.y);
    capsule.center2 = b2Vec2(center2.x, center2.y);
    capsule.radius = radius;
    return capsule;
}

b2ShapeId Physics2D::AddCircle(b2BodyId body, const b2Circle& circle, 
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

b2ShapeId Physics2D::AddBox(b2BodyId body, const b2Polygon& box,
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
b2ShapeId Physics2D::AddCapsule(b2BodyId body, const b2Capsule& capsule, 
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

void Physics2D::SetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity)
{
    b2Body_SetLinearVelocity(bodyID, b2Vec2(velocity.x, velocity.y));
}

BMath::Vec2 Physics2D::GetLinearVelocity(b2BodyId bodyID)
{
    b2Vec2 vel = b2Body_GetLinearVelocity(bodyID);
    return BMath::Vec2(vel.x, vel.y);
}
void Physics2D::Step()
{
    b2World_Step(m_World, m_FixedTimeStep, m_SubSteps);
}

f32 Physics2D::CastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context)
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
CastRayContext Physics2D::CastRay(const BMath::Vec2& origin, const BMath::Vec2& translation, b2ShapeId ignoreShapeID)
{
    CastRayContext context = {};
    context.Fraction = 1.0f;
    context.IgnoreShapeID = ignoreShapeID;
    b2QueryFilter filter = b2DefaultQueryFilter();
    b2World_CastRay(m_World, b2Vec2(origin.x, origin.y), b2Vec2(translation.x, translation.y), filter, CastCallback, &context);
    return context;
}

}
