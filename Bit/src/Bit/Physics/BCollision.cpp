#include "BCollision.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"

namespace BPhysics2D
{
    

b8 IsColliding(BBody* a, BBody* b, Contact& contact)
{
    BShape shapeA = GetShape(a);
    BShape shapeB = GetShape(b);
    if(shapeA.Type == SHAPE_CIRCLE && shapeB.Type == SHAPE_CIRCLE)
    {
        return IsCircleCircleColliding(a, b, contact);
    }
    return false;
}
b8 IsCircleCircleColliding(BBody* a, BBody* b, Contact& contact)
{
    BShape shapeA = GetShape(a);
    BShape shapeB = GetShape(b);
    BMath::Vec3 distanceAB = b->Position - a->Position;
    f32 sumRadius = shapeA.BCircle.Radius + shapeB.BCircle.Radius;
    b8 isColliding = BMath::Vec3::LengthSquared(distanceAB) <= (sumRadius * sumRadius);
    if(!isColliding)
        return false;
    contact.a = a;
    contact.b = b;
    contact.Normal = BMath::Vec3::Normalize(distanceAB);
    contact.Start = b->Position - (contact.Normal * shapeB.BCircle.Radius);
    contact.End = a->Position + (contact.Normal * shapeA.BCircle.Radius);
    contact.Depth = BMath::Vec3::Length(contact.End - contact.Start);
    return true;
}

void ResolvePenetration(Contact& contact)
{
    if(NearlyEqual(contact.a->InvMass, 0.0f) && NearlyEqual(contact.b->InvMass, 0.0f)) 
        return;

    f32 displacementA = contact.Depth / (contact.a->InvMass + contact.b->InvMass) * contact.a->InvMass; // the bigger the mass of a the bigger the displacement in b
    f32 displacementB = contact.Depth / (contact.a->InvMass + contact.b->InvMass) * contact.b->InvMass; 
    contact.a->Position -= contact.Normal * displacementA;
    contact.b->Position += contact.Normal * displacementB;
}
}
