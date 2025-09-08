#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysicsTypes.h"

namespace BPhysics2D
{

struct Contact
{
    BBody* a;
    BBody* b;

    BMath::Vec3 Start;
    BMath::Vec3 End;

    BMath::Vec3 Normal;
    f32 Depth;
};

b8 IsColliding(BBody* a, BBody* b, Contact& contact);
b8 IsCircleCircleColliding(BBody* a, BBody* b, Contact& constact);
b8 IsPolygonCircleColliding(BBody* a, BBody* b, Contact& constact);
b8 IsAABBColliding(BBody* a, BBody* b);
b8 IsPolygonPolygonColliding(BBody* a, BBody* b, Contact& contact);
f32 FindMinSeperation(BPolygonShape& a, BPolygonShape& b, BMath::Vec3& bestAxis, BMath::Vec3& bestPoint);


void ResolvePenetration(Contact& contact);
void ResolveCollision(Contact& contact);
    
}
