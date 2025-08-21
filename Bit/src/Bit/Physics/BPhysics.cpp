#include "BPhysics.h"
#include "Bit/Math/Vector.h"

namespace BPhysics
{

static BPhysics2DState* physicsState;

b8 BPhysics2D::Initialize()
{
    physicsState = new BPhysics2DState();

    return true;
}
void BPhysics2D::Shutdown()
{
    delete physicsState;
}

u32 BPhysics2D::BCreateCircleShape(f32 radius)
{
    BShape shape; 
    shape.Type = SHAPE_CIRCLE;
    shape.BCircle.Radius = radius;
    shape.BCircle.InertiaWithoutMass = 0.5f * (radius * radius);
    physicsState->Shapes.push_back(shape);
    return physicsState->Shapes.size() - 1; // return the id of shape
}
u32 BPhysics2D::BCreateBoxShape(f32 width, f32 height)
{
    BShape shape;
    shape.Type = SHAPE_BOX;
    shape.BBox.Width = width; 
    shape.BBox.Height= height; 
    shape.BBox.InertiaWithoutMass = (1.0f/12.0f) * ((width * width) + (height * height));
    physicsState->Shapes.push_back(shape);
    return physicsState->Shapes.size() - 1; // return the id of shape
}
u32 CreateBody(u32 ShapeIndex, const BMath::Vec3& position, f32 mass)
{
    const BShape& shape = physicsState->Shapes[ShapeIndex];
    BBody body; 
    body.ShapeIndex = ShapeIndex;
    body.Position = position;
    body.Mass = mass;
    body.InvMass = mass == 0.0 ? 0.0f : 1.0f / mass;
    switch (shape.Type) 
    {
        case SHAPE_CIRCLE:
            body.Inertia = shape.BCircle.InertiaWithoutMass * body.Mass;
            body.InvInertia = body.Inertia == 0.0f ? 0.0f : 1.0f / body.Inertia;
            break;

        case SHAPE_BOX:
            body.Inertia = shape.BBox.InertiaWithoutMass * body.Mass;
            body.InvInertia = body.Inertia == 0.0f ? 0.0f : 1.0f / body.Inertia;
            break;
    }
    physicsState->Bodies.push_back(body);
    return physicsState->Bodies.size() - 1;
}
void BPhysics2D::AddForce(BParticle& particle, BMath::Vec3& force)
{
    particle.SumForces += force;
}
BMath::Vec3 BPhysics2D::GenerateDragForce(BParticle particle, f32 dragValue)
{
    //dragValue -> FluidDensity * CoeffDrag * CrossSurfaceArea = constant value
    if(BMath::Vec3::LengthSquared(particle.Velocity) < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    BMath::Vec3 dragDirection = particle.Velocity.Normalize() * -1.0f;
    BMath::Vec3 dragMagnitude = BMath::Vec3::LengthSquared(particle.Velocity) * dragValue;
    BMath::Vec3 dragForce = dragDirection * dragMagnitude;

    return dragForce;
}
BMath::Vec3 BPhysics2D::GenerateFrictionForce(BParticle particle, f32 frictionValue)
{
    //frictionValue -> CoeffOfSurfaceFriction * Magnitude if normal force of the surface on the particle
    BMath::Vec3 frictionDirection = particle.Velocity.Normalize() * -1.0f;
    BMath::Vec3 frictionForce = frictionDirection * frictionValue;

    return frictionForce;
}
BMath::Vec3 BPhysics2D::GenerateGravitationalForce(BParticle a, BParticle b, f32 G)
{
    BMath::Vec3 d = b.Position - a.Position;
    f32 distanceSquared = BMath::Vec3::LengthSquared(d);
    if(distanceSquared < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    f32 attractionMagintude = G * (a.Mass * b.Mass) / distanceSquared;
    BMath::Vec3 attractionDirection = d.Normalize();
    BMath::Vec3 gravitationalForce = attractionDirection * attractionMagintude  ;


    return gravitationalForce;

}
// k -> Spring stiff constant == how much force do we need to deform the object
// l -> displacement after deformation
BMath::Vec3 BPhysics2D::GenerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k)
{
    BMath::Vec3 d = particle.Position - anchor;
    f32 displacement = BMath::Vec3::Length(d) - restLength;

    BMath::Vec3 springDirection = d.Normalize();
    f32 springForceMagnitude = -k * displacement;
    BMath::Vec3 springForce = springDirection * springForceMagnitude;
    return springForce;
}
void BPhysics2D::EnableWeight(BParticle& particle, f32 gravity)
{
    BMath::Vec3 weightForce(0.0, particle.Mass * gravity, 0.0f);

    AddForce(particle, weightForce);
}
void BPhysics2D::LinearIntegrate(BParticle particle, f32 deltaTime)
{
    if(particle.Mass == 0)
    {
        return;
    }
    particle.Acceleration = particle.SumForces * particle.InvMass;
    particle.Velocity += particle.Acceleration * deltaTime;
    particle.Position += particle.Velocity * deltaTime;

    // Clearing All Forces after Integration
    particle.SumForces = (0.0f);
}

void BPhysics2D::AddForce(BBody& body, BMath::Vec3& force)
{
    body.SumForces += force;
}
BMath::Vec3 BPhysics2D::GenerateDragForce(BBody body, f32 dragValue)
{
    //dragValue -> FluidDensity * CoeffDrag * CrossSurfaceArea = constant value
    if(BMath::Vec3::LengthSquared(body.Velocity) < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    BMath::Vec3 dragDirection = body.Velocity.Normalize() * -1.0f;
    BMath::Vec3 dragMagnitude = BMath::Vec3::LengthSquared(body.Velocity) * dragValue;
    BMath::Vec3 dragForce = dragDirection * dragMagnitude;

    return dragForce;
}
BMath::Vec3 BPhysics2D::GenerateFrictionForce(BBody body, f32 frictionValue)
{
    //frictionValue -> CoeffOfSurfaceFriction * Magnitude if normal force of the surface on the body
    BMath::Vec3 frictionDirection = body.Velocity.Normalize() * -1.0f;
    BMath::Vec3 frictionForce = frictionDirection * frictionValue;

    return frictionForce;
}
BMath::Vec3 BPhysics2D::GenerateGravitationalForce(BBody a, BBody b, f32 G)
{
    BMath::Vec3 d = b.Position - a.Position;
    f32 distanceSquared = BMath::Vec3::LengthSquared(d);
    if(distanceSquared < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    f32 attractionMagintude = G * (a.Mass * b.Mass) / distanceSquared;
    BMath::Vec3 attractionDirection = d.Normalize();
    BMath::Vec3 gravitationalForce = attractionDirection * attractionMagintude  ;


    return gravitationalForce;

}
// k -> Spring stiff constant == how much force do we need to deform the object
// l -> displacement after deformation
BMath::Vec3 BPhysics2D::GenerateSpringForce(BBody body, BMath::Vec3& anchor,f32 restLength, f32 k)
{
    BMath::Vec3 d = body.Position - anchor;
    f32 displacement = BMath::Vec3::Length(d) - restLength;

    BMath::Vec3 springDirection = d.Normalize();
    f32 springForceMagnitude = -k * displacement;
    BMath::Vec3 springForce = springDirection * springForceMagnitude;
    return springForce;
}
void BPhysics2D::EnableWeight(BBody& body, f32 gravity)
{
    BMath::Vec3 weightForce(0.0, body.Mass * gravity, 0.0f);
    
    AddForce(body, weightForce);
}
void BPhysics2D::LinearIntegrate(BBody body, f32 deltaTime)
{
    if(body.Mass == 0)
    {
        return;
    }
    body.Acceleration = body.SumForces * body.InvMass;
    body.Velocity += body.Acceleration * deltaTime;
    body.Position += body.Velocity * deltaTime;

    // Clearing All Forces after Integration
    body.SumForces = (0.0f);
}

void BPhysics2D::AngularIntegrate(BBody body, f32 deltaTime)
{
    body.AngularAcceleration = body.SumTorques * body.InvInertia;
    body.AngularVelocity += body.AngularAcceleration * deltaTime;
    body.Rotation += body.AngularVelocity * deltaTime;
}
}
