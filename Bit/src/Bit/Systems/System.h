#pragma once
#include <cstdint>
#include "Bit/Core/Event.h"
#include "Bit/Scene/Compontents.h"
#include <vector>
#include "Bit/Core/Defines.h"
namespace BitEngine
{
class EntityManager;
enum class SYSTEM_TYPE : uint32_t
{
    MOVEMENT,
    RENDER,
    UI_RENDER,
    PHYSICS2D,
    CAMERA,
    ANIMATION2D,
    COLLISION,
    INPUT,
    COUNT, //Array size indicator
};
#define SYSTEM_CLASS_TYPE(type) static SYSTEM_TYPE GetStaticType() { return SYSTEM_TYPE::type; }\
                                                    virtual SYSTEM_TYPE GetSystemType() const override { return SYSTEM_TYPE::type; }\
                                                    virtual const char* GetName() const override { return #type; }
class Entity;

class System
{
private:
    Signature m_ComponentSignature;
    SYSTEM_TYPE m_SystemType; 
    
public:
    System();
    virtual ~System() = default; 

    virtual void OnEntityAdded(const Entity& entity);
    virtual void AddEntity(const Entity& entity);
    virtual void RemoveEntity(const Entity& entity);
    virtual const std::vector<Entity>& GetEntities() const;
    virtual const Signature& GetComponentSignature() { return m_ComponentSignature; };

    static SYSTEM_TYPE GetStaticType();
    virtual SYSTEM_TYPE GetSystemType() const = 0; 
    virtual const char* GetName() const = 0;

    void SetEntityManager(EntityManager* entityManager);

    
    template<typename T>
    void RequireComponent()
    {
        unsigned int componentId = Component::Type<T>();
        m_ComponentSignature |= (1 << componentId);
    }
protected:
    EntityManager* m_EntityManager;
    std::vector<Entity> m_Entities;
};

} 
