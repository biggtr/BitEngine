#pragma once
#include <vector>
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"

namespace BitEngine
{

class Renderer2D;

struct ParticleSettings
{
    BMath::Vec3 Position;
    BMath::Vec3 Velocity = {5,5,0};
    BMath::Vec3 VelocityVariation = {5,5,0};

    BMath::Vec4 StartColor = {1,0,1,1};
    BMath::Vec4 EndColor = {1,1,0,1};

    f32 SizeVariation = 4;
    f32 StartSize = 5;
    f32 EndSize = 3;

    f32 RotationVariation = 3;
    f32 StartRotation = 5;
    f32 EndRotation = 1;

    f32 LifeTimeVariation = 2;
    f32 LifeTime = 1;
};
class ParticleSystem
{

private:
    struct Particle
    {
        BMath::Vec3 Position;
        BMath::Vec3 Velocity;

        BMath::Vec4 StartColor;
        BMath::Vec4 EndColor;

        f32 StartSize;
        f32 EndSize;

        f32 StartRotation;
        f32 EndRotation;

        f32 LifeTime;
        f32 TimeRemaining;
        
        b8 IsActive = false;
    };

    std::vector<Particle> m_Particles;
    u32 m_ParticleIndex;

public:
    ParticleSystem();
    ~ParticleSystem();

    void Resize(u32 count);

    void Emit(const ParticleSettings& particleSettings);

    void OnUpdate(f32 deltaTime);
    void OnRender(Renderer2D* renderer2D);

};
}
