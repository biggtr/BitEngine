#include "Bit/Scene/Entity.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Compontents.h"
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

    m_EntitiesToAdd.push_back(newEntity);

    printf("New Entity with id: %d Got Added..", entityID);
    return newEntity;
}


template<typename TComponent, typename ...TArgs> 
void EntityManager::AddComponent(const Entity& entity, TArgs&& ...args)
{
    uint32_t entityID = entity.GetID();
    uint32_t componentID = Component::Type<TComponent>();
    
    if(componentID >= m_ComponentPools.size())
    {
        m_ComponentPools.resize(componentID + 1, nullptr);
    }

    if(!m_ComponentPools[componentID])
    {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        m_ComponentPools[componentID] = newComponentPool;
    }
    Pool<TComponent>* componentPool = m_ComponentPools[componentID];
    if(entityID >= componentPool->GetSize())
    {
        componentPool->Resize(m_NumOfEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityID, newComponent);

    m_EntitiesSignatures[entityID] |= (1 << componentID);
}


template<typename TComponent> 
bool EntityManager::HasComponent(const Entity& entity)
{
    auto entityID = entity.GetID();
    return m_EntitiesSignatures[entityID] & (1 << Component::Type<TComponent>());
}
template<typename TComponent> 
void EntityManager::RemoveComponent(const Entity& entity)
{
    auto entityID = entity.GetID();
    m_EntitiesSignatures[entityID] &= ~(1 << Component::Type<TComponent>());
}


template<typename TSystem, typename ...TArgs> 
void EntityManager::AddSystem(TArgs&& ...args)
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");

    SYSTEM_TYPE type = TSystem::GetStaticType();
    if(m_Systems[(uint32_t)type])
    {
        return; //already exists
    }
    m_Systems[(uint32_t)type] = new TSystem(std::forward<TArgs>(args)...);

}
template<typename TSystem> 
void EntityManager::RemoveSystem()
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");

    SYSTEM_TYPE type = TSystem::GetStaticType();
    if(m_Systems[(uint32_t)type])
    {
        delete m_Systems[(uint32_t)type];
        m_Systems[(uint32_t)type] = nullptr;
    }
}
template<typename TSystem> 
bool EntityManager::HasSystem() const
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");
    SYSTEM_TYPE type = TSystem::GetStaticType();
    return m_Systems[(uint32_t)type] != nullptr;
}
template<typename TSystem> 
TSystem& EntityManager::GetSystem() const
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");
    SYSTEM_TYPE type = TSystem::GetStaticType();
    return *(TSystem*)m_Systems[(uint32_t)type];

}
template<typename TSystem> 
void EntityManager::AddEntityToSystem(const Entity& entity) const
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");
    uint32_t entityID = entity.GetID();
    const Signature& entitySignature = m_EntitiesSignatures[entityID];

    SYSTEM_TYPE type = TSystem::GetStaticType();
    TSystem* system = m_Systems[(uint32_t)type];
    if(!system) return;
    
    if((system->GetComponentSignature() & entitySignature) == system->GetComponentSignature)
    {
        system->AddEntity(entity);
    }
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


}

