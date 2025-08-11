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
    uint32_t entityID = m_NumOfEntities++;

    Entity newEntity(entityID);
    newEntity.m_EntityManager = this;

    m_EntitiesToAdd.push_back(newEntity);
    if(entityID >= m_EntitiesSignatures.size())
    {
        m_EntitiesSignatures.resize(entityID + 1);
    }

    // BIT_LOG_DEBUG("New Entity with id: %d Got Added..", entityID);
    return newEntity;
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

void EntityManager::Update()
{
    for(const Entity& entity : m_EntitiesToAdd)
    {
        AddEntityToSystems(entity);
    }
    m_EntitiesToAdd.clear();
}

} 
