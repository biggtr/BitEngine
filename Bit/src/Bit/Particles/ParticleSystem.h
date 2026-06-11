#pragma once
#include <vector>
#include "Bit/Core/Defines.h"
#include "Bit/Math/Vector.h"
#include "Bit/Renderer/Renderer2D.h"

struct ParticleSettings
{
    BMath::Vec3 Position;
    BMath::Vec3 Velocity = {5,1,0};
    BMath::Vec3 VelocityVariation = {5,5,0};

    BMath::Vec4 StartColor = {1,0,1,1};
    BMath::Vec4 EndColor = {1,1,0,1};

    f32 SizeVariation = 4;
    f32 StartSize = 3;
    f32 EndSize = 1;

    f32 RotationVariation = 3;
    f32 StartRotation = 5;
    f32 EndRotation = 1;

    f32 LifeTimeVariation = 2;
    f32 LifeTime = 1;
};

b8 Particle2DSystemInitialize(u64* memoryRequirement, void* state);
void Particle2DSystemShutdown(void* state);
void Particle2DSystemResize(u32 count);
void Particle2DSystemEmit(const ParticleSettings& particleSettings);
void Particle2DSystemOnUpdate(f32 deltaTime);
void Particle2DSystemOnRender(BitEngine::Renderer2D* renderer2D);

