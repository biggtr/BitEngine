#include "PlayerController.h"
#include "Bit/Physics/Physics2D.h"
void PlayerController::UpdateGroundedState(BitEngine::Rigidbody2DComponent& rigidBody, 
                                    Character2DControllerComponent& controller,
                                    BitEngine::BoxCollider2DComponent& boxCollider,
                                    const BMath::Vec2& currentPos)
{
    const float groundCheckDistance = 0.2f;
    
    BMath::Vec2 groundRayStart = {currentPos.x + boxCollider.offset.x,
        currentPos.y + boxCollider.offset.y - boxCollider.Height * 0.5f};
    BMath::Vec2 groundRayEnd   = {currentPos.x, groundRayStart.y - groundCheckDistance};
    BMath::Vec2 groundRayVector = groundRayEnd - groundRayStart;
    
    BitEngine::CastRayContext groundContext = BitEngine::Physics2DCastRay(groundRayStart, groundRayVector, rigidBody.PrimaryId);
    
    if(b2Shape_IsValid(groundContext.ShapeID) && 
       groundContext.Fraction >= 0.0f && 
       groundContext.Fraction < 1.0f &&
       groundContext.Normal.y > 0.5f)
    {
        controller.IsGrounded = true;
    }
    else
    {
        controller.IsGrounded = false;
    }
}
void PlayerController::HandleInput(Character2DControllerComponent& controller, f32 deltaTime)
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

