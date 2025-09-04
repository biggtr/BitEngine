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
    b8 isColliding = BMath::Vec3LengthSquared(distanceAB) <= (sumRadius * sumRadius);
    if(!isColliding)
        return false;
    contact.a = a;
    contact.b = b;
    contact.Normal = BMath::Vec3Normalize(distanceAB);
    contact.Start = b->Position - (contact.Normal * shapeB.BCircle.Radius);
    contact.End = a->Position + (contact.Normal * shapeA.BCircle.Radius);
    contact.Depth = BMath::Vec3Length(contact.End - contact.Start);
    return true;
}

b8 IsAABBColliding(BBody* a, BBody* b)
{
    BShape shapeA = GetShape(a);
    BShape shapeB = GetShape(b);

    f32 aLeft = -shapeA.BBox.Width; 
    f32 aRight = shapeA.BBox.Width;
    f32 aTop = shapeA.BBox.Height; 
    f32 aBot= -shapeA.BBox.Height;

    f32 bLeft = -shapeB.BBox.Width; 
    f32 bRight = shapeB.BBox.Width;
    f32 bTop = shapeB.BBox.Height; 
    f32 bBot= -shapeB.BBox.Height;

    return aRight >= bLeft && aLeft <= bRight && aBot <= bTop && aTop >= bBot;
}
f32 FindMinSeperation(BPolygonShape& a, BPolygonShape& b, BMath::Vec3& bestAxis, BMath::Vec3 bestPoint)
{
    f32 seperation = -B_INFINITY;

    for(u32 i = 0; i < a.VertexCount; ++i)
    {
        BMath::Vec3 va = Vec3EdgeAt(a, i);
        BMath::Vec3 normal = Vec3Normal2D(va);
        f32 minSeperation = B_INFINITY;
        BMath::Vec3 minVertex;
        for(u32 j = 0; j < b.VertexCount; ++j)
        {
            BMath::Vec3 vb = b.Vertices[j];
            f32 projection = BMath::Vec3Dot(vb - va, normal);
            if(projection < minSeperation)
            {
                minSeperation = projection;
                minVertex = vb;
            }
        }
        if(seperation > minSeperation)
        {
            seperation = minSeperation;
            bestAxis = va;
            bestPoint = minVertex;
        }
    }
    return seperation;
}
b8 IsPolygonPolygonColliding(BPolygonShape& a, BPolygonShape& b, Contact& contact)
{
    BMath::Vec3 axisA, axisB;
    BMath::Vec3 pointA, pointB;
    f32 seperationAB = FindMinSeperation(a, b, axisA, pointA);
    f32 seperationBA = FindMinSeperation(b, a, axisB, pointB);
    if(seperationAB > seperationBA)
    {
        contact.Depth = -seperationAB;
        contact.Normal = Vec3Normalize(axisA);
        contact.Start = pointA;
        contact.End = pointA + contact.Normal * contact.Depth;
    }
    else
    {
        contact.Depth = -seperationBA;
        contact.Normal = Vec3Normalize(axisB);
        contact.Start = pointB;
        contact.End = pointB - contact.Normal * contact.Depth;
    }
    return true; // colliding 
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
void ResolveCollision(Contact& contact)
{
    ResolvePenetration(contact);
        
    f32 e = fmin(contact.a->Restitution, contact.b->Restitution);

    BMath::Vec3 relativeVelocity = contact.b->Velocity - contact.a->Velocity;
    f32 velocityAlongNormal = BMath::Vec3Dot(relativeVelocity, contact.Normal);
    if(velocityAlongNormal > 0) // if velocity is positive its seperating going in same dir of normal
        return;
    BMath::Vec3 impulseDirection = contact.Normal;
    f32 impulseMagnitude = -(1 + e) * velocityAlongNormal / (contact.a->InvMass + contact.b->InvMass);

    BMath::Vec3 impulse = impulseDirection * impulseMagnitude;

    ApplyImpulse(*contact.a, impulse * -1.0f);
    ApplyImpulse(*contact.b, impulse); 
}
}
