#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/Scene/Entity.h"
#include <Bit/Core/Game.h>

class PoolGame : public BitEngine::Game
{
private:
    BitEngine::Entity m_WhiteBall;
    BitEngine::Entity m_Table;
    BitEngine::Entity m_Pockets[6];
    BitEngine::Entity m_Balls[100];
    u32 m_ActiveBallCount;
    BMath::Vec3 impulseLine;

    b8 isDragging;
    i32 initialMouseX;
    i32 initialMouseY;
    i32 currentMouseX;
    i32 currentMouseY;
public:
    PoolGame()
    {
    }
    ~PoolGame(){}
    virtual void Initialize() override;
    // virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override; 
    virtual void UIRender() override; 

private:
    void CreateBalls(BitEngine::Entity* outEntities, u8 lastRow);
    BitEngine::Entity CreateTable(BitEngine::Entity* outPockets, f32 width, f32 height);
    // void OnJump(const BitEngine::Entity& entity);


};
