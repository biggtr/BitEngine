#include "Dystopia.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/System.h"
#include "Bit/Physics/Physics2D.h"

void Dystopia::Initialize()
{
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();
    player.AddComponent<BitEngine::SpriteComponent>();
    player.AddComponent<Character2DControllerComponent>();

    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    m_AssetManager->AddTexture("samuri", "assets/textures/samuri.png");
    playerSprite.STexture = m_AssetManager->GetTexture("samuri");
    playerSprite.Width = 21 * 32;
    playerSprite.Height = 64;
    playerSprite.FrameWidth = 32;
    playerSprite.FrameHeight = 32;
    playerSprite.CurrentFrame = 0;
    playerSprite.IsUI = false;
    
    f32 width = playerSprite.FrameWidth;
    f32 height = playerSprite.FrameHeight;
    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 100.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {width, height, 1.0f};

    auto& playerRigidBody = player.AddComponent<BitEngine::Rigidbody2DComponent>();
    playerRigidBody.Position = {0.0f, 100.0f, 0.0f};
    playerRigidBody.Type  = BitEngine::PhysicsBodyType::Kinematic;


    // auto& playerCollider = player.AddComponent<BitEngine::CapsuleCollider2DComponent>();
    // playerCollider.center1 = {0.0f, 10.0f};
    // playerCollider.center2 = { 0.0f, -10.0f};
    // playerCollider.radius = 2.0f;
    
    auto& playerCollider = player.AddComponent<BitEngine::BoxCollider2DComponent>();
    playerCollider.Width = width * 0.4f; 
    playerCollider.Height = height * 0.4f; 
    playerCollider.offset = {0.0f, -5.0f};
    playerCollider.DebugColor = {1.0f, 1.0f, 0.0f, 1.0f};


    player.AddComponent<BitEngine::Animation2DControllerComponent>();
    m_Animation2DSystem->CreateAnimation(player, "PlayerRun", 16, 0, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerAttack", 7, 22, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerDamaged", 4, 29, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerIdle", 10, 29 + 4, 0.07f);


    auto floor = m_ECS->CreateEntity();
    
    auto& floorTransform = floor.AddComponent<BitEngine::TransformComponent>();
    floorTransform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);
    floorTransform.Scale = BMath::Vec3(30.0f, 10.0f, 0.0f);
    
    auto& floorRigidBody = floor.AddComponent<BitEngine::Rigidbody2DComponent>();
    floorRigidBody.Position = BMath::Vec3(0.0f, 0.0f, 0.0f);
    floorRigidBody.Type = BitEngine::PhysicsBodyType::Static;

    auto& floorCollider = floor.AddComponent<BitEngine::BoxCollider2DComponent>();
    floorCollider.Width = 200.0f;
    floorCollider.Height = 10.0f;
    floorCollider.DebugColor = {0.0f, 0.0f, 1.0f, 1.0f};
    BIT_LOG_INFO("Floor bounds minY=%f maxY=%f", floorRigidBody.Position.y - floorCollider.Height*0.5f, floorRigidBody.Position.y + floorCollider.Height*0.5f);
    auto& floorSprite = floor.AddComponent<BitEngine::SpriteComponent>();
    floorSprite.Color = BMath::Vec4(0.0f, 1.0f, 0.0f, 1.0f);
    


    m_AssetManager->AddTexture("dystopiaTileset", "assets/textures/dystopiatiles.png");

    BitEngine::Texture* tilesetTexture = m_AssetManager->AddTexture("tileset", "assets/textures/tileset.png");
    BitEngine::TileSet* tileset = m_TileEditor->CreateTileSet(tilesetTexture, 320.0f, 320.0f, 32, 32);

    BIT_LOG_INFO("Floor created at y=%f with height=%f (bounds: %f to %f)", 
    floorRigidBody.Position.y, 
    floorCollider.Height,
    floorRigidBody.Position.y - floorCollider.Height * 0.5f,
    floorRigidBody.Position.y + floorCollider.Height * 0.5f);

    // BIT_LOG_INFO("Player created at y=%f with height=%f", 
    // playerRigidBody.Position.y,
    // playerCollider.Height);

    m_TileEditor->CreateTileMap("Level_1", 32, 32, 32);

}

void Dystopia::RenderUI()
{
}
void Dystopia::Render2D()
{
}

