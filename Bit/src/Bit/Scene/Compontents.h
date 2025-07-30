#pragma once
#include "Bit/Renderer/RendererAPI.h"
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>
namespace BitEngine 
{

class Component
{
public:
    static uint8_t GetID()
    {
        static uint8_t newID = 0;
        return newID++;
    }

    template<typename T>
    static uint8_t Type()
    {
        static uint8_t type = Component::GetID();
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

}
