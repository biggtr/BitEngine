#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/ECS/Entity.h"
#include <Bit/Core/Game.h>

class Dystopia : public BitEngine::Game
{
public:
    Dystopia()
    {
    }
    ~Dystopia(){}
    virtual void Initialize() override;
    // virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
    virtual void UIRender() override; 

};
