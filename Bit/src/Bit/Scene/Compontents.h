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
    BMath::Vector3 Position;
    BMath::Vector3 Scale;
    BMath::Vector3 Rotation;

    CTransform() = default;
    CTransform(BMath::Vector3 position, BMath::Vector3 scale, BMath::Vector3 rotation)
    :   Position(position), Scale(scale), Rotation(rotation)
    {}


};

struct CSprite
{
    uint32_t Width;
    uint32_t Height;

    CSprite(uint32_t width = 100, uint32_t height = 100)
        : Width(width), Height(height)
    {}

};
}
