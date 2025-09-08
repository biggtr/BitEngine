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
    if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_POLYGON)
    {
        return IsPolygonPolygonColliding(shapeA.BPolygon, shapeB.BPolygon, contact);
    }
    if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_CIRCLE)
    {
        return IsPolygonCircleColliding(a, b, contact);
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

b8 IsPolygonCircleColliding(BBody* polygon, BBody* circle, Contact& contact)
{
    BCircleShape circleShape = GetShape(circle).BCircle;
    BPolygonShape polygonShape = GetShape(polygon).BPolygon;
    BMath::Vec3* polygonVertices = polygonShape.Vertices;
    BMath::Vec3 minCurrentVertex;
    BMath::Vec3 minNextVertex;
    f32 distanceCircleEdge = -B_INFINITY;
    b8 isOutside = false;
    for(u32 i = 0; i < polygonShape.VertexCount; ++i)
    {
        i32 currentVertex = i;
        i32 nextVertex = (i + 1) % polygonShape.VertexCount;
        BMath::Vec3 polygonEdge = Vec3EdgeAt(polygonShape, currentVertex);
        BMath::Vec3 edgeNormal = BMath::Vec3Normal2D(polygonEdge);
        BMath::Vec3 vertexCircleCenter = circle->Position - polygonVertices[currentVertex];
        f32 projection = BMath::Vec3Dot(vertexCircleCenter, edgeNormal);
        if(projection > 0)
        {
            // circle center is outside the polygon
            
            distanceCircleEdge = projection;
            minCurrentVertex = polygonVertices[currentVertex];
            minNextVertex = polygonVertices[nextVertex];
            isOutside = true;
            break;
        }
        else 
        {
            // we inside and we need to find the min edge (least negative which means the closest edge to the circle)
            if(projection > distanceCircleEdge)
            {
                distanceCircleEdge = projection;
                minCurrentVertex = polygonVertices[currentVertex];
                minNextVertex = polygonVertices[nextVertex];
            }
        }
    }
    if(isOutside)
    {
        BMath::Vec3 v1 = circle->Position - minCurrentVertex;
        BMath::Vec3 v2 = minNextVertex - minCurrentVertex;
        f32 v1Magnitude = BMath::Vec3Length(v1);
        if(BMath::Vec3Dot(v1, v2) < 0) // we iside the a region
        {
            if( v1Magnitude > circleShape.Radius)
            {
                // we're not colliding
                return false;
            }
            else
            {
                contact.a = polygon;
                contact.b = circle;
                contact.Depth = circleShape.Radius - v1Magnitude;
                contact.Normal = BMath::Vec3Normalize(v1);
                contact.Start = circle->Position + (contact.Normal * -circleShape.Radius);
                contact.End = contact.Start + (contact.Normal * contact.Depth);
            }
        }
        else
        {
            // check if we're inside the b region
            v1 = circle->Position - minNextVertex;
            v2 = minCurrentVertex - minNextVertex;
            f32 v1Magnitude = BMath::Vec3Length(v1);
            if(BMath::Vec3Dot(v1, v2) < 0) // we're inside the b region
            {
                if( v1Magnitude > circleShape.Radius)
                {
                    // we're not colliding
                    return false;
                }
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.Depth = circleShape.Radius - v1Magnitude;
                    contact.Normal = BMath::Vec3Normalize(v1);
                    contact.Start = circle->Position + (contact.Normal * -circleShape.Radius);
                    contact.End = contact.Start + (contact.Normal * contact.Depth);
                }

            }
            else
            {
                // we're inside the c region
                if(distanceCircleEdge > circleShape.Radius)
                {
                    // we're not colliding 
                    return false;
                }
                else 
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.Depth = circleShape.Radius - distanceCircleEdge;
                    contact.Normal = BMath::Vec3Normal2D(minNextVertex - minCurrentVertex);
                    contact.Start = circle->Position - (contact.Normal * circleShape.Radius);
                    contact.End = contact.Start + (contact.Normal * contact.Depth);
                }

            }

        }
    }
    else
    {
        // circle center is inside the rectangle itself
        contact.a = polygon;
        contact.b = circle;
        contact.Depth = circleShape.Radius - distanceCircleEdge;
        contact.Normal = BMath::Vec3Normal2D(minNextVertex - minCurrentVertex);
        contact.Start = circle->Position - (contact.Normal * circleShape.Radius);
        contact.End = contact.Start + (contact.Normal * contact.Depth);

    }




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
    if(seperationAB >= 0)
    {
        return false; // no collision
    }
    f32 seperationBA = FindMinSeperation(b, a, axisB, pointB);
    if(seperationBA >= 0)
    {
        return false; // no collision
    }
    if(seperationAB > seperationBA)
    {
        contact.Depth = -seperationAB;
        contact.Normal = Vec3Normalize(axisA);
        contact.Start = pointA;
        contact.End = pointA + contact.Normal * contact.Depth;
        return true;
    }
    else
    {
        contact.Depth = -seperationBA;
        contact.Normal = Vec3Normalize(axisB);
        contact.Start = pointB;
        contact.End = pointB - contact.Normal * contact.Depth;
        return true;
    }
    return false;
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

    // normal isnt caluclated untill theres a collision so if a is inside b and a on the left and the start point is inside b on the right
    // the normal is pointing from a to b which means from right to left == -1 -> so relativeVelocity -> .  n <-
    // velocityAlongNormal if negative means they collide and we resolve it cuz they was going towards each other
    BMath::Vec3 relativeVelocity = contact.a->Velocity - contact.b->Velocity;
    f32 velocityAlongNormal = BMath::Vec3Dot(relativeVelocity, contact.Normal);
    if(velocityAlongNormal > 0) // if velocity is positive its seperating going in same dir of normal
        return;
    BMath::Vec3 impulseDirection = contact.Normal;
    f32 impulseMagnitude = -(1 + e) * velocityAlongNormal / (contact.a->InvMass + contact.b->InvMass);

    BMath::Vec3 impulse = impulseDirection * impulseMagnitude;

    ApplyImpulse(*contact.a, impulse);
    ApplyImpulse(*contact.b, impulse * -1.0f); 
}
}
