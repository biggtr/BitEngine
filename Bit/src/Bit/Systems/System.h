#pragma once
#include <cstdint>
#include <vector>
namespace BitEngine
{

enum class SYSTEM_TYPE : uint32_t
{
    MOVEMENT,
    PHYSICS,
    COUNT, //Array size indicator
};
#define SYSTEM_CLASS_TYPE(type) static SYSTEM_TYPE GetStaticType() { return SYSTEM_TYPE::type }\
                                                    virtual SYSTEM_TYPE GetSystemType() const override { return SYSTEM_TYPE::type}\
                                                    virtual const char* GetName() const override { return #type; }
const unsigned int MAX_COMPONENTS = 32;
typedef uint32_t Signature;
class Entity;

class System
{
private:
    Signature m_ComponentSignature;
    std::vector<Entity> m_Entities;
    SYSTEM_TYPE m_SystemType; 
    
public:
    System() = default;
    virtual ~System() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void AddEntity(const Entity& entity);
    virtual void RemoveEntity(Entity& entity);
    virtual const std::vector<Entity>& GetEntities() const;
    virtual const Signature& GetComponentSignature() { return m_ComponentSignature; };
    static SYSTEM_TYPE GetStaticType();
    virtual SYSTEM_TYPE GetSystemType() const = 0; 
    virtual const char* GetName() const = 0;

    
    template<typename T>
    void RequireComponent();

};
}
