#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Utils/MemoryPool/Pool.h"
namespace BitEngine 
{
class Entity;
class System;
class IPool;
class EntityManager
{
    
private:
    uint32_t m_NumOfEntities;

    std::vector<Signature> m_EntitiesSignatures;

    System* m_Systems[(uint32_t)SYSTEM_TYPE::COUNT];
    std::vector<IPool*> m_ComponentPools;

    std::vector<Entity> m_EntitiesToAdd;
    std::vector<Entity> m_EntitiesToRemove;


public:
    EntityManager()
        : m_NumOfEntities(0)
    {}

    void Update();
    
    Entity CreateEntity();
    void AddEntity();
    void RemoveEntity();
    void KillEntity();
    void AddEntityToSystems(const Entity &entity) const;

template<typename TComponent, typename ...TArgs>
void AddComponent(const Entity& entity, TArgs&& ...args)
{
    uint32_t entityID = entity.GetID();
    uint32_t componentID = Component::Type<TComponent>();
    
    if(componentID >= m_ComponentPools.size())
    {
        m_ComponentPools.resize(componentID + 1, nullptr);
    }

    if(!m_ComponentPools[componentID])
    {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>(m_NumOfEntities);
        m_ComponentPools[componentID] = newComponentPool;
    }
    Pool<TComponent>* componentPool = (Pool<TComponent>*)(m_ComponentPools[componentID]);
    if(entityID >= componentPool->GetSize())
    {
        componentPool->Resize(m_NumOfEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityID, newComponent);

    m_EntitiesSignatures[entityID] |= (1 << componentID);
    BIT_LOG_INFO("Added new new component with ID : %d to entity with ID : %d", componentID, entityID);

}
template<typename TComponent> 
bool HasComponent(const Entity& entity)
{
    auto entityID = entity.GetID();
    return m_EntitiesSignatures[entityID] & (1 << Component::Type<TComponent>());
}
template<typename TComponent> 
void RemoveComponent(const Entity& entity)
{
    auto entityID = entity.GetID();
    m_EntitiesSignatures[entityID] &= ~(1 << Component::Type<TComponent>());
}


template<typename TSystem, typename ...TArgs> 
void AddSystem(TArgs&& ...args)
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
void RemoveSystem()
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
bool HasSystem() const
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");
    SYSTEM_TYPE type = TSystem::GetStaticType();
    return m_Systems[(uint32_t)type] != nullptr;
}
template<typename TSystem> 
TSystem& GetSystem() const
{
    static_assert(std::is_base_of<System, TSystem>::value,"TSystem must be derived from System.!");
    SYSTEM_TYPE type = TSystem::GetStaticType();
    return *(TSystem*)m_Systems[(uint32_t)type];

}
template<typename TSystem> 
void AddEntityToSystem(const Entity& entity) const
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


};
}
