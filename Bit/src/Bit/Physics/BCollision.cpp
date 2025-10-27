#include "BCollision.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include <iterator>

namespace BitEngine
{


    

b8 BPhysics2DIsColliding(BBody* a, BBody* b, BPhysics2DContact& contact)
{
    BShape shapeA = BPhysics2DGetShape(a);
    BShape shapeB = BPhysics2DGetShape(b);
    if(shapeA.Type == SHAPE_CIRCLE && shapeB.Type == SHAPE_CIRCLE)
    {
        return BPhysics2DIsCircleCircleColliding(a, b, contact);
    }
    else if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_POLYGON)
    {
        return BPhysics2DIsPolygonPolygonColliding(a, b, contact);
    }
    else if(shapeA.Type == SHAPE_POLYGON && shapeB.Type == SHAPE_CIRCLE)
    {
        return BPhysics2DIsPolygonCircleColliding(a, b, contact);
    }
    else if(shapeA.Type == SHAPE_CIRCLE && shapeB.Type == SHAPE_POLYGON)
    {
        b8 result = BPhysics2DIsPolygonCircleColliding(b, a, contact);
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
b8 BPhysics2DIsPolygonCircleColliding(BBody* polygon, BBody* circle, BPhysics2DContact& contact)
{
    BCircleShape circleShape = BPhysics2DGetShape(circle).BCircle;
    BPolygonShape polygonShape = BPhysics2DGetShape(polygon).BPolygon;
    BMath::Vec3* polygonVertices = polygonShape.Vertices;
    
    f32 minSeparation = -B_INFINITY;
    BMath::Vec3 closestEdgeNormal;
    BMath::Vec3 closestVertex1, closestVertex2;
    b8 isOutside = false;
    
    for(u32 i = 0; i < polygonShape.VertexCount; ++i)
    {
        i32 currentVertex = i;
        i32 nextVertex = (i + 1) % polygonShape.VertexCount;

        BMath::Vec3 worldVertex1 = polygonVertices[currentVertex] + polygon->Position;
        BMath::Vec3 worldVertex2 = polygonVertices[nextVertex] + polygon->Position;
        BMath::Vec3 edge = worldVertex2 - worldVertex1;
        BMath::Vec3 normal = BMath::Vec3Normal2D(edge);
        normal = BMath::Vec3Normalize(normal);
        
        BMath::Vec3 toCircle = circle->Position - worldVertex1;
        f32 separation = BMath::Vec3Dot(toCircle, normal);
        
        if(separation > 0)
        {
            isOutside = true;
        }
        
        if(separation > minSeparation)
        {
            minSeparation = separation;
            closestEdgeNormal = normal;
            closestVertex1 = worldVertex1;
            closestVertex2 = worldVertex2;
        }
    }
    
    if(isOutside && minSeparation > circleShape.Radius)
    {
        return false;
    }
    
    
    BMath::Vec3 toVertex1 = circle->Position - closestVertex1;
    BMath::Vec3 toVertex2 = circle->Position - closestVertex2;
    BMath::Vec3 edgeVector = closestVertex2 - closestVertex1;
    
    BMath::Vec3 edgeDirection = BMath::Vec3Normalize(edgeVector);
    f32 projectionOnEdge = BMath::Vec3Dot(toVertex1, edgeDirection);
    
    if(projectionOnEdge < 0)
    {
        // we're inside -> A region
        f32 distToVertex = BMath::Vec3Length(toVertex1);
        if(distToVertex > circleShape.Radius)
            return false;
            
        contact.a = polygon;
        contact.b = circle;
        contact.Depth = circleShape.Radius - distToVertex;
        contact.Normal = BMath::Vec3Normalize(toVertex1);
        contact.Start = circle->Position - (contact.Normal * circleShape.Radius);
        contact.End = contact.Start + (contact.Normal * contact.Depth);
    }
    else if(projectionOnEdge > BMath::Vec3Length(edgeVector))
    {
        // we're inside -> B region
        f32 distToVertex = BMath::Vec3Length(toVertex2);
        if(distToVertex > circleShape.Radius)
            return false;
            
        contact.a = polygon;
        contact.b = circle;
        contact.Depth = circleShape.Radius - distToVertex;
        contact.Normal = BMath::Vec3Normalize(toVertex2);
        contact.Start = circle->Position - (contact.Normal * circleShape.Radius);
        contact.End = contact.Start + (contact.Normal * contact.Depth);
    }
    else
    {
        // we're inside C region
        contact.a = polygon;
        contact.b = circle;
        
        if(isOutside)
        {
            contact.Depth = circleShape.Radius - minSeparation;
            contact.Normal = closestEdgeNormal;
        }
        else
        {
            contact.Depth = circleShape.Radius + (-minSeparation); 
            contact.Normal = closestEdgeNormal;
        }
        
        contact.Start = circle->Position - (contact.Normal * circleShape.Radius);
        contact.End = contact.Start + (contact.Normal * contact.Depth);
    }
    
    return true;
}
b8 BPhysics2DIsCircleCircleColliding(BBody* a, BBody* b, BPhysics2DContact& contact)
{
    BShape shapeA = BPhysics2DGetShape(a);
    BShape shapeB = BPhysics2DGetShape(b);
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
    contact.Start = b->Position - (contact.Normal * shapeB.BCircle.Radius);
    contact.End= a->Position + (contact.Normal * shapeA.BCircle.Radius);
    return true;
}

b8 BPhysics2DIsAABBColliding(BBody* a, BBody* b)
{
    BShape shapeA = BPhysics2DGetShape(a);
    BShape shapeB = BPhysics2DGetShape(b);

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
f32 BPhysics2DFindMinSeperation(BPolygonShape& a, BPolygonShape& b, BMath::Vec3& bestAxis, BMath::Vec3& bestPoint)
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
b8 BPhysics2DIsPolygonPolygonColliding(BBody* a, BBody* b, BPhysics2DContact& contact)
{
    BPolygonShape& shapeA = BPhysics2DGetShape(a).BPolygon;
    BPolygonShape& shapeB = BPhysics2DGetShape(b).BPolygon;

    BMath::Vec3 axisA, axisB;
    BMath::Vec3 pointA, pointB;
    f32 seperationAB = BPhysics2DFindMinSeperation(shapeA, shapeB, axisA, pointA);
    if(seperationAB >= 0)
    {
        return false; 
    }
    f32 seperationBA = BPhysics2DFindMinSeperation(shapeB, shapeA, axisB, pointB);
    if(seperationBA >= 0)
    {
        return false; 
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
void BPhysics2DResolvePenetration(BPhysics2DContact& contact)
{
    if(NearlyEqual(contact.a->InvMass, 0.0f) && NearlyEqual(contact.b->InvMass, 0.0f)) 
        return;

    f32 totalInvMass = contact.a->InvMass + contact.b->InvMass;
    if(totalInvMass < 0.001f)
        return;

    f32 correction = 0.01f + contact.Depth;

    
    f32 displacementA = correction * (contact.a->InvMass / totalInvMass);
    f32 displacementB = correction * (contact.b->InvMass / totalInvMass); 
    contact.a->Position -= contact.Normal * displacementA;
    contact.b->Position += contact.Normal * displacementB;
}
void BPhysics2DResolveCollision(BPhysics2DContact& contact)
{
    if(contact.Depth < 0.01f)
        return;
    BPhysics2DResolvePenetration(contact);
        
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

    BPhysics2DApplyImpulse(*contact.a, impulse * -1.0f);
    BPhysics2DApplyImpulse(*contact.b, impulse); 
}
}
