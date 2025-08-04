#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Texture.h"
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
    CTransform(const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Vector3& rotation)
    :   Position(position), Scale(scale), Rotation(rotation)
    {
    }


};

struct CSprite
{
    uint32_t Width;
    uint32_t Height;
    Texture* m_Texture;

    CSprite() 
    {
        m_Texture = nullptr; 
        Width = Height = 1;
    }
    CSprite(Texture* texture)
        : m_Texture(texture)
    {
        Width = m_Texture->GetWidth();
        Height = m_Texture->GetHeight();
    }

};

struct CRigidBody
{
    BMath::Vector2 Velocity;

    CRigidBody(const BMath::Vector2& velocity = BMath::Vector2(20.0f, 20.0f))
        : Velocity(velocity)
    {}

};

struct CCamera
{
    BMath::Vector3 Position;
    BMath::Vector3 Rotation;
    BMath::Matrix4x4 ProjectionMatrix;
    BMath::Matrix4x4 ViewMatrix;
    bool IsOrthographic;
    uint32_t Left, Right, Top, Bottom, ZNear, ZFar;


    CCamera(BMath::Vector3 Position, BMath::Vector3 Rotation,
            bool IsOrthographic, uint32_t Left, uint32_t Right,
            uint32_t Top, uint32_t Bottom, uint32_t ZNear, uint32_t ZFar)
        :   
            Position(Position), Rotation({0.0f, 0.0f, 0.0f}),
            IsOrthographic(true), Left(Left), Right(Right), Top(Top),
            Bottom(Bottom), ZNear(ZNear), ZFar(ZFar) 
          {
              ProjectionMatrix = BMath::Matrix4x4::Ortho(Left, 
                      Right, 
                      Bottom, 
                      Top, 
                      ZNear, 
                      ZFar
                  );
              ViewMatrix = BMath::Matrix4x4();
          }
};


struct CAnimation2D
{
    uint8_t NumFrames;
    uint8_t CurrentFrame;
    uint8_t FrameRateSpeed;
    bool IsLooping;

};
}
