#pragma  once 
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
        RequireComponent<CTransform>();
        RequireComponent<CBoxCollider>();
    }
    SYSTEM_CLASS_TYPE(COLLISION);

    void Update()
    {
        for(auto a = m_Entities.begin(); a != m_Entities.end(); ++a)
        {
            for(auto b = a; b != m_Entities.end(); ++b)
            {
                if((*a) == (*b))
                {
                    continue;
                }
                IsOverlappingAABB(*a, *b);
            }
        }
    }
private:
    b8 IsOverlappingAABB(Entity& A, Entity& B)
    {
        CTransform& ATransform = m_EntityManager->GetComponent<CTransform>(A);
        CTransform& BTransform = m_EntityManager->GetComponent<CTransform>(B);
        CBoxCollider& ABoxCollider = m_EntityManager->GetComponent<CBoxCollider>(A);
        CBoxCollider& BBoxCollider = m_EntityManager->GetComponent<CBoxCollider>(B);
        float ALeft = ATransform.Position.x + ABoxCollider.Offset.x;
        float ARight = ALeft + ABoxCollider.Width;
        float BLeft = BTransform.Position.x + ABoxCollider.Offset.x;
        float BRight = BLeft + BBoxCollider.Width;

        float ABot = ATransform.Position.y + ABoxCollider.Offset.y;
        float ATop = ABot + ABoxCollider.Height;
        float BBot = BTransform.Position.y + ABoxCollider.Offset.y;
        float BTop = BBot + BBoxCollider.Height;

        b8 isOverlappingX = ARight >= BLeft && ALeft <= BRight;
        b8 isOverlappingY = ABot <= BTop && ATop >= BBot;
        return isOverlappingX && isOverlappingY;
    }

};
}
