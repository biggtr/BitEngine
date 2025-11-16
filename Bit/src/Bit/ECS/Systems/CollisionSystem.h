#pragma  once 
#include "Bit/Core/Logger.h"
#include "Bit/Physics/BCollision.h"
#include "Bit/Physics/BPhysics.h"
#include "Bit/Physics/BPhysicsTypes.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "System.h"
namespace BitEngine 
{

struct EntityContact
{
    Entity entityA;
    Entity entityB;
    BPhysics2DContact contact;
};       
class CollisionSystem : public System
{
private:
    std::vector<EntityContact> m_EntityContacts;
public:

    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<Rigid2DBodyComponent>();
    }
    SYSTEM_CLASS_TYPE(COLLISION);

    void Update()
    {
        m_EntityContacts.clear(); 
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
                        BBody& bodyA = BPhysics2DGetBody(rigidBodyA.BodyIndex);
                        BBody& bodyB = BPhysics2DGetBody(rigidBodyB.BodyIndex);
                        if(bodyA.BodyType == BODY_STATIC && bodyB.BodyType == BODY_STATIC)
                            continue;
                        
                        BPhysics2DContact contact;
                        if(BPhysics2DIsColliding(&bodyA, &bodyB, contact))
                        {
                            if(bodyA.BodyType == BODY_DYNAMIC && bodyB.BodyType == BODY_DYNAMIC)
                            {
                                BPhysics2DResolveCollision(contact);
                            }
                            EntityContact entityContact;
                            entityContact.entityA = entityA;
                            entityContact.entityB = entityB;
                            entityContact.contact = contact;
                            m_EntityContacts.push_back(entityContact);
                        }
                    }
                }
                
                
            }
        }
        
    }
    std::vector<EntityContact>& GetContacts() { return m_EntityContacts; }


};
}
