#include "ParticleSystem.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Random.h"
#include "Bit/Renderer/Renderer2D.h"



namespace BitEngine
{


ParticleSystem::ParticleSystem()
{
    m_Particles.resize(4096);
    m_ParticleIndex = 4095;
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Resize(u32 count)
{
    m_Particles.resize(count);
    m_ParticleIndex = count - 1;
}
void ParticleSystem::Emit(const ParticleSettings& particleSettings)
{
    Particle& particle = m_Particles[m_ParticleIndex];
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

    m_ParticleIndex = --m_ParticleIndex % m_Particles.size();
}

void ParticleSystem::OnUpdate(f32 deltaTime)
{
    for(Particle& particle : m_Particles)
    {
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

void ParticleSystem::OnRender(Renderer2D* renderer2D)
{
    for(const Particle& particle : m_Particles)
    {
        if(!particle.IsActive)
            continue;

        f32 t = particle.TimeRemaining / particle.LifeTime;

        f32 size = BMath::Lerp(particle.EndSize, particle.StartSize, t);
        f32 rotation = BMath::Lerp(particle.EndRotation, particle.StartRotation, t);

        BMath::Vec4 color = BMath::Lerp(particle.EndColor, particle.StartColor, t);

        renderer2D->DrawQuad(particle.Position, BMath::Vec3(size, size, 0.0f), rotation, color);
        // BIT_LOG_DEBUG("RenderedParticle with position : %d, %d, %d", particle.Position.x, particle.Position.y, particle.Position.z) 
    }

}

}
