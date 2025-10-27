// #pragma  once 
// #include "Bit/Scene/Compontents.h"
// #include "Bit/Scene/Entity.h"
// #include "Bit/Scene/EntityManager.h"
// #include "Bit/Systems/System.h"
// namespace BitEngine 
// {
//
// class MovementSystem : public System
// {
// public:
//     MovementSystem()
//     {
//         RequireComponent<Rigid2DBodyComponent>();
//         RequireComponent<TransformComponent>();
//     }
//     SYSTEM_CLASS_TYPE(MOVEMENT);
//
//     void Update(float deltaTime)
//     {
//         for(const Entity& entity : m_Entities)
//         {
//             const Rigid2DBodyComponent& rigidBody = m_EntityManager->GetComponent<Rigid2DBodyComponent>(entity);
//             TransformComponent& transform = m_EntityManager->GetComponent<TransformComponent>(entity);
//         }
//     }
//
// };
// }
