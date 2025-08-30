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

void ResolvePenetration(Contact& contact);
void ResolveCollision(Contact& contact);
    
}
