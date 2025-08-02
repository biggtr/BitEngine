#pragma once 
#include <cstdint>
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
    :   m_ID(id)
    {
    }
public:
    uint32_t GetID() const { return m_ID; }

    template<typename TComponent> 
    bool HasComponent();
    template<typename TComponent> 
    void RemoveComponent(const Entity& entity);
    friend class EntityManager;
};

}
