#include "BPhysics.h"
#include "Bit/Math/Vector.h"

namespace BPhysics
{

void BPhysics2D::AddForce(CParticle particle, BMath::Vec3& force)
{
    particle.SumForces += force;
}
BMath::Vec3 BPhysics2D::GenerateDragForce(CParticle particle, f32 dragValue)
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
BMath::Vec3 BPhysics2D::GenerateFrictionForce(CParticle particle, f32 frictionValue)
{
    //frictionValue -> CoeffOfSurfaceFriction * Magnitude if normal force of the surface on the particle
    BMath::Vec3 frictionDirection = particle.Velocity.Normalize() * -1.0f;
    BMath::Vec3 frictionForce = frictionDirection * frictionValue;

    return frictionForce;
}
BMath::Vec3 BPhysics2D::GenerateGravitationalForce(CParticle a, CParticle b, f32 G)
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
void BPhysics2D::EnableWeight(CParticle particle, f32 gravity)
{
    BMath::Vec3 weightForce(0.0, particle.Mass * gravity, 0.0f);

    AddForce(particle, weightForce);
}
void BPhysics2D::Integrate(CParticle particle, f32 deltaTime)
{
    if(particle.Mass == 0)
    {
        return;
    }
    particle.Acceleration = particle.SumForces * particle.InverseMass;
    particle.Velocity += particle.Acceleration * deltaTime;
    particle.Position += particle.Velocity * deltaTime;

    // Clearing All Forces after Integration
    particle.SumForces = (0.0f);
}

}
