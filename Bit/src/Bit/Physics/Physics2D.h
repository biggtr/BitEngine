#pragma once
#include <box2d/box2d.h>
#include <vector>
#include "Bit/Math/Vector.h"

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
    Physics2D();
    ~Physics2D();

    void CreateWorld(const BMath::Vec2& gravity = {0.0f, -10.0f});
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
private:
    b2WorldId m_World; // maybe change in future to support many worlds for different scenes idk!
    BMath::Vec2 m_Gravity; 

};

}

