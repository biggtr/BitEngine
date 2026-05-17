#include "EnemyManager.h"
#include "Bit/Containers/darray.h"
#include "Bit/Core/Logger.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/Animation2DSystem.h"
#include "Bit/ECS/Systems/Physics2DSystem.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/Physics2D.h"
#include "Bit/Physics/PhysicsTypes.h"
#include "Bit/Resources/AssetStore.h"
#include "Bit/Tiles/TileMap.h"
#include <cstdlib>
#include <random>



EnemyManager::EnemyManager(BitEngine::EntityManager* entityManager)
{
    m_EntityManager = entityManager;
    m_Enemies = (BitEngine::Entity*)DArrayCreate(BitEngine::Entity);

    m_Animation2DSystem = m_EntityManager->GetSystem<BitEngine::Animation2DSystem>();
    m_Physics2DSystem = m_EntityManager->GetSystem<BitEngine::Physics2DSystem>();
}

EnemyManager::~EnemyManager()
{
    DArrayDestroy(m_Enemies);
}

BitEngine::Entity EnemyManager::AddEnemy(ENEMY_TYPE type, const BMath::Vec3& position, const BitEngine::Entity& target)
{
    BitEngine::Entity newEnemy;
    newEnemy = m_EntityManager->CreateEntity();
    m_EntityManager->AddComponent<BitEngine::TransformComponent>(newEnemy);
    m_EntityManager->AddComponent<BitEngine::SpriteComponent>(newEnemy);
    m_EntityManager->AddComponent<BitEngine::Rigidbody2DComponent>(newEnemy);
    m_EntityManager->AddComponent<Enemy>(newEnemy);
    
    auto* enemyController = &m_EntityManager->GetComponent<Enemy>(newEnemy);
    enemyController->Type = type;
    enemyController->ID = newEnemy;
    enemyController->Target = target;
    enemyController->State = ENEMY_STATE::IDLE;
    enemyController->MaxSpeed = 100.0f;
    enemyController->Acceleration = 300.0f;
    enemyController->Deceleration = 300.0f;
    enemyController->AirControl = 0.2f;
    enemyController->ChaseRange = 100.0f;
    enemyController->AttackRange = 30.0f;
    enemyController->LoseRange = 80.0f;
    enemyController->AttackCoolDown = 0.0f;
    enemyController->AttackCoolDownMax = 0.0f;
    enemyController->Restitution = 0.3f;
    enemyController->SeparationStrength = 20;
    enemyController->SeparationRadius = 28.0f;
    enemyController->SeparationVelocity = BMath::Vec3(0.0f, 0.0f, 0.0f);
    auto* enemyTransform = &m_EntityManager->GetComponent<BitEngine::TransformComponent>(newEnemy);
    enemyTransform->Position = position;
    enemyTransform->Scale = {10, 10, 1.0f};
    
    auto* enemySprite = &m_EntityManager->GetComponent<BitEngine::SpriteComponent>(newEnemy);
    enemySprite->STexture = BitEngine::AssetStoreGetTexture("CharactersSprite");
    enemySprite->Color = {1,1,1,1};
    enemySprite->CurrentFrame = 16;
    enemySprite->FrameWidth = 16;
    enemySprite->FrameHeight = 16;
    enemySprite->Width = 16 * 8;
    enemySprite->Height = 16 * 8;
    
    auto& enemyRigidBody = m_EntityManager->GetComponent<BitEngine::Rigidbody2DComponent>(newEnemy);
    enemyRigidBody.Position = position;
    enemyRigidBody.Type = BitEngine::PhysicsBodyType::Kinematic;
    m_Physics2DSystem->CreateBoxShape(
        newEnemy,
        16,
        16,
        {0.0f, 0.0f}, 
        0.0f,         
        true,         
        PhysicsCategories::ENEMY,
        (PhysicsCategories)(PhysicsCategories::ENEMY | PhysicsCategories::PLAYER)
    );
    // switch (type) 
    // {
    //     case SUICIDAL:
    //     {
    //     
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SuicidalIdle", 4, 0, 0.1);
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SuicidalRun", 4, 0, 0.1);
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SuicidalAttack", 4, 0, 0.1);
    //     }
    //     case SLIME:
    //     {
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SlimeIdle", 4, 0, 0.1);
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SlimeRun", 4, 0, 0.1);
    //         m_Animation2DSystem->CreateAnimation(newEnemy.ID, "SlimeAttack", 4, 0, 0.1);
    //     }
    // }
    DArrayPush(m_Enemies, newEnemy);
    return newEnemy;
}
void EnemyManager::HandleJump(Enemy& controller, f32 deltaTime)
{
    if(controller.IsGrounded)
    {
        controller.CoyoteTimer = controller.CoyoteTime;
    }
    else if(controller.CoyoteTimer > 0.0f)
    {
        controller.CoyoteTimer -= deltaTime;
    }

    b8 canJump = false;
    
    if(controller.JumpCount  == 0)
        canJump = controller.CoyoteTimer > 0.0f;
    else
        canJump = controller.JumpCount < controller.MaxJumps;
    if(controller.JumpBufferTimer > 0.0f && canJump)
    {
        controller.Velocity.y = controller.JumpForce;
        controller.IsJumping = true;
        controller.JumpCount++;

        controller.JumpBufferTimer = 0.0f;
        controller.CoyoteTimer = 0.0f;
    }

    if(controller.JumpReleased && controller.Velocity.y > 0.0f)
    {
        controller.Velocity.y *= 0.5f;
    }
}
BMath::Vec2 EnemyManager::ResolveTileCollisionSweep(
    f32 deltaTime, BMath::Vec2 currentPos,
    BitEngine::BoxCollider2DComponent& boxCollider,
    BitEngine::TileEditor* tileEditor,
    Enemy& controller)
{
    f32 tileSize  = (f32)tileEditor->GetTileMap()->GetTileSize();
    f32 epsilon   = 0.01f;

    BMath::Vec2 pos = currentPos + boxCollider.offset;

    controller.IsGrounded    = false;
    controller.CollidingLeft  = false;
    controller.CollidingRight = false;
    controller.CollidingAbove = false;
    controller.CollidingBelow = false;

    const f32 halfW = boxCollider.Width  * 0.5f;
    const f32 halfH = boxCollider.Height * 0.5f;

    f32 desiredX = pos.x + controller.Velocity.x * deltaTime;

    if (controller.Velocity.x != 0.0f)
    {
        i32 minTileX = BMath::Floor((BMath::Min(pos.x, desiredX) - halfW) / tileSize);
        i32 maxTileX = BMath::Floor((BMath::Max(pos.x, desiredX) + halfW) / tileSize);
        i32 minTileY = BMath::Floor((pos.y - halfH + epsilon) / tileSize);
        i32 maxTileY = BMath::Floor((pos.y + halfH - epsilon) / tileSize);

        bool hitWall = false;
        f32 bestWall = (controller.Velocity.x > 0.0f) ? FLT_MAX : -FLT_MAX;

        for (i32 ty = minTileY; ty <= maxTileY; ty++)
        {
            for (i32 tx = minTileX; tx <= maxTileX; tx++)
            {
                if (!tileEditor->IsTileSolid(tx, ty)) continue;

                if (controller.Velocity.x > 0.0f)
                {
                    f32 tileLeft     = (f32)tx * tileSize;
                    f32 currentRight = pos.x + halfW;

                    if (tileLeft < currentRight - epsilon) continue;

                    f32 playerRight = desiredX + halfW;
                    if (playerRight > tileLeft && tileLeft < bestWall)
                    {
                        bestWall = tileLeft;
                        hitWall  = true;
                    }
                }
                else 
                {
                    f32 tileRight   = (f32)(tx + 1) * tileSize;
                    f32 currentLeft = pos.x - halfW;

                    if (tileRight > currentLeft + epsilon) continue;

                    f32 playerLeft = desiredX - halfW;
                    if (playerLeft < tileRight && tileRight > bestWall)
                    {
                        bestWall = tileRight;
                        hitWall  = true;
                    }
                }
            }
        }

        if (hitWall)
        {
            if (controller.Velocity.x > 0.0f)
            {
                desiredX = bestWall - halfW - epsilon;
                controller.CollidingRight = true;
            }
            else
            {
                desiredX = bestWall + halfW + epsilon;
                controller.CollidingLeft = true;
            }

            controller.Velocity.x = 0.0f;
        }
    }

    pos.x = desiredX;

    f32 desiredY = pos.y + controller.Velocity.y * deltaTime;

    {
        i32 minTileX = BMath::Floor((pos.x - halfW + epsilon) / tileSize);
        i32 maxTileX = BMath::Floor((pos.x + halfW - epsilon) / tileSize);
        i32 minTileY = BMath::Floor((BMath::Min(pos.y, desiredY) - halfH) / tileSize);
        i32 maxTileY = BMath::Floor((BMath::Max(pos.y, desiredY) + halfH) / tileSize);

        bool hitFloor   = false;
        bool hitCeiling = false;
        f32  bestFloor   = -FLT_MAX;
        f32  bestCeiling =  FLT_MAX;

        for (i32 ty = minTileY; ty <= maxTileY; ty++)
        {
            for (i32 tx = minTileX; tx <= maxTileX; tx++)
            {
                if (!tileEditor->IsTileSolid(tx, ty)) continue;

                f32 tileLeft   = (f32)tx       * tileSize;
                f32 tileRight  = (f32)(tx + 1) * tileSize;
                f32 tileTop    = (f32)(ty + 1) * tileSize;
                f32 tileBottom = (f32)ty        * tileSize;

                if ((pos.x + halfW) <= tileLeft  + epsilon) continue;
                if ((pos.x - halfW) >= tileRight - epsilon) continue;

                if (controller.Velocity.y <= 0.0f)
                {
                    f32 currentBottom = pos.y - halfH;

                    if (tileTop > currentBottom + epsilon) continue;

                    f32 playerBottom = desiredY - halfH;
                    if (playerBottom <= tileTop && tileTop > bestFloor)
                    {
                        bestFloor = tileTop;
                        hitFloor  = true;
                    }
                }

                if (controller.Velocity.y > 0.0f)
                {
                    f32 currentTop = pos.y + halfH;

                    if (tileBottom < currentTop - epsilon) continue;

                    f32 playerTop = desiredY + halfH;
                    if (playerTop >= tileBottom && tileBottom < bestCeiling)
                    {
                        bestCeiling = tileBottom;
                        hitCeiling  = true;
                    }
                }
            }
        }

        if (hitFloor)
        {
            desiredY = bestFloor + halfH + epsilon;
            if(controller.Velocity.y < 0.0)
            {
                controller.Velocity.y = -controller.Velocity.y * controller.Restitution;
            }
            controller.IsGrounded = true;
            controller.CollidingBelow = true;
            controller.IsJumping  = false;
            controller.JumpCount  = 0;
        }

        if (hitCeiling)
        {
            desiredY = bestCeiling - halfH - epsilon;
            if(controller.Velocity.y > 0.0)
            {
                controller.Velocity.y = -controller.Velocity.y * controller.Restitution;
            }
            controller.CollidingAbove = true;
        }
    }

    pos.y = desiredY;

    return pos - boxCollider.offset;
}

