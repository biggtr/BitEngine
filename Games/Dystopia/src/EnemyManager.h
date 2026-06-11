#pragma once
#include "Bit/ECS/Entity.h"
#include "Bit/ECS/EntityManager.h"
#include "Bit/ECS/Systems/Animation2DSystem.h"
#include "Bit/ECS/Systems/Physics2DSystem.h"
#include "Bit/Editor/TileEditor.h"
#include "Bit/Renderer/Texture.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include <unordered_map>

enum ENEMY_TYPE
{
    CAT,
    Medosa,
    Chicken,
};
enum ENEMY_STATE
{
    IDLE,
    ATTACK,
    PATROL,
    CHASE,
    HURT,
    DEAD,
};
struct Enemy
{
    ENEMY_TYPE Type;
    ENEMY_STATE State;
    
    f32 Health;
    f32 MaxHealth;

    BitEngine::Entity ID;
    BitEngine::Entity Target;

    f32 ChaseRange; 
    f32 AttackRange;
    f32 LoseRange;
    f32 AttackCoolDown;
    f32 AttackCoolDownMax;
    b8 ShouldAttack = false;

    b8 IsHurt;
    f32 HurtTimer;
    f32 HurtDuration;

    f32 SeparationRadius;
    f32 SeparationStrength;
    BMath::Vec3 SeparationVelocity;
    BMath::Vec3 PatrolStartPosition;
    BMath::Vec3 PatrolEndPosition;
    b8 IsMovingToEnd = true;


    BMath::Vec3 Velocity;

    f32 MaxSpeed; 
    f32 Acceleration; 
    f32 Deceleration; 
    f32 AirControl;

    f32 JumpForce = 100.0f;          
    f32 RisingGravity = 200.0f;     
    f32 FallingGravity = 180.0f;    
    f32 TerminalVelocity = 150.0f;   

    b8 WasGrounded = false;
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

    f32 Restitution = 0.25;
};
class EnemyManager
{
public:
    EnemyManager(BitEngine::EntityManager* entityManager);
    ~EnemyManager();
    
    BitEngine::Entity AddEnemy(ENEMY_TYPE type, const BMath::Vec3& position, const BitEngine::Entity& target);
    
    BMath::Vec2 ResolveTileCollisionSweep(
    f32 deltaTime, BMath::Vec2 currentPos,
    BitEngine::BoxCollider2DComponent& boxCollider,
    BitEngine::TileEditor* tileEditor,
    Enemy& controller);
    void HandleAttack(Enemy& enemy, f32 deltaTime);
    void HandleDamagingPlayer(Enemy& enemy);
    void HandlePatrol(Enemy& enemy);
    void HandleChase(Enemy& enemy, f32 deltaTime);
    void HandleIdle(Enemy& enemy);
    void HandleHurt(Enemy& enemy);
    void HandleDead(Enemy& enemy);
    void Update(f32 deltaTime, BitEngine::TileEditor* tileEditor);
    void KillEnemy(BitEngine::Entity enemy);

    void UpdateAnimation(Enemy& controller, BitEngine::TransformComponent& transform);
    void HandleGravity(Enemy& controller, f32 deltaTime);
    void HandleMovement(Enemy& enemy, f32 deltaTime);
    void HandleJump(Enemy& controller, f32 deltaTime);
    void HandleSeparation();
    BitEngine::Entity GetEnemyByShapeID(b2ShapeId shapeID);
private:

    u32 m_EnemyCount;
    BitEngine::Entity* m_Enemies;
    std::unordered_map<u32, BitEngine::Entity> m_ShapeIDToEnemyID;
    BitEngine::EntityManager* m_EntityManager;
    BitEngine::Animation2DSystem* m_Animation2DSystem;
    BitEngine::Physics2DSystem* m_Physics2DSystem;
};
