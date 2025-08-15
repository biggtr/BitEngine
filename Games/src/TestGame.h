#pragma once 
#include "Bit/Scene/Entity.h"
#include <Bit/Core/Game.h>

class TestGame : public BitEngine::Game
{
private:
        BitEngine::Entity m_Player;
public:
    TestGame()
    {
    }
    ~TestGame(){}
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
};