void EnemyManager::HandleGravity(Enemy& controller, f32 deltaTime)
{
    if (controller.IsGrounded && controller.Velocity.y <= 0.0f)
    {
        controller.Velocity.y = 0.0f;
        controller.IsJumping = false;
        return;
    }
    
    f32 gravity = (controller.Velocity.y > 0.0f) ? controller.RisingGravity : controller.FallingGravity;

    controller.Velocity.y -= gravity * deltaTime;

    if (controller.Velocity.y < -controller.TerminalVelocity)
        controller.Velocity.y = -controller.TerminalVelocity;
}
void EnemyManager::HandleMovement(Enemy& enemy, f32 deltaTime)
{
    f32 targetSpeed = enemy.MoveInput * enemy.MaxSpeed;
    f32 currentSpeed = enemy.Velocity.x;
    f32 accelerationRate = enemy.Acceleration;

    if(!enemy.IsGrounded)
    {
        accelerationRate *= enemy.AirControl;
    }
    
    if((targetSpeed > 0.0f && currentSpeed < 0.0f) || (targetSpeed < 0.0f && currentSpeed > 0.0f))
    {
        accelerationRate *= 1.5;
    }

    if(targetSpeed == 0.0f && enemy.IsGrounded)
    {
        accelerationRate = enemy.Deceleration;
    }

    f32 speedDiff = targetSpeed - currentSpeed;
    f32 movement = speedDiff * accelerationRate * deltaTime;    

    if(BMath::Abs(movement) > BMath::Abs(speedDiff))
    {
        movement = speedDiff;
    }

    enemy.Velocity.x += movement;
}
void EnemyManager::HandleAttack(Enemy& enemy, f32 deltaTime)
{
    BitEngine::TransformComponent& targetTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.Target);
    BitEngine::TransformComponent& enemyTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.ID);
    enemy.MoveInput = 0;

    BMath::Vec3 directionToTarget = targetTransform.Position - enemyTransform.Position;
    f32 distanceToTarget = BMath::Vec3Length(directionToTarget);

    if(distanceToTarget > enemy.AttackRange)
    {
        BIT_LOG_DEBUG("going to chase");
        enemy.State = ENEMY_STATE::CHASE;
        return;
    }

    if(enemy.AttackCoolDown <= 0.0f)
    {
        enemy.ShouldAttack = true;
        HandleDamagingPlayer(enemy);
    }
    else
    {
    }
}
void EnemyManager::HandleDamagingPlayer(Enemy& enemy)
{
    BitEngine::Character2DControllerComponent& characterController = m_EntityManager->GetComponent<BitEngine::Character2DControllerComponent>(enemy.Target);

    if(characterController.Health > 0.0f)
    {
        // BIT_LOG_DEBUG("health : %.2f", characterController.Health);
        characterController.Health = BMath::Clamp((characterController.Health - 10.0f), 0.0f, characterController.MaxHealth);
    }
    enemy.AttackCoolDown = enemy.AttackCoolDownMax;
    enemy.ShouldAttack = false;
}
void EnemyManager::HandlePatrol(Enemy& enemy)
{

}
void EnemyManager::HandleChase(Enemy& enemy, f32 deltaTime)
{

    BitEngine::TransformComponent& targetTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.Target);
    BitEngine::TransformComponent& enemyTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.ID);

    BMath::Vec3 directionToTarget = targetTransform.Position - enemyTransform.Position;
    f32 distanceToTarget = BMath::Vec3Length(directionToTarget);

    if(distanceToTarget > enemy.LoseRange)
    {
        BIT_LOG_DEBUG("going to lose");
        enemy.State = ENEMY_STATE::IDLE;
        return;
    }
    // BIT_LOG_DEBUG("distanceToTarget %.2f", distanceToTarget);
    // BIT_LOG_DEBUG("attack range %.2f", enemy.AttackRange);

    if(distanceToTarget <= enemy.AttackRange && enemy.AttackCoolDown <= 0.0f)
    {
        BIT_LOG_DEBUG("going to attack");
        enemy.State = ENEMY_STATE::ATTACK;
        return;
    }

    f32 distX = BMath::Abs(directionToTarget.x);
    f32 distY = BMath::Abs(directionToTarget.y);
    if(distY > 30.0f)
    {
        enemy.MoveInput = 0.0f;
        return;
    }
    if(distX > 0.01f)
    {
        f32 rampStart = 50.0f;
        f32 rampEnd   = 10.0f;
        f32 t = (distX - rampEnd) / (rampStart - rampEnd);
        t = BMath::Clamp(t, 0.0f, 1.0f);
        f32 desiredInput = (directionToTarget.x > 0.0f) ? 1.0f : -1.0f;
        enemy.MoveInput = desiredInput * t;
        if((enemy.CollidingRight || enemy.CollidingLeft) && enemy.IsGrounded)
        {
            HandleJump(enemy, deltaTime);
        }
    }
    else
    {
        enemy.MoveInput = 0.0f;
    }
}
void EnemyManager::HandleIdle(Enemy& enemy)
{
    BitEngine::TransformComponent& targetTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.Target);
    BitEngine::TransformComponent& enemyTransform = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemy.ID);

    BMath::Vec3 directionToTarget = targetTransform.Position - enemyTransform.Position;
    f32 distanceToTarget = BMath::Vec3Length(directionToTarget);
    
    if(distanceToTarget <= enemy.ChaseRange && (!enemy.CollidingRight || !enemy.CollidingLeft))
    {
        enemy.State = ENEMY_STATE::CHASE;
        return;
    }

    enemy.MoveInput = 0.0f;
}
void EnemyManager::HandleHurt(Enemy& enemy)
{

}
void EnemyManager::HandleDead(Enemy& enemy)
{

}

