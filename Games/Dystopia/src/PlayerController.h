#pragma once
#include "Bit/ECS/Compontents.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Math/Vector.h"
#include "Bit/Tiles/TileMap.h"

struct Character2DControllerComponent
{
    BMath::Vec3 Velocity;

    f32 MaxSpeed = 60.0f;           
    f32 Acceleration = 8.0f;      
    f32 Deceleration = 4.0f;      
    f32 AirControl = 0.2f;          
    
    f32 JumpForce = 100.0f;          
    f32 RisingGravity = 200.0f;     
    f32 FallingGravity = 180.0f;    
    f32 TerminalVelocity = 150.0f;   

    b8 IsGrounded = false;
    b8 IsJumping = false;
    i32 JumpCount = 0;
    i32 MaxJumps = 3;                

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
class PlayerController 
{

public:

    void HandleInput(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleMovement(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleJump(Character2DControllerComponent& controller, f32 deltaTime);

    void HandleGravity(Character2DControllerComponent& controller, f32 deltaTime);
    void HandleGroundDetection(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller);
    void ApplyPhysics(BitEngine::TransformComponent& transform, Character2DControllerComponent& controller, f32 deltaTime);
    BMath::Vec2 ResolveTileCollisionSweep(f32 deltaTime, BMath::Vec2 currentPos, BitEngine::BoxCollider2DComponent& boxCollider, BitEngine::TileEditor* tileEditor, Character2DControllerComponent& controller);
                                                  

};
