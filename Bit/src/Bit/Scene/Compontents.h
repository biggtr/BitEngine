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
    BMath::vec3 Position;
    BMath::vec3 Scale;
    BMath::vec3 Rotation;

    CTransform() = default;
    CTransform(const BMath::vec3& position, const BMath::vec3& scale, const BMath::vec3& rotation)
    {
        Position = position;
        Scale = scale;
        Rotation = rotation;
    }


};

struct CSprite
{
    uint32_t Width;
    uint32_t Height;
    BMath::vec4 Color;
    Texture* m_Texture;

    CSprite() 
    {
        m_Texture = nullptr; 
        Width = Height = 1;
    }
    CSprite(Texture* texture, const BMath::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f})
        :  m_Texture(texture)
    {
        Color = color;
        Width = m_Texture->GetWidth();
        Height = m_Texture->GetHeight();
    }

};

struct CRigidBody
{
    BMath::vec2 Velocity;

    CRigidBody(const BMath::vec2& velocity = {20.0f, 20.0f} )
    {
        Velocity = velocity;
    }

};

struct CCamera
{
    BMath::vec3 Position;
    BMath::vec3 Rotation;
    BMath::Matrix4x4 ProjectionMatrix;
    BMath::Matrix4x4 ViewMatrix;
    bool IsOrthographic;
    f32 Left, Right, Top, Bottom, ZNear, ZFar;

    CCamera() { }
    CCamera(const BMath::vec3& position, const BMath::vec3& rotation,
            bool isOrthographic, f32 left, f32 right,
            f32 bottom, f32 top, f32 zNear, f32 zFar)
        :   
            Position(position), Rotation(rotation), IsOrthographic(isOrthographic), Left(left), Right(right), Top(top),
            Bottom(bottom), ZNear(zNear), ZFar(zFar) 
          {
              ProjectionMatrix = BMath::Matrix4x4::Ortho(Left, 
                      Right, 
                      Bottom, 
                      Top, 
                      ZNear, 
                      ZFar
                  );
                  ViewMatrix = BMath::Matrix4x4::Rotate(-Rotation.z) *
                     BMath::Matrix4x4::Translate(-Position.x, -Position.y, -Position.z);
          }
};


struct CAnimation2D
{
    uint8_t NumFrames;
    uint8_t CurrentFrame;
    uint8_t FrameRateSpeed;
    float UVs[8];
    bool IsLooping;

};
}
