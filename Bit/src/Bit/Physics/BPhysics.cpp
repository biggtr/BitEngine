#include "BPhysics.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysicsTypes.h"

namespace BPhysics2D
{

static BPhysics2DState* physicsState;

b8 Initialize()
{
    physicsState = new BPhysics2DState();

    return true;
}
void Shutdown()
{
    delete physicsState;
}

u32 BCreateCircleShape(f32 radius)
{
    BShape shape; 
    shape.Type = SHAPE_CIRCLE;
    shape.BCircle.Radius = radius;
    shape.BCircle.InertiaWithoutMass = 0.5f * (radius * radius);
    u32 shapeIndex = physicsState->Shapes.size();
    physicsState->Shapes.push_back(shape);
    return shapeIndex;
}
u32 BCreateBoxShape(f32 width, f32 height)
{
    BMath::Vec3 v1 = {-width * 0.5f, -height * 0.5f, 0.0f};
    BMath::Vec3 v2 = { width * 0.5f, -height * 0.5f, 0.0f};
    BMath::Vec3 v3 = { width * 0.5f,  height * 0.5f, 0.0f};
    BMath::Vec3 v4 = {-width * 0.5f,  height * 0.5f, 0.0f};
    BMath::Vec3 vertices[4] = 
    {
        v1, v2,
        v3, v4
    };
    f32 inertiaWithoutMass = (1.0f/12.0f) * ((width * width) + (height * height));
    return BCreatePolygonShape(vertices, 4, inertiaWithoutMass);
}
u32 BCreatePolygonShape(const BMath::Vec3* vertices, u32 count, f32 inertiaWithoutMass)
{
    BShape shape;
    shape.Type = SHAPE_POLYGON;
    if(count > MAX_POLYGON_VERTEX_COUNT)
        count = MAX_POLYGON_VERTEX_COUNT;

    for(u32 i = 0; i < count; ++i)
    {
        shape.BPolygon.Vertices[i] = vertices[i];
    }
    shape.BPolygon.InertiaWithoutMass = inertiaWithoutMass;
    shape.BPolygon.VertexCount = count;
    u32 shapeIndex = physicsState->Shapes.size();
    physicsState->Shapes.push_back(shape);
    return shapeIndex;
}
u32 CreateBody(u32 ShapeIndex, const BMath::Vec3& position, f32 mass, f32 restitution)
{
    const BShape& shape = physicsState->Shapes[ShapeIndex];
    BBody body; 
    body.ShapeIndex = ShapeIndex;
    body.Position = position;
    // BIT_LOG_DEBUG("from physics pos is : %f, %f, %f", body.Position.x, body.Position.y, body.Position.z);
    body.Rotation = 0.0f;
    body.Acceleration = {0.0f, 0.0f, 0.0f};
    body.Velocity = {0.0f, 0.0f, 0.0f};
    body.Mass = mass;
    body.InvMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; 
    body.Restitution = restitution;
    switch (shape.Type) 
    {
        case SHAPE_CIRCLE:
            body.Inertia = shape.BCircle.InertiaWithoutMass * body.Mass;
            body.InvInertia = NearlyEqual(body.Inertia, 0.0f) ? 0.0f : 1.0f / body.Inertia;
            break;

        case SHAPE_BOX:
            body.Inertia = shape.BBox.InertiaWithoutMass * body.Mass;
            body.InvInertia = NearlyEqual(body.Inertia, 0.0f) ? 0.0f : 1.0f / body.Inertia;
            break;
        case SHAPE_POLYGON:
          break;
        }
    u32 bodyIndex = physicsState->Bodies.size();
    physicsState->Bodies.push_back(body);
    return bodyIndex;
}
BShape& GetShape(BBody* body)
{
    return physicsState->Shapes[body->ShapeIndex];
}

BBody& GetBody(u32 bodyIndex)
{
    return physicsState->Bodies[bodyIndex];
}

BMath::Vec3 Vec3EdgeAt(BPolygonShape& polygon, u32 index)
{
    u32 currentVertex = index;
    u32 nextVertex = (index + 1) % polygon.VertexCount;

    BMath::Vec3 edge = polygon.Vertices[nextVertex] - polygon.Vertices[currentVertex];
    return edge;
}
////////////////////////////////////////////////////////
// Linear Movement
////////////////////////////////////////////////////////
void AddForce(BParticle& particle, BMath::Vec3& force)
{
    particle.SumForces += force;
}
BMath::Vec3 GenerateDragForce(BParticle particle, f32 dragValue)
{
    //dragValue -> FluidDensity * CoeffDrag * CrossSurfaceArea = constant value
    if(BMath::Vec3LengthSquared(particle.Velocity) < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    BMath::Vec3 dragDirection = Vec3Normalize(particle.Velocity) * -1.0f;
    f32 dragMagnitude = BMath::Vec3LengthSquared(particle.Velocity) * dragValue;
    BMath::Vec3 dragForce = dragDirection * dragMagnitude;

    return dragForce;
}
BMath::Vec3 GenerateFrictionForce(BParticle particle, f32 frictionValue)
{
    //frictionValue -> CoeffOfSurfaceFriction * Magnitude if normal force of the surface on the particle
    BMath::Vec3 frictionDirection = Vec3Normalize(particle.Velocity) * -1.0f;
    BMath::Vec3 frictionForce = frictionDirection * frictionValue;

    return frictionForce;
}
BMath::Vec3 GenerateGravitationalForce(BParticle a, BParticle b, f32 G)
{
    BMath::Vec3 d = b.Position - a.Position;
    f32 distanceSquared = BMath::Vec3LengthSquared(d);
    if(distanceSquared < 0.0f)
    {
        return BMath::Vec3(0.0f);
    }
    f32 attractionMagintude = G * (a.Mass * b.Mass) / distanceSquared;
    BMath::Vec3 attractionDirection = Vec3Normalize(d);
    BMath::Vec3 gravitationalForce = attractionDirection * attractionMagintude  ;


    return gravitationalForce;

}
// k -> Spring stiff constant == how much force do we need to deform the object
// l -> displacement after deformation
BMath::Vec3 GenerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k)
{
    BMath::Vec3 d = particle.Position - anchor;
    f32 displacement = BMath::Vec3Length(d) - restLength;

    BMath::Vec3 springDirection = Vec3Normalize(d);
    f32 springForceMagnitude = -k * displacement;
    BMath::Vec3 springForce = springDirection * springForceMagnitude;
    return springForce;
}
void EnableWeight(BParticle& particle, f32 gravity)
{
    BMath::Vec3 weightForce(0.0, particle.Mass * gravity, 0.0f);

    AddForce(particle, weightForce);
}
void LinearIntegrate(BParticle particle, f32 deltaTime)
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

void AddForce(BBody& body, const BMath::Vec3& force)
{
    body.SumForces += force;
}
void ApplyImpulse(BBody& body, const BMath::Vec3& impulse)
{
    if(NearlyEqual(body.Mass, 0.0f))
        return;

    body.Velocity += impulse * body.InvMass;
}
BMath::Vec3 GenerateDragForce(BBody& body, f32 dragValue)
{
    //dragValue -> FluidDensity * CoeffDrag * CrossSurfaceArea = constant value
    if(NearlyEqual(BMath::Vec3LengthSquared(body.Velocity), 0.0f))
    {
        return BMath::Vec3(0.0f);
    }
    BMath::Vec3 dragDirection = Vec3Normalize(body.Velocity) * -1.0f;
    f32 dragMagnitude = BMath::Vec3LengthSquared(body.Velocity) * dragValue;
    BMath::Vec3 dragForce = dragDirection * dragMagnitude;

    return dragForce;
}
BMath::Vec3 GenerateFrictionForce(BBody& body, f32 frictionValue)
{
    //frictionValue -> CoeffOfSurfaceFriction * Magnitude if normal force of the surface on the body
    if(NearlyEqual(BMath::Vec3LengthSquared(body.Velocity), 0.0f))
        return {0.0f, 0.0f, 0.0f};
    BMath::Vec3 frictionDirection = Vec3Normalize(body.Velocity) * -1.0f;
    BMath::Vec3 frictionForce = frictionDirection * frictionValue;

    return frictionForce;
}
BMath::Vec3 GenerateGravitationalForce(BBody& a, BBody b, f32 G)
{
    BMath::Vec3 d = b.Position - a.Position;
    f32 distanceSquared = BMath::Vec3LengthSquared(d);
    if(NearlyEqual(distanceSquared, 0.0f))
    {
        return BMath::Vec3(0.0f);
    }
    f32 attractionMagintude = G * (a.Mass * b.Mass) / distanceSquared;
    BMath::Vec3 attractionDirection = Vec3Normalize(d);
    BMath::Vec3 gravitationalForce = attractionDirection * attractionMagintude  ;


    return gravitationalForce;

}
// k -> Spring stiff constant == how much force do we need to deform the object
// l -> displacement after deformation
BMath::Vec3 GenerateSpringForce(BBody& body, BMath::Vec3& anchor,f32 restLength, f32 k)
{
    BMath::Vec3 d = body.Position - anchor;
    f32 displacement = BMath::Vec3Length(d) - restLength;

    BMath::Vec3 springDirection = Vec3Normalize(d);
    f32 springForceMagnitude = -k * displacement;
    BMath::Vec3 springForce = springDirection * springForceMagnitude;
    return springForce;
}
void EnableWeight(BBody& body, f32 gravity)
{
    BMath::Vec3 weightForce(0.0, body.Mass * gravity, 0.0f);
    
    AddForce(body, weightForce);
}
void LinearIntegrate(BBody& body, f32 deltaTime)
{
    if(NearlyEqual(body.InvMass, 0.0f))
    {
        return;
    }
    body.Acceleration = body.SumForces * body.InvMass;
    body.Velocity += body.Acceleration * deltaTime;
    body.Position += body.Velocity * deltaTime;

    // Clearing All Forces after Integration
    body.SumForces = (0.0f);
}

////////////////////////////////////////////////////////
// Angural Movement
////////////////////////////////////////////////////////
void AddTorque(BBody& body, f32 torque)
{
    body.SumTorques += torque;
}
void AngularIntegrate(BBody& body, f32 deltaTime)
{
    if(NearlyEqual(body.InvInertia, 0.0f))
    {
        return;
    }
    body.AngularAcceleration = body.SumTorques * body.InvInertia;
    body.AngularVelocity += body.AngularAcceleration * deltaTime;
    body.Rotation += body.AngularVelocity * deltaTime;
}
}
