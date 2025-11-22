#pragma once 
#include "Bit/Core/Application.h"
#include "Bit/ECS/Entity.h"
#include <Bit/Core/Game.h>

struct Character2DControllerComponent
{
    BMath::Vec3 Velocity;

    f32 MaxSpeed = 100.0f;
    f32 Acceleration = 300.0f;
    f32 Deceleration = 400.0f;
    f32 AirControl = 0.86f;
    
    f32 JumpForce = 200.0f;
    f32 RisingGravity = 400.0f;
    f32 FallingGravity = 550.0f;
    f32 TerminalVelocity = 100.0f;

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
};
