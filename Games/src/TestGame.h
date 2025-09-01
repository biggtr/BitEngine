#pragma once 
#include "Bit/Scene/Entity.h"
#include <Bit/Core/Game.h>

class TestGame : public BitEngine::Game
{
private:
    BitEngine::Entity whiteBall;
    BMath::Vec3 impulseLine;

    b8 isDragging;
    i32 initialMouseX;
    i32 initialMouseY;
    i32 currentMouseX;
    i32 currentMouseY;
public:
    TestGame()
    {
    }
    ~TestGame(){}
    virtual void Initialize() override;
    virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
    virtual void UIRender() override; 

private:
    // void OnJump(const BitEngine::Entity& entity);


};
