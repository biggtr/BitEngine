#include "Bit/Math/Vector.h"
namespace BPhysics
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
    SHAPE_POLYGON,
    SHAPE_BOX
};
struct BCircleShape
{
    f32 Radius;
    f32 InertiaWithoutMass;
    BCircleShape(f32 radius)
        : Radius(radius)
    {
        InertiaWithoutMass = 0.5f * (radius * radius);
    }
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
    BBoxShape(f32 width, f32 height)
        : Width(width), Height(height)
    {
        InertiaWithoutMass = (1.0f/12.0f) * ((Width * Width) + (Height * Height));
    }
};
struct BBody
{
    SHAPE_TYPE ShapeType;

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
    union
    {
        BCircleShape BCircle;
        BPolygonShape BPolygon;
        BBoxShape BBox;
    };
    
    BBody(SHAPE_TYPE shapeType, const BMath::Vec3& position, f32 mass)
        : ShapeType(shapeType), Position(position), Mass(mass)
    {
        switch (shapeType) 
        {
            case SHAPE_CIRCLE:
                Inertia = BCircle.InertiaWithoutMass * Mass;
                break;
            case SHAPE_POLYGON:
                Inertia = BPolygon.InertiaWithoutMass * Mass;
                break;
            case SHAPE_BOX:
                Inertia = BBox.InertiaWithoutMass * Mass;
                break;
        }
    }
};
}
