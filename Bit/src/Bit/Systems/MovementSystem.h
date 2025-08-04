#pragma  once 
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/System.h"
namespace BitEngine 
{

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<CRigidBody>();
        RequireComponent<CTransform>();
    }
    SYSTEM_CLASS_TYPE(MOVEMENT);

    void Update(float deltaTime)
    {
        for(const Entity& entity : m_Entities)
        {
            const CRigidBody& rigidBody = m_EntityManager->GetComponent<CRigidBody>(entity);
            CTransform& transform = m_EntityManager->GetComponent<CTransform>(entity);
            transform.Position.x += rigidBody.Velocity.x * deltaTime;
            transform.Position.y += rigidBody.Velocity.y * deltaTime;
        }
    }

};
}
