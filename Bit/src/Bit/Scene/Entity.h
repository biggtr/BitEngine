#pragma once 
#include <cstdint>
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

}
