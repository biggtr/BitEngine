#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "PhysicsTypes.h"

namespace BPhysics
{

class BPhysics2D
{
public:
    BPhysics2D();

    static void AddForce(CParticle particle, BMath::Vec3& force);
    static BMath::Vec3 GenerateDragForce(CParticle particle, f32 dragValue);
    BMath::Vec3 GenerateFrictionForce(CParticle particle, f32 frictionValue);
    BMath::Vec3 GenerateGravitationalForce(CParticle a, CParticle b, f32 G);
    BMath::Vec3 GenerateSpringForce(CParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k);
    static void EnableWeight(CParticle particle, f32 gravity);
    static void Integrate(CParticle particle, f32 deltaTime);
private:

};

}
