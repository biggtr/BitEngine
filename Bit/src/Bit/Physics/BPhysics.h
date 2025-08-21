#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "BPhysicsTypes.h"
#include <unordered_map>
#include <vector>

namespace BPhysics
{

struct BPhysics2DState 
{
    std::vector<BParticle> Particles;
    std::vector<BBody> Bodies;
    std::vector<BShape> Shapes;
};
class BPhysics2D
{

     
public:
    BPhysics2D();

    static b8 Initialize();
    static void Shutdown();

    static u32 BCreateCircleShape(f32 radius);
    static u32 BCreateBoxShape(f32 width, f32 height);
    static u32 CreateBody();
    static BParticle* CreateParticle();

    

    static void AddForce(BParticle& particle, BMath::Vec3& force);
    static BMath::Vec3 GenerateDragForce(BParticle particle, f32 dragValue);
    static BMath::Vec3 GenerateFrictionForce(BParticle particle, f32 frictionValue);
    static BMath::Vec3 GenerateGravitationalForce(BParticle a, BParticle b, f32 G);
    static BMath::Vec3 GenerateSpringForce(BParticle particle, BMath::Vec3& anchor,f32 restLength, f32 k);
    static void EnableWeight(BParticle& particle, f32 gravity);
    static void LinearIntegrate(BParticle particle, f32 deltaTime);

    static void AddForce(BBody& body, BMath::Vec3& force);
    static BMath::Vec3 GenerateDragForce(BBody body, f32 dragValue);
    static BMath::Vec3 GenerateFrictionForce(BBody body, f32 frictionValue);
    static BMath::Vec3 GenerateGravitationalForce(BBody a, BBody b, f32 G);
    static BMath::Vec3 GenerateSpringForce(BBody body, BMath::Vec3& anchor,f32 restLength, f32 k);
    static void EnableWeight(BBody& body, f32 gravity);
    static void LinearIntegrate(BBody body, f32 deltaTime);

    static void AngularIntegrate(BBody particle, f32 deltaTime);
private:

};

}
