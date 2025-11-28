#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/ECS/Entity.h"
#include <Bit/Core/Game.h>

struct Character2DControllerComponent
{
    BMath::Vec3 Velocity;

    f32 MaxSpeed = 100.0f;           
    f32 Acceleration = 1000.0f;      
    f32 Deceleration = 1200.0f;      
    f32 AirControl = 0.6f;          
    
    f32 JumpForce = 170.0f;          
    f32 RisingGravity = 800.0f;     
    f32 FallingGravity = 200.0f;    
    f32 TerminalVelocity = 500.0f;   

    b8 IsGrounded = false;
    b8 IsJumping = false;
    i32 JumpCount = 0;
    i32 MaxJumps = 2;                

    float CoyoteTime = 0.08f;        
    float CoyoteTimer = 0.0f;
    float JumpBufferTime = 0.15f;    
    float JumpBufferTimer = 0.0f;

    float MoveInput = 0.0f;
    bool JumpPressed = false;
    bool JumpHeld = false;
    bool JumpReleased = false;

    b8 CollidingBelow = false;
    b8 CollidingAbove = false;
    b8 CollidingLeft = false;
    b8 CollidingRight = false;
};

class Dystopia : public BitEngine::Game
{
public:
    Dystopia()
    {
    }
    ~Dystopia(){}
    virtual void Initialize() override;
    // virtual void SetupInput() override;
    virtual void Update(float deltaTime) override;
    virtual void Render2D() override; 
    virtual void Render3D() override; 
    virtual void RenderUI() override; 

private:

    void HandleInput(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleMovement(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleJump(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleGravity(Character2DControllerComponent& controller, f32 deltaTime);

    void ApplyPhysics(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, f32 deltaTime);

    // void HandleCollision(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller); 

    void HandleGroundDetection(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller);

    void UpdateAnimation(Character2DControllerComponent& controller, BitEngine::TransformComponent& transform);

    BMath::Vec2 HandleKinematicCollisions(f32 deltaTime, 
                                                  BitEngine::Rigidbody2DComponent& rigidBody, 
                                                  Character2DControllerComponent& controller,
                                                  BitEngine::BoxCollider2DComponent& boxCollider,
                                                  const BMath::Vec2& currentPos,
                                                  const BMath::Vec2& desiredPos);

    void UpdateGroundedState(BitEngine::Rigidbody2DComponent& rigidBody, 
                                    Character2DControllerComponent& controller,
                                    BitEngine::BoxCollider2DComponent& boxCollider,
                                    const BMath::Vec2& currentPos);
private:
    BitEngine::Entity player;
    BitEngine::Entity m_WorldColliders[100];
    u32 TileIndex;
};
