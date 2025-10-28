#pragma once 
#include "Bit/ECS/Entity.h"
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
    virtual void Render3D() override; 
    virtual void RenderUI() override; 


};
