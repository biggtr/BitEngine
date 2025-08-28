#include "System.h"
#include "Bit/Core/Event.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Entity.h"
#include <algorithm>



namespace BitEngine
{
    System::System() :
        m_ComponentSignature(0),
        m_SystemType(SYSTEM_TYPE::COUNT),
        m_EntityManager(nullptr),
        m_Entities{}
    {
    }

    void System::OnEntityAdded(const Entity& entity)
    {
        BIT_LOG_DEBUG("OnEntityAdded fired for system=%p, entityID=%u", this, entity.GetID());
    }
    void System::SetEntityManager(EntityManager* entityManager)
    {
        m_EntityManager = entityManager;
    }
    void System::AddEntity(const Entity& entity)
    {
        m_Entities.push_back(entity);
    }
    void System::RemoveEntity(const Entity& entity)
    {
        for(size_t i = 0; i < m_Entities.size(); i++)
        {
            if(m_Entities[i].GetID() == entity.GetID())
            {
                m_Entities[i] = m_Entities.back(); //swaping with the last element to pop it 
                m_Entities.pop_back();
                return;
            }
        }
    }
    const std::vector<Entity>& System::GetEntities() const
    {
        return m_Entities;
    }

}
