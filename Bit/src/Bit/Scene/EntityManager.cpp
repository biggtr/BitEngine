#include "Bit/Scene/EntityManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <utility>
namespace BitEngine
{

Entity EntityManager::CreateEntity()
{
    u32 entityID{};
    if(m_FreeIDs.IsEmpty())
    {
        entityID = m_NumOfEntities++;
        if(entityID >= m_EntitiesSignatures.size())
        {
            m_EntitiesSignatures.resize(entityID + 1);
        }
    }
    else 
    {
        entityID = m_FreeIDs.Pop();
    }


    Entity newEntity(entityID);
    newEntity.m_EntityManager = this;

    m_EntitiesToAdd.push_back(newEntity);

    return newEntity;
}
void EntityManager::KillEntity(const Entity& entity)
{
    m_EntitiesToKill.push_back(entity);
}

void EntityManager::AddEntityToSystems(const Entity &entity) const 
{

    uint32_t entityID = entity.GetID();
    const Signature& entitySignature = m_EntitiesSignatures[entityID];

    for(System* system : m_Systems)
    {

        if(!system) continue;
        const Signature& systemSignature = system->GetComponentSignature();
        if((systemSignature & entitySignature) == systemSignature)
        {
            system->AddEntity(entity);
        }
    }
}

void EntityManager::RemoveEntityFromSystems(const Entity &entity) const 
{

    uint32_t entityID = entity.GetID();
    const Signature& entitySignature = m_EntitiesSignatures[entityID];

    for(System* system : m_Systems)
    {

        if(!system) continue;
        const Signature& systemSignature = system->GetComponentSignature();
        if((systemSignature & entitySignature) == systemSignature)
        {
            system->RemoveEntity(entity);
        }
    }
}

void EntityManager::Update()
{
    for(const Entity& entity : m_EntitiesToAdd)
    {
        AddEntityToSystems(entity);
    }
    m_EntitiesToAdd.clear();
    for(const Entity& entity : m_EntitiesToKill)
    {
        RemoveEntityFromSystems(entity);
        m_EntitiesSignatures[entity.GetID()] = 0;
        m_FreeIDs.PushBack(entity.GetID());
    }
    m_EntitiesToKill.clear();
}

} 
