#pragma once 
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "Compontents.h"
namespace BitEngine
{

enum class ENTITY_TYPE
{
    PLAYER, BULLET, ENEMY
};

class Entity
{
public:

private:
    Entity(unsigned int id)
    :   m_ID(id)
    {
    }
    
private:
    unsigned int m_ID;
};


}
