#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "BPhysicsTypes.h"
#include <unordered_map>
#include <vector>

namespace BitEngine
{


b8 BPhysics2DInitialize();
void BPhysics2DShutdown();

u32 BPhysics2DCreateCircleShape(f32 radius);
u32 BPhysics2DCreateBoxShape(f32 width, f32 height);
u32 BPhysics2DCreatePolygonShape(const BMath::Vec3* vertices, u32 count, f32 inertiaWithoutMass);
u32 BPhysics2DCreateBody(u32 ShapeIndex, const BMath::Vec3& position, f32 mass, f32 restitution = 0.5f);
BShape& BPhysics2DGetShape(BBody* body);
BBody& BPhysics2DGetBody(u32 bodyIndex);
BMath::Vec3 BPhysics2DVec3EdgeAt(BPolygonShape& polygon, u32 index);

BParticle* BPhysics2DCreateParticle();

void BPhysics2DAddForce(BParticle& particle, BMath::Vec3& force);
BMath::Vec3 BPhysics2DGennerateDragForce(BParticle particle, f32 dragValue);
BMath::Vec3 BPhysics2DGennerateFrictionForce(BParticle particle, f32 frictionValue);
BMath::Vec3 BPhysics2DGennerateGravitationalForce(BParticle a, BParticle b, f32 G);
BMath::Vec3 BPhysics2DGennerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k);
void BPhysics2DEnableWeight(BParticle& particle, f32 gravity);
void BPhysics2DLinearIntegrate(BParticle particle, f32 deltaTime);

void BPhysics2DAddForce(BBody& body, const BMath::Vec3& force);
void BPhysics2DApplyImpulse(BBody& body, const BMath::Vec3& impulse);
BMath::Vec3 BPhysics2DGennerateDragForce(BBody& body, f32 dragValue);
BMath::Vec3 BPhysics2DGenerateFrictionForce(BBody& body, f32 frictionValue);
BMath::Vec3 BPhysics2DGennerateGravitationalForce(BBody& a, BBody b, f32 G);
BMath::Vec3 BPhysics2DGennerateSpringForce(BBody& body, BMath::Vec3& anchor,f32 restLength, f32 k);
void BPhysics2DEnableWeight(BBody& body, f32 gravity);
void BPhysics2DLinearIntegrate(BBody& body, f32 deltaTime);

void BPhysics2DAddTorque(BBody& body, f32 torque);
void BPhysics2DAngularIntegrate(BBody& particle, f32 deltaTime);
    

}
