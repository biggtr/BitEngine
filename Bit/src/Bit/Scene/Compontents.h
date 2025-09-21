#pragma once
#include "Bit/Core/Input.h"
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

struct TransformComponent 
{
    BMath::Vec3 Position;
    BMath::Vec3 Scale;
    BMath::Vec3 Rotation;

    TransformComponent() = default;
    TransformComponent(const BMath::Vec3& position, const BMath::Vec3& scale = {1.0f, 1.0f, 0.0f}, const BMath::Vec3& rotation = {0.0f, 0.0f, 0.0f})
    {
        Position = position;
        Scale = scale;
        Rotation = rotation;
    }
};

struct SpriteComponent
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
    b8 IsUI;

    SpriteComponent() 
    {
        STexture = nullptr; 
        Width = Height = FrameWidth = FrameHeight = 1;
        CurrentFrame = 0;
        IsUI = false;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl

    }
    SpriteComponent(Texture* texture, const BMath::Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) 
        :  STexture(texture)
    {
        Color = color;
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        FrameWidth = Width;
        FrameHeight = Height;
        CurrentFrame = 0;
        IsUI = false;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
    SpriteComponent(Texture* texture, u32 frameWidth, u32 frameHeight)
        :  STexture(texture), FrameWidth(frameWidth), FrameHeight(frameHeight)
    {
        Color = { 1.0f, 1.0f, 1.0f, 1.0f}; 
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        IsUI = false;
        CurrentFrame = 0;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
    SpriteComponent(Texture* texture, u32 frameWidth, u32 frameHeight, u32 currentFrame)
        :  STexture(texture), FrameWidth(frameWidth), FrameHeight(frameHeight), CurrentFrame(currentFrame)
    {
        Color = { 1.0f, 1.0f, 1.0f, 1.0f}; 
        Width = texture->GetWidth();
        Height = texture->GetHeight();
        IsUI = false;
        UVs[0] = 0.0f; UVs[1] = 0.0f; //bt
        UVs[2] = 1.0f; UVs[3] = 0.0f; //br
        UVs[4] = 1.0f; UVs[5] = 1.0f; //tr
        UVs[6] = 0.0f; UVs[7] = 1.0f; //tl
    }
};

struct Circle2DComponent
{
    f32 Radius;
    BMath::Vec4 Color;
    f32 Thickness;
    f32 Fade;
    Circle2DComponent()
    {
        Radius = 12.0f;
        Color = {1.0f, 1.0f, 1.0f, 1.0};
        Thickness = 1.0f;
        Fade = 0.05f;
    }
    Circle2DComponent(f32 radius, const BMath::Vec4 color = (1.0f), f32 thickness = 1.0f, f32 fade = 0.005f)
        : Radius(radius), Color(color), Thickness(thickness), Fade(fade)
    {}
};
struct Animation2DComponent
{
    const char* Name;
    u8 NumFrames;
    u8 StartFrame;
    f32 FrameDuration;
    bool IsLooping;
};
struct Animation2DControllerComponent
{
    std::vector<Animation2DComponent> Animations;
    const char* CurrentAnimationName;
    f32 ElapsedTime;
    u8 CurrentFrame;
    b8 AnimationChanged;

    Animation2DControllerComponent()
        : CurrentAnimationName(""), ElapsedTime(0.0f), CurrentFrame(0), AnimationChanged(false)
    {

    }
    
};
struct Box2DColliderComponent
{
    
    BMath::Vec3 Size;
    
    BMath::Vec3 Offset;
    Box2DColliderComponent()
        : Size(0.0f), Offset(0.0f)
    {

    }
    Box2DColliderComponent(BMath::Vec3 size, const BMath::Vec3& offset = {0.0f})
        : Size(size), Offset(offset)
    {
    }
};
struct Circle2DColliderComponent
{
    f32 Radius;
    BMath::Vec3 Offset;
    Circle2DColliderComponent()
    {
        Radius = 12.0f;
    }
    Circle2DColliderComponent(f32 radius)
        : Radius(radius)
    {}
};
struct Rigid2DBodyComponent
{
    enum class BodyType { Static, Dynamic, Kinematic};
    BodyType Type = BodyType::Dynamic;

    u32 BodyIndex;
    f32 Mass;
    f32 Restitution;
    Rigid2DBodyComponent(){}
    Rigid2DBodyComponent(f32 mass, f32 restitution = 0.9f)
        : Mass(mass), Restitution(restitution) 
    {
    }

};

struct Camera2DComponent
{
    BMath::Vec3 Position;
    f32 Rotation;
    BMath::Mat4 ViewMatrix;
    b8 ViewDirty;
    Camera2DComponent() { }
    Camera2DComponent(const BMath::Vec3& position, f32 rotation)
        :   
            Position(position), Rotation(rotation)
          {
                  ViewMatrix = BMath::Rotate(0.0f, 0.0f, -rotation) *
                     BMath::Translate(-Position.x, -Position.y, -Position.z);
                  ViewDirty = true;
          }
};


}
