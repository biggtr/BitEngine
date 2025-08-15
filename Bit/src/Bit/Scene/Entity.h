#pragma once 
#include "Bit/Core/Defines.h"
#include <cstdint>
#include <utility>
namespace BitEngine
{
class EntityManager;
enum class ENTITY_TYPE
{
    PLAYER, BULLET, ENEMY
};
class Entity
{
private:

    uint32_t m_ID;
    EntityManager* m_EntityManager;
    Entity(unsigned int id)
    :   m_ID(id), m_EntityManager(nullptr)
    {
    }
public:
    Entity(){}
    b8 operator==(const Entity& other) const
    {
        return m_ID == other.GetID();
    }
public:
    uint32_t GetID() const { return m_ID; }

    template<typename TComponent, typename ...TArgs>
    TComponent& AddComponent(TArgs&& ...args);
    template<typename TComponent>
    TComponent& GetComponent();
    template<typename TComponent> 
    b8 HasComponent();
    template<typename TComponent> 
    void RemoveComponent();
    void KillEntity();
    
    friend class EntityManager;
};
}
