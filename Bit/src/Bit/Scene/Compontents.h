#pragma once
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Scene/Entity.h"
#include <unordered_map>
#include <vector>
namespace BitEngine 
{

class Family
{
public:
    static unsigned int Identifier()
    {
        static unsigned int value = 0;
        return value++;
    }

    template<typename T>
    static unsigned int Type()
    {
        static unsigned int type = Family::Identifier();
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


class IComponentArray
{
public:

    virtual ~IComponentArray() = default;
};
template<typename T>
class ComponentArray : public IComponentArray
{
public:

    std::vector<T> data;

    std::vector<T>& GetArray() { return data; }

};
class ComponentManager 
{

public:
    std::unordered_map<unsigned int, IComponentArray*> componentArrays;

    

public:

    template<typename T>
    void AddComponent(const T& component)
    {
        auto typeID = Family::Type<T>();
        if(componentArrays.find(typeID) == componentArrays.end()) // will be deleted in future when hashmap for component existence
        {
            // if component was not found add it 
            componentArrays[typeID] = new ComponentArray<T>();
        }
        // Getting the component if were found and add new instance of it to the array

        std::vector<T>& componentArray = (ComponentArray<T>*)componentArrays[typeID];
        componentArray.GetArray().push_back(component);
    }

    ~ComponentManager()
    {
        for(auto& pair : componentArrays)
        {
            delete pair.second;
        }
    }

};
}
