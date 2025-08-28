#pragma  once 
#include "Bit/Core/Logger.h"
#include "Bit/Physics/BCollision.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/System.h"
namespace BitEngine 
{

class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<Circle2DColliderComponent>();
        RequireComponent<Rigid2DBodyComponent>();
    }
    SYSTEM_CLASS_TYPE(COLLISION);

    void Update()
    {
        for(u32 i = 0; i < m_Entities.size(); ++i)
        {
            for(u32 j = i + 1; j < m_Entities.size(); ++j)
            {
                const Entity& entityA = m_Entities[i];
                const Entity& entityB = m_Entities[j];
                if(m_EntityManager->HasComponent<Circle2DColliderComponent>(entityA))
                {
                    Rigid2DBodyComponent& rigidBodyA = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entityA);
                    TransformComponent& transformA = m_EntityManager->GetComponent<TransformComponent>(entityA);
                    Rigid2DBodyComponent& rigidBodyB = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entityB);
                    TransformComponent& transformB = m_EntityManager->GetComponent<TransformComponent>(entityB);
                    BPhysics2D::BBody& bodyA = BPhysics2D::GetBody(rigidBodyA.BodyIndex);
                    BPhysics2D::BBody& bodyB = BPhysics2D::GetBody(rigidBodyB.BodyIndex);

                    BPhysics2D::Contact contact;
                    if(BPhysics2D::IsColliding(&bodyA, &bodyB, contact))
                    {
                        BIT_LOG_DEBUG("Circle Colliding");
                        BPhysics2D::ResolvePenetration(contact);
                        transformA.Position = contact.a->Position;
                        transformB.Position = contact.b->Position;
                    }
                }
            }
        }
        
    }
private:
    // b8 IsOverlappingAABB(Entity& A, Entity& B)
    // {
    //     CTransform& ATransform = m_EntityManager->GetComponent<CTransform>(A);
    //     CTransform& BTransform = m_EntityManager->GetComponent<CTransform>(B);
    //     CBoxCollider& ABoxCollider = m_EntityManager->GetComponent<CBoxCollider>(A);
    //     CBoxCollider& BBoxCollider = m_EntityManager->GetComponent<CBoxCollider>(B);
    //     float ALeft = ATransform.Position.x + ABoxCollider.Offset.x;
    //     float ARight = ALeft + ABoxCollider.Width;
    //     float BLeft = BTransform.Position.x + ABoxCollider.Offset.x;
    //     float BRight = BLeft + BBoxCollider.Width;
    //
    //     float ABot = ATransform.Position.y + ABoxCollider.Offset.y;
    //     float ATop = ABot + ABoxCollider.Height;
    //     float BBot = BTransform.Position.y + ABoxCollider.Offset.y;
    //     float BTop = BBot + BBoxCollider.Height;
    //
    //     b8 isOverlappingX = ARight >= BLeft && ALeft <= BRight;
    //     b8 isOverlappingY = ABot <= BTop && ATop >= BBot;
    //     return isOverlappingX && isOverlappingY;
    // }

};
}
