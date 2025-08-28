#pragma once
#include "Bit/Core/Logger.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Systems/System.h"
#include "Bit/Utils/MemoryPool/Pool.h"
#include <unordered_map>
namespace BitEngine
{

class Animation2DSystem : public System
{
public:
    Animation2DSystem()
    {
        RequireComponent<Animation2DControllerComponent>();
        RequireComponent<SpriteComponent>();
    }
    SYSTEM_CLASS_TYPE(ANIMATION2D);


    void CreateAnimation(const Entity& entity, const char* name, u8 frameCount, u8 startFrame, f32 frameDuration, b8 isLooping = true)
    {
        if(!m_EntityManager->HasComponent<Animation2DControllerComponent>(entity)) return;
        Animation2DControllerComponent& animationController = m_EntityManager->GetComponent<Animation2DControllerComponent>(entity);
        Animation2DComponent animation{name, frameCount, startFrame, frameDuration, isLooping};
        animationController.Animations.push_back(animation);
        animationController.CurrentAnimationName = name;
    }

    void SetAnimation(const Entity& entity, const char* animationName)
    {
        if(!m_EntityManager->HasComponent<Animation2DControllerComponent>(entity)) return;

        Animation2DControllerComponent& animationController = m_EntityManager->GetComponent<Animation2DControllerComponent>(entity);

        if(animationController.CurrentAnimationName != animationName)
        {
            for(auto& animation : animationController.Animations)
            {
                if(animation.Name == animationName)
                {
                    animationController.CurrentAnimationName = animationName;
                    break;
                }
            }
        }
    }
    Animation2DComponent* GetCurrentAnimation(Animation2DControllerComponent& animationController)
    {

        for(auto& animation : animationController.Animations)
        {
            if(animation.Name == animationController.CurrentAnimationName)
            {
                return &animation;
            }
        }
        return nullptr;
    }
    
public:
    
    void Update(f64 deltaTime) 
    {
        for(const Entity& entity : GetEntities())
        {
            SpriteComponent& spriteComponent = m_EntityManager->GetComponent<SpriteComponent>(entity);
            Animation2DControllerComponent& animationController = m_EntityManager->GetComponent<Animation2DControllerComponent>(entity);
            Animation2DComponent* currentAnimation = GetCurrentAnimation(animationController);
            if(!currentAnimation) continue;
            if(animationController.AnimationChanged)
            {
                animationController.ElapsedTime = 0.0f;
                animationController.CurrentFrame = 0;
                animationController.AnimationChanged = false;
            }
            animationController.ElapsedTime += deltaTime;
            if(animationController.ElapsedTime >= currentAnimation->FrameDuration)
            {
                animationController.CurrentFrame++;
                animationController.ElapsedTime = 0.0f;
            }
            if(animationController.CurrentFrame >= currentAnimation->NumFrames)
            {
                animationController.CurrentFrame = currentAnimation->IsLooping ? 0 : currentAnimation->NumFrames - 1;
            }
            spriteComponent.CurrentFrame = currentAnimation->StartFrame + animationController.CurrentFrame;
        }
    }

};

}




