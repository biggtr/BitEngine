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
                if( m_EntityManager->HasComponent<Circle2DColliderComponent>(entityA) && 
                        m_EntityManager->HasComponent<Circle2DColliderComponent>(entityB) &&
                        m_EntityManager->HasComponent<Rigid2DBodyComponent>(entityA) &&
                        m_EntityManager->HasComponent<Rigid2DBodyComponent>(entityB)
                )
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
                        BPhysics2D::ResolveCollision(contact);
                        // transformA.Position = contact.a->Position;
                        // transformB.Position = contact.b->Position;
                    }
                }
            }
        }
        
    }

};
}
