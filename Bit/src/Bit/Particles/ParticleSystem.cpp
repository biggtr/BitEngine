#include "ParticleSystem.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Random.h"
#include "Bit/Renderer/Renderer2D.h"

#define MAX_PARTICLE_COUNT 4096

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

struct ParticlesState
{
    Particle* Particles;
    u32 ParticleIndex;
};


static ParticlesState* statePtr = nullptr;

b8 Particle2DSystemInitialize(u64* memoryRequirement, void* state)
{
    *memoryRequirement = sizeof(ParticlesState);
    if(!state)
    {
        return true;
    }

    statePtr = (ParticlesState*)state;
    memset(statePtr, 0, sizeof(ParticlesState));
    statePtr->Particles = (Particle*)DArrayReserve(Particle, MAX_PARTICLE_COUNT); 
    for(u32 i = 0; i < MAX_PARTICLE_COUNT; ++i)
    {
        statePtr->Particles[i] = Particle();
    }
    statePtr->ParticleIndex = MAX_PARTICLE_COUNT - 1;
    return true;
}
void Particle2DSystemShutdown(void* state)
{
    if(state)
    {
        statePtr = 0;
    }
    BIT_LOG_INFO("Particle2DSystem Is shutting down..!");
}
void Particle2DSystemEmit(const ParticleSettings& particleSettings)
{

    Particle& particle = statePtr->Particles[statePtr->ParticleIndex];
    particle.IsActive = true;

    particle.Position = particleSettings.Position;
    particle.Velocity = particleSettings.Velocity;
    particle.StartSize = particleSettings.StartSize;
    particle.StartRotation = particleSettings.StartRotation;
    particle.StartColor = particleSettings.StartColor;
    particle.LifeTime = particleSettings.LifeTime;

    particle.Velocity.x += particleSettings.VelocityVariation.x * BMath::RandomRange(-1.0f, 1.0f); 
    particle.Velocity.y += particleSettings.VelocityVariation.y * BMath::RandomRange(-1.0f, 1.0f); 

    particle.StartColor = particleSettings.StartColor;
    particle.EndColor = particleSettings.EndColor;

    particle.StartSize += particleSettings.SizeVariation * BMath::RandomRange(-1.0f, 1.0f);
    particle.EndSize = particleSettings.EndSize;

    particle.StartRotation += particleSettings.RotationVariation * BMath::RandomRange(-1.0f, 1.0f);
    particle.EndRotation = particleSettings.EndRotation;

    particle.LifeTime += particleSettings.LifeTimeVariation * BMath::RandomRange(-1.0f, 1.0f);
    particle.TimeRemaining = particle.LifeTime;

    statePtr->ParticleIndex = --statePtr->ParticleIndex % MAX_PARTICLE_COUNT;
}
void Particle2DSystemOnUpdate(f32 deltaTime)
{
    
    for(u32 i = 0; i < MAX_PARTICLE_COUNT; ++i)
    {
        Particle& particle = statePtr->Particles[i];
        if(!particle.IsActive)
            continue;

        if(particle.TimeRemaining < 0.0f)
        {
            particle.IsActive = false;
            continue;
        }
        particle.TimeRemaining -= deltaTime;
        particle.Position += particle.Velocity * deltaTime;
    }
}
void Particle2DSystemOnRender(BitEngine::Renderer2D* renderer2D)
{

    for(u32 i = 0; i < MAX_PARTICLE_COUNT; ++i)
    {
        Particle& particle = statePtr->Particles[i];
        if(!particle.IsActive)
            continue;

        f32 t = particle.TimeRemaining / particle.LifeTime;

        f32 size = BMath::Lerp(particle.EndSize, particle.StartSize, t);
        f32 rotation = BMath::Lerp(particle.EndRotation, particle.StartRotation, t);

        BMath::Vec4 color = BMath::Lerp(particle.EndColor, particle.StartColor, t);

        renderer2D->DrawQuad(particle.Position, BMath::Vec3(size, size, 0.0f), rotation, color);
    }
}