void PlayerController::HandleJump(Character2DControllerComponent& controller, f32 deltaTime)
{
    if(controller.IsGrounded)
    {
        controller.CoyoteTimer = controller.CoyoteTime;
    }
    else if(controller.CoyoteTimer > 0.0f)
    {
        controller.CoyoteTimer -= deltaTime;
    }

    b8 canJump = (controller.CoyoteTimer > 0.0f && controller.JumpCount == 0) || 
                 (controller.JumpCount < controller.MaxJumps);

    if(controller.JumpBufferTimer > 0.0f && canJump && controller.JumpHeld)
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

void PlayerController::HandleMovement(Character2DControllerComponent& controller, f32 deltaTime)
{
    f32 targetSpeed = controller.MoveInput * controller.MaxSpeed;
    f32 currentSpeed = controller.Velocity.x;
    f32 accelerationRate = controller.Acceleration;

    if(!controller.IsGrounded)
    {
        accelerationRate *= controller.AirControl;
    }

    if(targetSpeed > 0.0f && currentSpeed < 0.0f || targetSpeed < 0.0f && currentSpeed > 0.0f) // if target speed was going right and i was moving left and turned the other dir accelerate faster to make it smooth
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

void PlayerController::HandleGravity(Character2DControllerComponent& controller, f32 deltaTime)
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
BMath::Vec2 PlayerController::HandleKinematicCollisions(f32 deltaTime, 
                                                  BitEngine::Rigidbody2DComponent& rigidBody, 
                                                  Character2DControllerComponent& controller,
                                                  BitEngine::BoxCollider2DComponent& boxCollider,
                                                  const BMath::Vec2& currentPos,
                                                  const BMath::Vec2& desiredPos)
{
    bool wasGrounded = controller.IsGrounded;
    controller.IsGrounded = false;
    BMath::Vec2 finalPos = desiredPos;
    
    const f32 groundEpsilon = 0.01f;
    const f32 groundCheckDistance = 0.2f;
    
    if(!wasGrounded && controller.Velocity.y < -5.0f)
    {
        BMath::Vec2 sweepStart = {currentPos.x + boxCollider.offset.x, currentPos.y + boxCollider.offset.y - boxCollider.Height * 0.5f};
        BMath::Vec2 sweepEnd   = {desiredPos.x + boxCollider.offset.x, desiredPos.y + boxCollider.offset.y - boxCollider.Height * 0.5f};
        BMath::Vec2 sweepVector = sweepEnd - sweepStart;
        
        if(fabs(sweepVector.y) > 0.1f)
        {
            BitEngine::CastRayContext sweepContext = BitEngine::Physics2DCastRay(sweepStart, sweepVector, rigidBody.PrimaryId);
            
            if(b2Shape_IsValid(sweepContext.ShapeID) && 
               sweepContext.Fraction > 0.0f && 
               sweepContext.Fraction <= 1.0f &&
               sweepContext.Normal.y > 0.5f)
            {
                f32 hitGroundY = sweepContext.Point.y;
                finalPos.y = hitGroundY - boxCollider.offset.y + boxCollider.Height * 0.5f + groundEpsilon;
                finalPos.x = sweepStart.x + (sweepEnd.x - sweepStart.x) * sweepContext.Fraction;
                
                controller.Velocity.y = 0.0f;
                controller.IsGrounded = true;
                controller.JumpCount = 0;
                
                return finalPos;
            }
        }
    }
    
    BMath::Vec2 groundRayStart = {finalPos.x + boxCollider.offset.x, finalPos.y + boxCollider.offset.y - boxCollider.Height * 0.5f};
    BMath::Vec2 groundRayEnd   = {finalPos.x, groundRayStart.y - groundCheckDistance};
    BMath::Vec2 groundRayVector = groundRayEnd - groundRayStart;
    
    BitEngine::CastRayContext groundContext = BitEngine::Physics2DCastRay(groundRayStart, groundRayVector, rigidBody.PrimaryId);
    
    if(b2Shape_IsValid(groundContext.ShapeID) && 
       groundContext.Fraction >= 0.0f && 
       groundContext.Fraction < 1.0f &&
       groundContext.Normal.y > 0.5f)
    {
        float groundY = groundContext.Point.y;
        float standingY = groundY - boxCollider.offset.y + boxCollider.Height * 0.5f + groundEpsilon;
        float distanceToGround = finalPos.y - standingY;
        
        if(distanceToGround >= -0.05f && distanceToGround <= groundCheckDistance)
        {
            if(wasGrounded && fabs(controller.Velocity.y) < 0.1f && fabs(distanceToGround) < 0.1f)
            {
                controller.IsGrounded = true;
                controller.JumpCount = 0;
                controller.Velocity.y = 0.0f;
            }
            else
            {
                finalPos.y = standingY;
                controller.IsGrounded = true;
                controller.JumpCount = 0;
                controller.Velocity.y = 0.0f;
            }
        }
    }
    
    if(controller.Velocity.y > 0.5f)
    {
        BMath::Vec2 ceilingRayStart = {finalPos.x + boxCollider.offset.x, finalPos.y + boxCollider.offset.y + boxCollider.Height * 0.5f};
        BMath::Vec2 ceilingRayEnd   = {finalPos.x, ceilingRayStart.y + 0.2f};
        BMath::Vec2 ceilingRayVector = ceilingRayEnd - ceilingRayStart;
        
        BitEngine::CastRayContext ceilingContext = BitEngine::Physics2DCastRay(ceilingRayStart, ceilingRayVector, rigidBody.PrimaryId);
        
        if(b2Shape_IsValid(ceilingContext.ShapeID) && 
           ceilingContext.Fraction > 0.0f && 
           ceilingContext.Normal.y < -0.5f)
        {
            float ceilingY = ceilingContext.Point.y;
            finalPos.y = ceilingY - boxCollider.offset.y - boxCollider.Height * 0.5f - groundEpsilon;
            controller.Velocity.y = 0.0f;
        }
    }
    
    return finalPos;
}
// void PlayerController::HandleCollision(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller)
// {
//     controller.CollidingBelow = false;
//     controller.CollidingAbove = false;
//     controller.CollidingLeft = false;
//     controller.CollidingRight = false;
//
//
//     std::vector<BitEngine::TileCollisionInfo> tileCollisions;
//     m_TileEditor->GetTileCollisions(transform.Position, controller.Width, controller.Height, 
//                                     tileCollisions, 0);
//     for (auto& tileCol : tileCollisions)
//     {
//         transform.Position.x += tileCol.Normal.x * tileCol.Depth;
//         transform.Position.y += tileCol.Normal.y * tileCol.Depth;
//
//
//         float velocityAlongNormal = controller.Velocity.x * tileCol.Normal.x + 
//                                    controller.Velocity.y *  tileCol.Normal.y;
//
//         if (velocityAlongNormal < 0.0f)  
//         {
//             controller.Velocity.x -= velocityAlongNormal * tileCol.Normal.x;
//             controller.Velocity.y -= velocityAlongNormal * tileCol.Normal.y;
//         }
//
//         if (tileCol.Normal.y > 0.7f)  
//         {
//             controller.CollidingBelow = true;
//         }
//         else if (tileCol.Normal.y < -0.7f)
//         {
//             controller.CollidingAbove = true;
//         }
//
//         if (tileCol.Normal.x > 0.7f) 
//         {
//             controller.CollidingLeft = true;
//         }
//         else if (tileCol.Normal.x < -0.7f)  
//         {
//             controller.CollidingRight = true;
//         }
//
//     }
// }



