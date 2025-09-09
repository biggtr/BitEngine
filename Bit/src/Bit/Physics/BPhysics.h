#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "BPhysicsTypes.h"
#include <unordered_map>
#include <vector>

namespace BPhysics2D
{
struct BPhysics2DState 
{
    std::vector<BParticle> Particles;
    std::vector<BBody> Bodies;
    std::vector<BShape> Shapes;
};


b8 Initialize();
void Shutdown();

u32 BCreateCircleShape(f32 radius);
u32 BCreateBoxShape(f32 width, f32 height);
u32 BCreatePolygonShape(const BMath::Vec3* vertices, u32 count, f32 inertiaWithoutMass);
u32 CreateBody(u32 ShapeIndex, const BMath::Vec3& position, f32 mass, f32 restitution = 0.5f);
BShape& GetShape(BBody* body);
BBody& GetBody(u32 bodyIndex);
BMath::Vec3 Vec3EdgeAt(BPolygonShape& polygon, u32 index);

BParticle* CreateParticle();

void AddForce(BParticle& particle, BMath::Vec3& force);
BMath::Vec3 GenerateDragForce(BParticle particle, f32 dragValue);
BMath::Vec3 GenerateFrictionForce(BParticle particle, f32 frictionValue);
BMath::Vec3 GenerateGravitationalForce(BParticle a, BParticle b, f32 G);
BMath::Vec3 GenerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k);
void EnableWeight(BParticle& particle, f32 gravity);
void LinearIntegrate(BParticle particle, f32 deltaTime);

void AddForce(BBody& body, const BMath::Vec3& force);
void ApplyImpulse(BBody& body, const BMath::Vec3& impulse);
BMath::Vec3 GenerateDragForce(BBody& body, f32 dragValue);
BMath::Vec3 GenerateFrictionForce(BBody& body, f32 frictionValue);
BMath::Vec3 GenerateGravitationalForce(BBody& a, BBody b, f32 G);
BMath::Vec3 GenerateSpringForce(BBody& body, BMath::Vec3& anchor,f32 restLength, f32 k);
void EnableWeight(BBody& body, f32 gravity);
void LinearIntegrate(BBody& body, f32 deltaTime);

void AddTorque(BBody& body, f32 torque);
void AngularIntegrate(BBody& particle, f32 deltaTime);
    

}
