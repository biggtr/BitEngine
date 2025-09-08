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
    else if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_POLYGON)
    {
        return IsPolygonPolygonColliding(a, b, contact);
    }
    else if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_CIRCLE)
    {
        return IsPolygonCircleColliding(a, b, contact);
    }
    else if(shapeA.Type == SHAPE_CIRCLE && shapeB.Type == SHAPE_POLYGON)
    {
        b8 result = IsPolygonCircleColliding(b, a, contact);
        if(result)
        {
            BBody* temp = contact.a;
            contact.a = contact.b;
            contact.b = temp;
            contact.Normal *= -1.0f;

        }
        return result;
    }
    return false;
}
b8 IsCircleCircleColliding(BBody* a, BBody* b, Contact& contact)
{
    BShape shapeA = GetShape(a);
    BShape shapeB = GetShape(b);
    BMath::Vec3 distanceAB = b->Position - a->Position;
    f32 distance = BMath::Vec3Length(distanceAB);
    f32 sumRadius = shapeA.BCircle.Radius + shapeB.BCircle.Radius;
    if(distance > sumRadius)
        return false;
    contact.a = a;
    contact.b = b;
    if(distance < 0.001f) 
    {
        contact.Normal = BMath::Vec3(1.0f, 0.0f, 0.0f);
    }
    else 
    {
        contact.Normal = BMath::Vec3Normalize(distanceAB);
    }
    contact.Depth = sumRadius - distance;
    contact.Start = a->Position + (contact.Normal * shapeA.BCircle.Radius);
    contact.End = b->Position - (contact.Normal * shapeB.BCircle.Radius);
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
f32 FindMinSeperation(BPolygonShape& a, BPolygonShape& b, BMath::Vec3& bestAxis, BMath::Vec3& bestPoint)
{
    f32 minSeperation = -B_INFINITY;

    for(u32 i = 0; i < a.VertexCount; ++i)
    {
        BMath::Vec3 vertex1 = a.Vertices[i];
        BMath::Vec3 vertex2 = a.Vertices[(i + 1) % a.VertexCount];
        BMath::Vec3 edge = vertex2 - vertex1;
        BMath::Vec3 normal = Vec3Normal2D(edge);
        f32 minProjection = B_INFINITY;
        BMath::Vec3 minVertex;
        for(u32 j = 0; j < b.VertexCount; ++j)
        {
            BMath::Vec3 vb = b.Vertices[j];
            f32 projection = BMath::Vec3Dot(vb - vertex1, normal);
            if(projection < minProjection)
            {
                minProjection = projection;
                minVertex = vb;
            }
        }
        if(minProjection > minSeperation)
        {
            minSeperation = minProjection;
            bestAxis = normal;
            bestPoint = minVertex;
        }
        if(minProjection > 0.0f)
            return minProjection;
    }
    return minSeperation;
}
b8 IsPolygonPolygonColliding(BBody* a, BBody* b, Contact& contact)
{
    BPolygonShape& shapeA = GetShape(a).BPolygon;
    BPolygonShape& shapeB = GetShape(b).BPolygon;

    BMath::Vec3 axisA, axisB;
    BMath::Vec3 pointA, pointB;
    f32 seperationAB = FindMinSeperation(shapeA, shapeB, axisA, pointA);
    if(seperationAB >= 0)
    {
        return false; // no collision
    }
    f32 seperationBA = FindMinSeperation(shapeB, shapeA, axisB, pointB);
    if(seperationBA >= 0)
    {
        return false; // no collision
    }
    if(seperationAB > seperationBA)
    {
        contact.a = a;
        contact.b = b;
        contact.Depth = -seperationAB;
        contact.Normal = Vec3Normalize(axisA);
        contact.Start = pointA;
        contact.End = pointA + contact.Normal * contact.Depth;
        return true;
    }
    else
    {
        contact.a = b;
        contact.b = a;
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

    f32 totalInvMass = contact.a->InvMass + contact.b->InvMass;
    if(totalInvMass < 0.001f)
        return;

    
    f32 displacementA = contact.Depth * (contact.a->InvMass / totalInvMass);
    f32 displacementB = contact.Depth * (contact.b->InvMass / totalInvMass); 
    contact.a->Position -= contact.Normal * displacementA;
    contact.b->Position += contact.Normal * displacementB;
}
void ResolveCollision(Contact& contact)
{
    if(contact.Depth < 0.01f)
        return;
    ResolvePenetration(contact);
        
    f32 e = fmin(contact.a->Restitution, contact.b->Restitution);

    // normal isnt caluclated untill theres a collision so if a is inside b and a on the left and the start point is inside b on the right
    // the normal is pointing from a to b which means from right to left == -1 -> so relativeVelocity -> .  n <-
    // velocityAlongNormal if negative means they collide and we resolve it cuz they was going towards each other
    BMath::Vec3 relativeVelocity = contact.b->Velocity - contact.a->Velocity;
    f32 velocityAlongNormal = BMath::Vec3Dot(relativeVelocity, contact.Normal);
    if(velocityAlongNormal > -0.01f) // if velocity is positive its seperating going in same dir of normal
        return;

    f32 totalInvMass = contact.a->InvMass + contact.b->InvMass;
    if(totalInvMass < 0.001f)
        return;
    BMath::Vec3 impulseDirection = contact.Normal;
    f32 impulseMagnitude = -(1 + e) * velocityAlongNormal / totalInvMass; 

    BMath::Vec3 impulse = impulseDirection * impulseMagnitude;

    ApplyImpulse(*contact.a, impulse * -1.0f);
    ApplyImpulse(*contact.b, impulse); 
}
}