void EnemyManager::Update(f32 deltaTime, BitEngine::TileEditor* tileEditor)
{
    for (u32 i = 0; i < DArrayLength(m_Enemies); ++i)
    {
        BitEngine::Entity enemyID = m_Enemies[i];
        auto& enemyController = enemyID.GetComponent<Enemy>();

        b8 wasJumpHeld = enemyController.JumpHeld;
        b8 shouldJump = (enemyController.CollidingLeft || enemyController.CollidingRight) && enemyController.IsGrounded;
        enemyController.JumpPressed = shouldJump && !wasJumpHeld;
        enemyController.JumpReleased = !enemyController.JumpHeld && wasJumpHeld;
        if(enemyController.JumpPressed)
        {
        enemyController.JumpBufferTimer = enemyController.JumpBufferTime;
        }
        else if(enemyController.JumpBufferTimer > 0.0f)
        {
           enemyController.JumpBufferTimer -= deltaTime;
        }
        switch (enemyController.State)
        {
            case IDLE:    HandleIdle(enemyController);  break;
            case ATTACK:  HandleAttack(enemyController, deltaTime); break;
            case PATROL:  HandlePatrol(enemyController); break;
            case CHASE:   HandleChase(enemyController, deltaTime); break;
            case HURT:    HandleHurt(enemyController); break;
            case DEAD:    HandleDead(enemyController); break;
        }
        if(enemyController.ShouldAttack)
        {
            HandleDamagingPlayer(enemyController);
        }
        else
        {
            enemyController.AttackCoolDown -= deltaTime;
        }

    
        HandleMovement(enemyController, deltaTime);
        HandleGravity(enemyController, deltaTime);
    }

    for (u32 i = 0; i < DArrayLength(m_Enemies); ++i)
    {
        BitEngine::Entity enemyA = m_Enemies[i];
        auto& transformA = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemyA);
        auto& controllerA = m_EntityManager->GetComponent<Enemy>(enemyA);

        BMath::Vec3 separationForce = BMath::Vec3Zero();

        for (u32 j = 0; j < DArrayLength(m_Enemies); ++j)
        {
            if (i == j) continue;

            BitEngine::Entity enemyB = m_Enemies[j];
            auto& transformB = m_EntityManager->GetComponent<BitEngine::TransformComponent>(enemyB);
            auto& controllerB = m_EntityManager->GetComponent<Enemy>(enemyB);

            BMath::Vec3 diff = transformA.Position - transformB.Position;
            f32 dist = BMath::Vec3Length(diff);
            f32 minDist = (controllerA.SeparationRadius + controllerB.SeparationRadius) * 0.5f;

            if (dist < minDist && dist > 0.01f)
            {
                BMath::Vec3 dir = diff / dist;
                f32 overlap = minDist - dist;

                f32 force = overlap * controllerA.SeparationStrength;
                force = BMath::Min(force, controllerA.MaxSpeed * 0.3f);

                dir.y = 0.0f;
                separationForce += dir * force;
            }
        }

        controllerA.SeparationVelocity = separationForce;
    }

    for (u32 i = 0; i < DArrayLength(m_Enemies); ++i)
    {
        BitEngine::Entity enemyID = m_Enemies[i];
        auto& rigidBody = enemyID.GetComponent<BitEngine::Rigidbody2DComponent>();
        auto& controller = enemyID.GetComponent<Enemy>();
        auto& transform = enemyID.GetComponent<BitEngine::TransformComponent>();

        controller.Velocity.x += controller.SeparationVelocity.x;
        controller.SeparationVelocity = BMath::Vec3Zero();

        controller.Velocity.x = BMath::Clamp(controller.Velocity.x, -controller.MaxSpeed, controller.MaxSpeed);

        auto& boxCollider = rigidBody.MultiColliderComponents[0].BoxCollider2D;
        BMath::Vec2 currentPos = BitEngine::Physics2DGetPosition(rigidBody.BodyId);
        BMath::Vec2 finalPos = ResolveTileCollisionSweep(deltaTime, currentPos, boxCollider, tileEditor, controller);

        transform.Position.x = finalPos.x;
        transform.Position.y = finalPos.y;
        rigidBody.Position = transform.Position;
        BitEngine::Physics2DSetPosition(rigidBody.BodyId, finalPos);
    }
}
void EnemyManager::KillEnemy(BitEngine::Entity enemy)
{
    m_EntityManager->KillEntity(enemy);
}
