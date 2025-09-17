#pragma once 
#include "Bit/Scene/Entity.h"
#include <Bit/Core/Game.h>

class Game3D : public BitEngine::Game
{
public:
    Game3D()
    {
    }
    ~Game3D(){}
    virtual void Initialize() override;
    // virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
    virtual void UIRender() override; 



};
