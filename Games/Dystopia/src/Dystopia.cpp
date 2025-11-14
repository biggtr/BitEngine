#include "Dystopia.h"
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/Matrix.h"
#include "Bit/Math/Vector.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/Systems/InputSystem.h"
#include "Bit/ECS/Systems/System.h"

void Dystopia::Initialize()
{
    player = m_ECS->CreateEntity();
    player.AddComponent<BitEngine::TransformComponent>();

    auto& playerTransform = player.GetComponent<BitEngine::TransformComponent>();
    playerTransform.Position = BMath::Vec3(0.0f, 40.0f, -5.0f);
    playerTransform.Rotation = BMath::Vec3(0.0f, 0.0f, 0.0f);
    playerTransform.Scale = {35.0f, 35.0f, 1.0f};

    player.AddComponent<BitEngine::SpriteComponent>();
    auto& playerSprite = player.GetComponent<BitEngine::SpriteComponent>();
    m_AssetManager->AddTexture("samuri", "assets/textures/samuri.png");
    playerSprite.STexture = m_AssetManager->GetTexture("samuri");
    playerSprite.Width = 21 * 32;
    playerSprite.Height = 64;
    playerSprite.FrameWidth = 32;
    playerSprite.FrameHeight = 32;
    playerSprite.CurrentFrame = 0;
    playerSprite.IsUI = false;

    player.AddComponent<BitEngine::Animation2DControllerComponent>();
    m_Animation2DSystem->CreateAnimation(player, "PlayerRun", 16, 0, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerAttack", 7, 22, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerDamaged", 4, 29, 0.07f);
    m_Animation2DSystem->CreateAnimation(player, "PlayerIdle", 10, 29 + 4, 0.07f);

    player.AddComponent<Character2DControllerComponent>();
    auto& collision = player.AddComponent<CollisionComponent>();
    collision.Width = 28.0f;
    collision.Height = 32.0f;


    player.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f, BitEngine::BODY_KINEMATIC);
    
    auto& playerCollider = player.AddComponent<BitEngine::Box2DColliderComponent>();
    playerCollider.Size = BMath::Vec3(28.0f, 32.0f, 0.0f);
    
    auto floor = m_ECS->CreateEntity();
    
    auto& floorTransform = floor.AddComponent<BitEngine::TransformComponent>();
    floorTransform.Position = BMath::Vec3(0.0f, 0.0f, -5.0f);
    floorTransform.Scale = BMath::Vec3(50.0f, 10.0f, 0.0f);
    
    auto& floorSprite = floor.AddComponent<BitEngine::SpriteComponent>();
    floorSprite.Color = BMath::Vec4(0.0f, 1.0f, 0.0f, 1.0f);
    
    floor.AddComponent<BitEngine::Rigid2DBodyComponent>(0.0f, BitEngine::BODY_STATIC);
    
    auto& floorCollider = floor.AddComponent<BitEngine::Box2DColliderComponent>();
    floorCollider.Size = BMath::Vec3(500.0f, 10.0f, 0.0f);

    m_AssetManager->AddTexture("dystopiaTileset", "assets/textures/dystopiatiles.png");

    BitEngine::Texture* tilesetTexture = m_AssetManager->AddTexture("tileset", "assets/textures/tileset.png");
    BitEngine::TileSet* tileset = m_TileEditor->CreateTileSet(tilesetTexture, 320.0f, 320.0f, 32, 32);
            

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
    auto& collision = player.GetComponent<CollisionComponent>();

    HandleInput(controller, deltaTime);

    HandleMovement(controller, deltaTime);

    HandleJump(controller, deltaTime);

    HandleGravity(controller, deltaTime);


    HandleCollision(transform, controller, collision);
    m_Physics2DSystem->SetKinematicVelocity(player, controller.Velocity);
    auto& rigidbody = player.GetComponent<BitEngine::Rigid2DBodyComponent>();
    BitEngine::BBody& body = BitEngine::BPhysics2DGetBody(rigidbody.BodyIndex);
    body.Position = transform.Position;
    
    m_Physics2DSystem->SetKinematicVelocity(player, controller.Velocity);
    HandleGroundDetection(transform, controller, collision);

    UpdateAnimation(controller, transform);
            
    auto& playertransform = player.GetComponent<BitEngine::TransformComponent>();
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D))
    {
        playertransform.Scale.x *= playertransform.Scale.x < 0 ? -1 : 1; 
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_A))
    {
        playertransform.Scale.x *= playertransform.Scale.x > 0 ? -1 : 1;
        m_Animation2DSystem->SetAnimation(player, "PlayerRun");
    }

}

