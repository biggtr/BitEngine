#pragma once
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/RendererAPI.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Texture.h"
#include <X11/Xlib.h>
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

struct CBoxCollider
{
    
    BMath::Vec3 Size;
    
    BMath::Vec3 Offset;
    CBoxCollider()
        : Size(0.0f), Offset(0.0f)
    {

    }
    CBoxCollider(BMath::Vec3 size, const BMath::Vec3& offset = {0.0f})
        : Size(size), Offset(offset)
    {
    }
};
struct CSprite
{
    uint32_t Width;
    uint32_t Height;
    BMath::Vec4 Color;
//TODO: change it to texture id and in systems query the texture objects with asset manager
    Texture* STexture; 
    u32 FrameWidth;
    u32 FrameHeight;
    u32 CurrentFrame;
    float UVs[8];

    CSprite() 
    {
        STexture = nullptr; 
        Width = Height = FrameWidth = FrameHeight = 1;
        CurrentFrame = 0;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
    CSprite(Texture* texture, const BMath::Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) 
        :  STexture(texture)
    {
        Color = color;
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        FrameWidth = Width;
        FrameHeight = Height;
        CurrentFrame = 0;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
    CSprite(Texture* texture, u32 frameWidth, u32 frameHeight) 
        :  STexture(texture), FrameWidth(frameWidth), FrameHeight(frameHeight)
    {
        Color = { 1.0f, 1.0f, 1.0f, 1.0f}; 
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        CurrentFrame = 0;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
    CSprite(Texture* texture, u32 frameWidth, u32 frameHeight, u32 currentFrame) 
        :  STexture(texture), FrameWidth(frameWidth), FrameHeight(frameHeight), CurrentFrame(currentFrame)
    {
        Color = { 1.0f, 1.0f, 1.0f, 1.0f}; 
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
};

struct CAnimation2D
{
    const char* Name;
    u8 NumFrames;
    u8 StartFrame;
    f32 FrameDuration;
    bool IsLooping;
};
struct CAnimation2DController
{
    std::vector<CAnimation2D> Animations;
    const char* CurrentAnimationName;
    f32 ElapsedTime;
    u8 CurrentFrame;
    b8 AnimationChanged;

    CAnimation2DController()
        : CurrentAnimationName(""), ElapsedTime(0.0f), CurrentFrame(0), AnimationChanged(false)
    {

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
    f32 Rotation;
    BMath::Mat4 ViewMatrix;
    b8 ViewDirty;
    CCamera() { }
    CCamera(const BMath::Vec3& position, f32 rotation)
        :   
            Position(position), Rotation(rotation)
          {
                  ViewMatrix = BMath::Mat4::Rotate(0.0f, 0.0f, -rotation) *
                     BMath::Mat4::Translate(-Position.x, -Position.y, -Position.z);
                  ViewDirty = true;
          }
};


}
