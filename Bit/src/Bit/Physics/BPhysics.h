#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "BPhysicsTypes.h"

namespace BPhysics
{

class BPhysics2D
{
public:
    BPhysics2D();

    static void AddForce(BParticle particle, BMath::Vec3& force);
    static BMath::Vec3 GenerateDragForce(BParticle particle, f32 dragValue);
    static BMath::Vec3 GenerateFrictionForce(BParticle particle, f32 frictionValue);
    static BMath::Vec3 GenerateGravitationalForce(BParticle a, BParticle b, f32 G);
    static BMath::Vec3 GenerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k);
    static void EnableWeight(BParticle particle, f32 gravity);
    static void LinearIntegrate(BParticle particle, f32 deltaTime);
    static void AngularIntegrate(BBody particle, f32 deltaTime);
private:

};

}
