#pragma once
#include "Bit/Renderer/RendererAPI.h"
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
namespace BitEngine 
{

class Family
{
public:
    static uint8_t Identifier()
    {
        static uint8_t value = 0;
        return value++;
    }

    template<typename T>
    static uint8_t Type()
    {
        static uint8_t type = Family::Identifier();
        return type;
    }
};

struct CTransform 
{
    BitMath::Vector3 Position;
    BitMath::Vector3 Scale;
    BitMath::Vector3 Rotation;

    CTransform(BitMath::Vector3 position, BitMath::Vector3 scale, BitMath::Vector3 rotation)
    :   Position(position), Scale(scale), Rotation(rotation)
    {}


};

struct CHealth
{
    float Health;
    CHealth(float health)
        :   Health(health)
    {}


};

struct CTest
{
    float Test;
    CTest(float test)
        :   Test(test)
    {}


};
class IComponentArray
{
public:

    virtual ~IComponentArray() = default;
};
template<typename T>
class ComponentArray : public IComponentArray
{
public:
    std::vector<T>& GetArray() { return data; }

private:

    std::vector<T> data;

};
class Archetype 
{

public:
    std::unordered_map<uint8_t, IComponentArray*> componentArrays; // typeID --> array of component

public:

    template<typename T>
    void AddComponent(T component)
    {
        uint8_t typeID = Family::Type<T>();
        if(componentArrays.find(typeID) == componentArrays.end()) // will be deleted in future when hashmap for component existence
        {
            // if component was not found add it 
            componentArrays[typeID] = new ComponentArray<T>();
            std::cout << "typeID : " << static_cast<int>(typeID )<< "\n";
        }
        // Getting the component if were found and add new instance of it to the array

        auto componentArray = (ComponentArray<T>*)componentArrays[typeID];
        componentArray->GetArray().push_back(component);
    }
    uint8_t GetSignature()
    {
        uint8_t archetypeSignature = 0;
        for(auto& pair : componentArrays)
        {
            archetypeSignature |= (1 << pair.first); 
            std::cout << static_cast<int>(archetypeSignature) << " this is Signature\n";
        }

        std::cout << componentArrays.size() << " size of component arrays\n";
        return archetypeSignature; 
    }

    ~Archetype()
    {
        for(auto& pair : componentArrays)
        {
            delete pair.second;
        }
    }
};

}
