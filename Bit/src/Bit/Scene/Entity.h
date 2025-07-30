#pragma once 
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "Bit/Systems/System.h"
#include "Bit/Utils/MemoryPool/Pool.h"
#include "Compontents.h"
namespace BitEngine
{

enum class ENTITY_TYPE
{
    PLAYER, BULLET, ENEMY
};

class Entity
{
private:

    uint32_t m_ID;
    Entity(unsigned int id)
    :   m_ID(id)
    {
    }
public:
    uint32_t GetID() const { return m_ID; }
    friend class EntityManager;
};

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
    
    Entity CreateEntity();
    void AddEntity();
    void RemoveEntity();
    void KillEntity();

    template<typename TComponent, typename ...TArgs> void AddComponent(const Entity& entity, TArgs&& ...args);
    template<typename TComponent> bool HasComponent(const Entity& entity);
    template<typename TComponent> void RemoveComponent(const Entity& entity);

    template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
    template<typename TSystem> void RemoveSystem();
    template<typename TSystem> void AddEntityToSystem(const Entity& entity) const;
    void AddEntityToSystems(const Entity& entity) const;
    template<typename TSystem> bool HasSystem() const;
    template<typename TSystem> TSystem& GetSystem() const;
};

}
