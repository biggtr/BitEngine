#include "PlayerController.h"
#include "Bit/Core/Logger.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/Math/BMath.h"
#include "Bit/Math/Vector.h"
#include "Bit/Physics/Physics2D.h"
#include "EnemyManager.h"
#include <cfloat>

void PlayerController::HandleInput(BitEngine::Character2DControllerComponent& controller, f32 deltaTime)
{
    b8 wasJumpHeld = controller.JumpHeld;
    controller.MoveInput = 0.0f;

    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D))
    {
        controller.MoveInput = 1.0f;
    }
    else if(BitEngine::InputIsKeyDown(BitEngine::KEY_A))
    {
        controller.MoveInput = -1.0f;
    }

    controller.JumpHeld = BitEngine::InputIsKeyDown(BitEngine::KEY_SPACE);
    controller.JumpPressed = controller.JumpHeld && !wasJumpHeld;
    controller.JumpReleased = !controller.JumpHeld && wasJumpHeld;

    if(controller.JumpPressed)
    {
        controller.JumpBufferTimer = controller.JumpBufferTime;
    }
    else if(controller.JumpBufferTimer > 0.0f)
    {
        controller.JumpBufferTimer -= deltaTime;
    }
}

void PlayerController::HandleJump(BitEngine::Character2DControllerComponent& controller, f32 deltaTime)
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
void PlayerController::HandleAttack(BitEngine::Character2DControllerComponent& controller, Enemy* enemy, f32 deltaTime)
{

    if(enemy->Health > 0.0f)
    {
        enemy->Health = BMath::Clamp((enemy->Health - controller.AttackDamage), 0.0f, enemy->MaxHealth);
        enemy->HurtTimer = enemy->HurtDuration;
        enemy->State = ENEMY_STATE::HURT;
    }
}
void PlayerController::UpdateWeaponFocusPoint(BitEngine::Character2DControllerComponent& controller, f32 deltaTime)
{
    BMath::Vec3 targetPoint = controller.WeaponFocusPosition;
    if(controller.AttackRange > 0)
    {
        f32 distance = BMath::Vec3Distance(targetPoint, controller.WeaponFocusPoint);
        // BIT_LOG_DEBUG("distance is %.2f", distance);
        // BIT_LOG_DEBUG("targetpoint %.2f %.2f %.2f", targetPoint.x, targetPoint.y, targetPoint.z);
        // BIT_LOG_DEBUG("weaponfocusPoint %.2f %.2f %.2f", controller.WeaponFocusPoint.x, controller.WeaponFocusPoint.y, controller.WeaponFocusPoint.z);
        if(distance > controller.AttackRange)
        {
            controller.WeaponFocusPoint = BMath::Lerp(targetPoint, controller.WeaponFocusPoint, controller.AttackRange / distance);
            // BIT_LOG_DEBUG("weaponfocusPoint %.2f %.2f %.2f", controller.WeaponFocusPoint.x, controller.WeaponFocusPoint.y, controller.WeaponFocusPoint.z);
        }
    }
    else
    {
        controller.WeaponFocusPoint = targetPoint;
    }

}
void PlayerController::HandleMovement(BitEngine::Character2DControllerComponent& controller, f32 deltaTime)
{
    f32 targetSpeed = controller.MoveInput * controller.MaxSpeed;
    f32 currentSpeed = controller.Velocity.x;
    f32 accelerationRate = controller.Acceleration;
    if(!controller.IsGrounded)
    {
        accelerationRate *= controller.AirControl;
    }

    if((targetSpeed > 0.0f && currentSpeed < 0.0f) || targetSpeed < 0.0f && currentSpeed > 0.0f) // if target speed was going right and i was moving left and turned the other dir accelerate faster to make it smooth
        accelerationRate *= 1.5f;

    if(targetSpeed == 0.0f && controller.IsGrounded)
        accelerationRate = controller.Deceleration;

    f32 speedDiff = targetSpeed - currentSpeed;
    f32 movement = speedDiff * accelerationRate * deltaTime;

    //clamp movement
    if (fabs(movement) > fabs(speedDiff))
        movement = speedDiff;

    controller.Velocity.x += movement;
}

void PlayerController::HandleGravity(BitEngine::Character2DControllerComponent& controller, f32 deltaTime)
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

BMath::Vec2 PlayerController::ResolveTileCollisionSweep(
    f32 deltaTime, BMath::Vec2 currentPos,
    BitEngine::BoxCollider2DComponent& boxCollider,
    BitEngine::TileEditor* tileEditor,
    BitEngine::Character2DControllerComponent& controller)
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

