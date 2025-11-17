#include "Physics2D.h"
#include "Bit/Math/Vector.h"
#include "box2d/box2d.h"
#include "box2d/types.h"


namespace BitEngine
{

b2WorldId Physics2D::CreateWorld(const BMath::Vec2& gravity)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){gravity.x, gravity.y};

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
    bodyDef.position = (b2Vec2){position.x, position.y};
    b2BodyId bodyId = b2CreateBody(m_World, &bodyDef);

    return bodyId;
}

b2Circle Physics2D::CreateCircleShape(const BMath::Vec2& center, f32 radius)
{
    b2Circle circle =  {.center = (b2Vec2){center.x, center.y}, .radius = radius}; 
    return circle;
}

b2Polygon Physics2D::CreateBoxShape(f32 width, f32 height)
{
    b2Polygon box = b2MakeBox(width * 0.5f, height * 0.5f);
    return box;
}

b2Capsule Physics2D::CreateCapsuleShape(const BMath::Vec2& center1, const BMath::Vec2& center2, f32 radius)
{
    b2Capsule capsule;
    capsule.center1 = (b2Vec2){center1.x, center1.y};
    capsule.center2 = (b2Vec2){center2.x, center2.y};
    capsule.radius = radius;
    return capsule;
}

b2ShapeId Physics2D::AddCircle(b2BodyId body, const b2Circle& circle,
                               float density, float friction, float restitution)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreateCircleShape(body, &def, &circle);
}

b2ShapeId Physics2D::AddBox(b2BodyId body, const b2Polygon& box,
                               float density, float friction, float restitution)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreatePolygonShape(body, &def, &box);
}
b2ShapeId Physics2D::AddCapsule(b2BodyId body, const b2Capsule& capsule,
                               float density, float friction, float restitution)
{
    b2ShapeDef def = b2DefaultShapeDef();
    def.density = density;
    def.material.friction = friction;
    def.material.restitution = restitution;

    return b2CreateCapsuleShape(body, &def, &capsule);
}

void Physics2D::SetLinearVelocity(b2BodyId bodyID, const BMath::Vec3& velocity)
{
    b2Body_SetLinearVelocity(bodyID, (b2Vec2){velocity.x, velocity.y});
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

}
