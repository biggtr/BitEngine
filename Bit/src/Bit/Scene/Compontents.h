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
    BMath::Vec3 Position;
    BMath::Vec3 Scale;
    BMath::Vec3 Rotation;

    CTransform() = default;
    CTransform(const BMath::Vec3& position, const BMath::Vec3& scale, const BMath::Vec3& rotation)
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
    BMath::Vec4 Color;
    Texture* m_Texture;

    CSprite() 
    {
        m_Texture = nullptr; 
        Width = Height = 1;
    }
    CSprite(Texture* texture, const BMath::Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f})
        :  m_Texture(texture)
    {
        Color = color;
        Width = m_Texture->GetWidth();
        Height = m_Texture->GetHeight();
    }

};

struct CRigidBody
{
    BMath::Vec2 Velocity;

    CRigidBody(const BMath::Vec2& velocity = {20.0f, 20.0f} )
    {
        Velocity = velocity;
    }

};

struct CCamera
{
    BMath::Vec3 Position;
    BMath::Vec3 Rotation;
    BMath::Mat4 ProjectionMatrix;
    BMath::Mat4 ViewMatrix;
    bool IsOrthographic;
    f32 Left, Right, Top, Bottom, ZNear, ZFar;

    CCamera() { }
    CCamera(const BMath::Vec3& position, const BMath::Vec3& rotation,
            bool isOrthographic, f32 left, f32 right,
            f32 bottom, f32 top, f32 zNear = -1.0f, f32 zFar = 1.0f)
        :   
            Position(position), Rotation(rotation), IsOrthographic(isOrthographic), Left(left), Right(right), Top(top),
            Bottom(bottom), ZNear(zNear), ZFar(zFar) 
          {
              ProjectionMatrix = BMath::Mat4::Ortho(Left, 
                      Right, 
                      Bottom, 
                      Top, 
                      ZNear, 
                      ZFar
                  );
                  ViewMatrix = BMath::Mat4::Rotate(-Rotation.z) *
                     BMath::Mat4::Translate(-Position.x, -Position.y, -Position.z);
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
