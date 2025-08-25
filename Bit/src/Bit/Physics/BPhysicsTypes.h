#pragma once

#include "Bit/Math/Vector.h"
namespace BPhysics2D
{
struct BParticle
{
    BMath::Vec3 SumForces;
    BMath::Vec3 Acceleration;
    BMath::Vec3 Velocity; 
    BMath::Vec3 Position;
    f32 Mass;
    f32 InvMass;
    f32 Gravity;

    BParticle(const BMath::Vec3& position, f32 mass)
        :  SumForces(0.0f), Acceleration(0.0f), Velocity(0.0f),
           Position(position), Mass(mass), Gravity(9.8f)
    {
        InvMass = Mass == 0.0f ? 0.0f : 1.0f / Mass;
    }
};
enum SHAPE_TYPE
{
    SHAPE_CIRCLE,
    SHAPE_BOX
};
struct BCircleShape
{
    f32 Radius;
    f32 InertiaWithoutMass;
};
struct BPolygonShape
{
    std::vector<BMath::Vec3> Vertices;
    f32 InertiaWithoutMass;
    BPolygonShape(std::vector<BMath::Vec3> vertices)
        : Vertices(vertices)
    {
        InertiaWithoutMass = 0.0f;
    }
};
struct BBoxShape
{
    f32 Width;
    f32 Height;
    f32 InertiaWithoutMass;
};
struct BShape
{
    SHAPE_TYPE Type;
    union
    {
        BCircleShape BCircle;
        BBoxShape BBox;
    };
};
struct BBody
{
    u32 ShapeIndex;

    BMath::Vec3 Acceleration;
    BMath::Vec3 Velocity; 
    BMath::Vec3 Position;

    BMath::Vec3 SumForces;

    f32 Mass;
    f32 InvMass;

    f32 Rotation;
    f32 AngularVelocity;
    f32 AngularAcceleration;

    f32 SumTorques;
    
    f32 Inertia;
    f32 InvInertia;
};
}