void Dystopia::HandleInput(Character2DControllerComponent& controller, f32 deltaTime)
{
    b8 wasJumpPressed = controller.JumpPressed;

    if(BitEngine::InputIsKeyDown(BitEngine::KEY_D))
    {
        controller.MoveInput = 1.0f;
    }
    if(BitEngine::InputIsKeyDown(BitEngine::KEY_A))
    {
        controller.MoveInput = -1.0f;
    }

    controller.JumpHeld = BitEngine::InputIsKeyDown(BitEngine::KEY_SPACE);
    controller.JumpPressed = controller.JumpHeld && !wasJumpPressed;
    controller.JumpReleased = !controller.JumpHeld && wasJumpPressed;

    if(controller.JumpPressed)
        controller.JumpBufferTimer = controller.JumpBufferTime;

    else if(controller.JumpBufferTimer > 0.0f)
        controller.JumpBufferTimer -= deltaTime;
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

    controller.Velocity += movement;
}

void Dystopia::HandleJump(Character2DControllerComponent& controller, f32 deltaTime)
{
    if(controller.IsGrounded)
        controller.CoyoteTimer = controller.CoyoteTime;

    else if(controller.CoyoteTimer > 0.0f)
        controller.CoyoteTimer -= deltaTime;

    b8 canJump = (controller.CoyoteTimer > 0.0f && controller.JumpCount == 0) || (controller.JumpCount < controller.MaxJumps);

    if(controller.JumpBufferTimer > 0.0f && canJump)
    {
        controller.Velocity.y = controller.JumpForce;
        controller.IsJumping = true;
        controller.JumpCount++;

        controller.JumpBufferTimer = 0.0f;
        controller.CoyoteTimer = 0.0f;
    }

    // if the jump button was released mid air
    if (controller.JumpReleased && controller.Velocity.y > 0.0f)
        controller.Velocity.y *= 0.5f;
    
    
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

void Dystopia::HandleCollision(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, CollisionComponent& collision)
{
    collision.CollidingBelow = false;
    collision.CollidingAbove = false;
    collision.CollidingLeft = false;
    collision.CollidingRight = false;
    
    
    std::vector<BitEngine::TileCollisionInfo> tileCollisions;
    m_TileEditor->GetTileCollisions(transform.Position, collision.Width, collision.Height, 
                                    tileCollisions, 0);
    for (auto& tileCol : tileCollisions)
    {
        transform.Position.x += tileCol.Normal.x * tileCol.Depth;
        transform.Position.y += tileCol.Normal.y * tileCol.Depth;
            
            
        float velocityAlongNormal = controller.Velocity.x * tileCol.Normal.x + 
                                   controller.Velocity.y *  tileCol.Normal.y;
        
        if (velocityAlongNormal < 0.0f)  
        {
            controller.Velocity.x -= velocityAlongNormal * tileCol.Normal.x;
            controller.Velocity.y -= velocityAlongNormal * tileCol.Normal.y;
        }
        
        if (tileCol.Normal.y > 0.7f)  
        {
            collision.CollidingBelow = true;
        }
        else if (tileCol.Normal.y < -0.7f)
        {
            collision.CollidingAbove = true;
        }
        
        if (tileCol.Normal.x > 0.7f) 
        {
            collision.CollidingLeft = true;
        }
        else if (tileCol.Normal.x < -0.7f)  
        {
            collision.CollidingRight = true;
        }
        
    }
}

void Dystopia::HandleGroundDetection(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, CollisionComponent& collision)
{
    bool wasGrounded = controller.IsGrounded;
    
    controller.IsGrounded = collision.CollidingBelow;
    
    if (controller.IsGrounded && !wasGrounded)
    {
        controller.JumpCount = 0;
        controller.IsJumping = false;
    }
    
    if (!controller.IsGrounded && wasGrounded)
    {
        BIT_LOG_INFO("Player left ground"); // play animation idk!
    }
}

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
