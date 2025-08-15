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
        RequireComponent<CAnimation2DController>();
        RequireComponent<CSprite>();
    }
    SYSTEM_CLASS_TYPE(ANIMATION2D);


    void CreateAnimation(const Entity& entity, const char* name, u8 frameCount, u8 startFrame, f32 frameDuration, b8 isLooping = true)
    {
        if(!m_EntityManager->HasComponent<CAnimation2DController>(entity)) return;
        CAnimation2DController& animationController = m_EntityManager->GetComponent<CAnimation2DController>(entity);
        CAnimation2D animation{name, frameCount, startFrame, frameDuration, isLooping};
        animationController.Animations.push_back(animation);
        animationController.CurrentAnimationName = name;
    }

    void SetAnimation(const Entity& entity, const char* animationName)
    {
        if(!m_EntityManager->HasComponent<CAnimation2DController>(entity)) return;

        CAnimation2DController& animationController = m_EntityManager->GetComponent<CAnimation2DController>(entity);

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
    CAnimation2D* GetCurrentAnimation(CAnimation2DController& animationController)
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
            CSprite& spriteComponent = m_EntityManager->GetComponent<CSprite>(entity);
            CAnimation2DController& animationController = m_EntityManager->GetComponent<CAnimation2DController>(entity);
            CAnimation2D* currentAnimation = GetCurrentAnimation(animationController);
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




