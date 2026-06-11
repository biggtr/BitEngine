#pragma once
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Math/Vector.h"
#include "Bit/Tiles/TileMap.h"
#include "EnemyManager.h"

class PlayerController 
{

public:

    void HandleInput(BitEngine::Character2DControllerComponent& controller, f32 deltaTime);

    void HandleMovement(BitEngine::Character2DControllerComponent& controller, f32 deltaTime);

    void HandleJump(BitEngine::Character2DControllerComponent& controller, f32 deltaTime);
    void UpdateWeaponFocusPoint(BitEngine::Character2DControllerComponent& controller, f32 deltaTime);

    void HandleAttack(BitEngine::Character2DControllerComponent& controller, Enemy* enemy, f32 deltaTime);
    void HandleDead();

    void HandleGravity(BitEngine::Character2DControllerComponent& controller, f32 deltaTime);
    void HandleGroundDetection(BitEngine::TransformComponent& transform, BitEngine::Character2DControllerComponent& controller);
    void ApplyPhysics(BitEngine::TransformComponent& transform, BitEngine::Character2DControllerComponent& controller, f32 deltaTime);
    BMath::Vec2 ResolveTileCollisionSweep(f32 deltaTime, BMath::Vec2 currentPos, BitEngine::BoxCollider2DComponent& boxCollider, BitEngine::TileEditor* tileEditor, BitEngine::Character2DControllerComponent& controller);
    void SetECS(BitEngine::EntityManager* entityManager) { m_ECS = entityManager; }
private:
    BitEngine::EntityManager* m_ECS;
};
