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
                b8 aHasCircle = m_EntityManager->HasComponent<Circle2DColliderComponent>(entityA); 
                b8 bHasCircle = m_EntityManager->HasComponent<Circle2DColliderComponent>(entityB); 
                b8 aHasBox= m_EntityManager->HasComponent<Box2DColliderComponent>(entityA); 
                b8 bHasBox= m_EntityManager->HasComponent<Box2DColliderComponent>(entityB); 
                if( 
                        m_EntityManager->HasComponent<Rigid2DBodyComponent>(entityA) &&
                        m_EntityManager->HasComponent<Rigid2DBodyComponent>(entityB) 
                )
                {
                    if((aHasCircle || bHasCircle) || (aHasBox || bHasBox))
                    {
                        Rigid2DBodyComponent& rigidBodyA = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entityA);
                        Rigid2DBodyComponent& rigidBodyB = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entityB);
                        BPhysics2D::BBody& bodyA = BPhysics2D::GetBody(rigidBodyA.BodyIndex);
                        BPhysics2D::BBody& bodyB = BPhysics2D::GetBody(rigidBodyB.BodyIndex);
                        BPhysics2D::Contact contact;
                        if(BPhysics2D::IsColliding(&bodyA, &bodyB, contact))
                        {
                            BPhysics2D::ResolveCollision(contact);
                        }
                    }
                }
                
                
            }
        }
        
    }

};
}