void Dystopia::Render3D()
{
}
void Dystopia::Update(f32 deltaTime)
{
    auto& transform = player.GetComponent<BitEngine::TransformComponent>();
    auto& controller = player.GetComponent<Character2DControllerComponent>();
    auto& rigidbody = player.GetComponent<BitEngine::Rigidbody2DComponent>();
    auto& boxCollider = player.GetComponent<BitEngine::BoxCollider2DComponent>();

    BMath::Vec2 currentPos = m_Physics2D->GetPosition(rigidbody.BodyId);
    UpdateGroundedState(rigidbody, controller, boxCollider, currentPos);
    
    HandleInput(controller, deltaTime);
    HandleMovement(controller, deltaTime);
    HandleJump(controller, deltaTime);
    HandleGravity(controller, deltaTime);

    BMath::Vec2 desiredMove = BMath::Vec2(controller.Velocity.x, controller.Velocity.y) * deltaTime;
    BMath::Vec2 desiredPos = currentPos + desiredMove;
    
    BMath::Vec2 finalPos = HandleKinematicCollisions(deltaTime, rigidbody, controller, boxCollider, currentPos, desiredPos);
    
    m_Physics2D->SetPosition(rigidbody.BodyId, finalPos);
    rigidbody.Position = BMath::Vec3(finalPos.x, finalPos.y, rigidbody.Position.z);
    
    transform.Position.x = finalPos.x;
    transform.Position.y = finalPos.y;
    
    UpdateAnimation(controller, transform);
}

void Dystopia::UpdateGroundedState(BitEngine::Rigidbody2DComponent& rigidBody, 
                                    Character2DControllerComponent& controller,
                                    BitEngine::BoxCollider2DComponent& boxCollider,
                                    const BMath::Vec2& currentPos)
{
    const float groundCheckDistance = 0.2f;
    
    BMath::Vec2 groundRayStart = {currentPos.x + boxCollider.offset.x,
        currentPos.y + boxCollider.offset.y - boxCollider.Height * 0.5f};
    BMath::Vec2 groundRayEnd   = {currentPos.x, groundRayStart.y - groundCheckDistance};
    BMath::Vec2 groundRayVector = groundRayEnd - groundRayStart;
    
    BitEngine::CastRayContext groundContext = m_Physics2D->CastRay(groundRayStart, groundRayVector, rigidBody.ShapeId);
    
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
void Dystopia::HandleInput(Character2DControllerComponent& controller, f32 deltaTime)
{
    b8 wasJumpPressed = controller.JumpPressed;
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
    controller.JumpPressed = controller.JumpHeld && !wasJumpPressed;
    controller.JumpReleased = !controller.JumpHeld && wasJumpPressed;

    if(controller.JumpPressed)
    {
        controller.JumpBufferTimer = controller.JumpBufferTime;
    }
    else if(controller.JumpBufferTimer > 0.0f)
    {
        controller.JumpBufferTimer -= deltaTime;
    }
    
    if(controller.IsGrounded && !controller.JumpHeld && controller.JumpBufferTimer > 0.0f)
    {
        controller.JumpBufferTimer = 0.0f;
    }
}

void Dystopia::HandleJump(Character2DControllerComponent& controller, f32 deltaTime)
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

void Dystopia::HandleMovement(Character2DControllerComponent& controller, f32 deltaTime)
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

void Dystopia::HandleGravity(Character2DControllerComponent& controller, f32 deltaTime)
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
BMath::Vec2 Dystopia::HandleKinematicCollisions(f32 deltaTime, 
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
            BitEngine::CastRayContext sweepContext = m_Physics2D->CastRay(sweepStart, sweepVector, rigidBody.ShapeId);
            
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
    
    BitEngine::CastRayContext groundContext = m_Physics2D->CastRay(groundRayStart, groundRayVector, rigidBody.ShapeId);
    
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
        
        BitEngine::CastRayContext ceilingContext = m_Physics2D->CastRay(ceilingRayStart, ceilingRayVector, rigidBody.ShapeId);
        
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
// void Dystopia::HandleCollision(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller)
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


void Dystopia::UpdateAnimation(Character2DControllerComponent& controller, BitEngine::TransformComponent& transform)
{
    if (controller.MoveInput > 0.0f)
        transform.Scale.x = abs(transform.Scale.x);
    else if (controller.MoveInput < 0.0f)
        transform.Scale.x = -abs(transform.Scale.x); 
    
    if (!controller.IsGrounded)
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerIdle");  // jump animation 
    }
    else if (abs(controller.Velocity.x) > 10.0f)
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }
    else
    {
        m_Animation2DSystem->SetAnimation(player, "PlayerIdle");
    }
}
