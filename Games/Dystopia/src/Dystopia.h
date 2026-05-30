#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/ECS/Entity.h"
#include "Bit/Renderer/Texture.h"
#include "PlayerController.h"
#include <Bit/Core/Game.h>
#include "EnemyManager.h"


class Dystopia : public BitEngine::Game
{
public:
    Dystopia()
        : m_PlayerController()
    {
    }
    ~Dystopia()
    {
        delete m_EnemyManager;
    }
    virtual void Initialize() override;
    // virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render2D() override; 
    virtual void Render3D() override; 
    virtual void RenderUI() override; 

    void UpdateAnimation(BitEngine::Character2DControllerComponent& controller, BitEngine::TransformComponent& transform);
    void SpawnParticles(BMath::Vec3 position);

    void SpawnEnemy(f32 deltaTime);
private:
    PlayerController m_PlayerController;
    EnemyManager* m_EnemyManager;
    BitEngine::Entity player;
    BitEngine::Entity weapon;

    BitEngine::Texture* m_CharacterSpriteSheet;
    BitEngine::Entity* m_Enemies;
    BMath::Vec3 m_EnemySpawnLocation;
    f32 m_EnemySpawnCoolDown = 0.0f;
    f32 m_EnemySpawnCoolDownMax = 10.0f;
    u32 TileIndex;

    BMath::Vec3 m_SpawnLocation;
};
