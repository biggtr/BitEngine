#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysicsTypes.h"

namespace BitEngine
{


struct BPhysics2DContact
{
    BBody* a;
    BBody* b;

    BMath::Vec3 Start;
    BMath::Vec3 End;

    BMath::Vec3 Normal;
    f32 Depth;
};

b8 BPhysics2DIsColliding(BBody* a, BBody* b, BPhysics2DContact& contact);
b8 BPhysics2DIsCircleCircleColliding(BBody* a, BBody* b, BPhysics2DContact& constact);
b8 BPhysics2DIsPolygonCircleColliding(BBody* a, BBody* b, BPhysics2DContact& constact);
b8 BPhysics2DIsAABBColliding(BBody* a, BBody* b);
b8 BPhysics2DIsPolygonPolygonColliding(BBody* a, BBody* b, BPhysics2DContact& contact);
f32 BPhysics2DFindMinSeperation(BPolygonShape& a, BPolygonShape& b, BMath::Vec3& bestAxis, BMath::Vec3& bestPoint);


void BPhysics2DResolvePenetration(BPhysics2DContact& contact);
void BPhysics2DResolveCollision(BPhysics2DContact& contact);
    
}
