#pragma once 
#include "Bit/Scene/Entity.h"
#include <Bit/Core/Game.h>

class TestGame : public BitEngine::Game
{
private:
        BitEngine::Entity m_Player;
        BitEngine::Entity m_Background;
        std::vector<u32> m_Bodies;
public:
    TestGame()
    {
    }
    ~TestGame(){}
    virtual void Initialize() override;
    virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 

private:
    // Fix Static Functions in future and pass this ptr to the functions to bind 
    void OnJump(const BitEngine::Entity& entity);


};
