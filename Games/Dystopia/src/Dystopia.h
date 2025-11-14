#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/ECS/Compontents.h"
#include "Bit/ECS/Entity.h"
#include <Bit/Core/Game.h>

struct Character2DControllerComponent
{
    BMath::Vec3 Velocity;

    f32 MaxSpeed = 180.0f;
    f32 Acceleration = 1500.0f;
    f32 Deceleration = 2000.0f;
    f32 AirControl = 0.85f;
    
    f32 JumpForce = 380.0f;
    f32 RisingGravity = 1000.0f;
    f32 FallingGravity = 1800.0f;
    f32 TerminalVelocity = 500.0f;

    b8 IsGrounded = false;
    b8 IsJumping = false;
    i32 JumpCount = 0;
    i32 MaxJumps = 2;  

    float CoyoteTime = 0.12f; // gives the player some time after leaving the ground to jump (beep beep)
    float CoyoteTimer = 0.0f; // counter till the coyotetime to see how much time left for coyote jumping
    float JumpBufferTime = 0.12f; // allows the player to jump before touching the ground again from falling makes it not laggy and responsive
    float JumpBufferTimer = 0.0f;

    float MoveInput = 0.0f;  // -1, 0, 1
    bool JumpPressed = false;
    bool JumpHeld = false;
    bool JumpReleased = false;
};
struct CollisionComponent
{
    f32 Width = 32.0f;
    f32 Height = 32.0f;
    BMath::Vec3 Offset;

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

    void HandleCollision(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, CollisionComponent& collision);

    void HandleGroundDetection(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, CollisionComponent& collision);

    void UpdateAnimation(Character2DControllerComponent& controller, BitEngine::TransformComponent& transform);
private:
    BitEngine::Entity player;
    BitEngine::Entity m_WorldColliders[100];
};
