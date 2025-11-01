#pragma once
#include <vector>
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"

namespace BitEngine
{

class Renderer2D;

struct ParticleSettings
{

    BMath::Vec3 Velocity;
    BMath::Vec3 VelocityVariation;

    BMath::Vec4 StartColor;
    BMath::Vec4 EndColor;

    f32 SizeVariation;
    f32 StartSize;
    f32 EndSize;

    f32 RotationVariation;
    f32 StartRotation;
    f32 EndRotation;

    f32 LifeTimeVariation;
    f32 LifeTime;
    f32 TimeRemaining;
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
