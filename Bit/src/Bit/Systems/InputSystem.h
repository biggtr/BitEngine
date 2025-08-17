#pragma  once 
#include "Bit/Core/Input.h"
#include "Bit/Core/Logger.h"
#include "Bit/Scene/Compontents.h"
#include "Bit/Scene/Entity.h"
#include "Bit/Scene/EntityManager.h"
#include "Bit/Systems/System.h"
#include <functional>
namespace BitEngine 
{

enum ACTION_TYPE
{
    ACTION_MOVE_FORWARD,
    ACTION_MOVE_BACKWARD,
    ACTION_MOVE_LEFT,
    ACTION_MOVE_RIGHT,
    ACTION_JUMP,
    ACTION_ATTACK
};
struct Action
{
    std::vector<KEYS> keys;
    f32 value = 0.0f;
};
struct CInput
{
    std::unordered_map<ACTION_TYPE, Action> Actions;
    std::unordered_map<ACTION_TYPE, std::function<void(const Entity&)>> ActionBindings;
    std::unordered_map<ACTION_TYPE, std::function<void(const Entity&, f32)>> AxisBindings;
};
class InputSystem : public System
{
private:
    Input* m_Input;
public:
    InputSystem(Input* input)
    {
        m_Input = input;
        RequireComponent<CInput>();
    }
    SYSTEM_CLASS_TYPE(INPUT);

    // doesnt support mouse for now 
    // TODO:: Add Mouse Support
    // TODO : Change this Entity Part to Take A Pointer rather that the entity
    void CreateAction(ACTION_TYPE actionType, const Entity& entity, KEYS key)
    {
        if(!m_EntityManager->HasComponent<CInput>(entity)) return;
        
        CInput& inputComponent = m_EntityManager->GetComponent<CInput>(entity);
        if(inputComponent.Actions[actionType].keys[key] == key)
        {
            BIT_LOG_DEBUG("Action : %d with key : %d is Already Created..!", actionType, key);
            return;
        }
        inputComponent.Actions[actionType].keys.push_back(key);
    }
    void CreateAxis(ACTION_TYPE actionType, const Entity& entity, KEYS key, f32 value)
    {
        if(!m_EntityManager->HasComponent<CInput>(entity)) return;
        
        CInput& inputComponent = m_EntityManager->GetComponent<CInput>(entity);
        if(inputComponent.Actions[actionType].keys[key] == key)
        {
            BIT_LOG_DEBUG("Action : %d with key : %d is Already Created..!", actionType, key);
            return;
        }
        inputComponent.Actions[actionType].keys.push_back(key);
        inputComponent.Actions[actionType].value = value;
    }

    template<typename T>
    void BindAction(ACTION_TYPE actionType, const Entity& entity, T* instance, void (T::*memberFunc)(const Entity&))
    {
        if(!m_EntityManager->HasComponent<CInput>(entity)) return;
        
        CInput& inputComponent = m_EntityManager->GetComponent<CInput>(entity);
        if(!inputComponent.Actions.contains(actionType))
        {
            BIT_LOG_DEBUG("Action Was Not Found To Bind the function..!");
            return;
        }
        inputComponent.ActionBindings[actionType] = [instance, memberFunc](const Entity& entity){
            (instance->*memberFunc)(entity);
        };
    }
    template<typename T>
    void BindAxis(ACTION_TYPE actionType, const Entity& entity, T* instance, void(T::*memberFunc)(const Entity&, f32))
    {
        if(!m_EntityManager->HasComponent<CInput>(entity)) return;

        CInput& inputComponent = m_EntityManager->GetComponent<CInput>(entity);
        if(!inputComponent.Actions.contains(actionType))
        {
            BIT_LOG_DEBUG("Action Was Not Found To Bind the function..!");
            return;
        }
        inputComponent.ActionBindings[actionType] = [instance, memberFunc](const Entity& entity){
            (instance->*memberFunc)(entity);
        };
    }

    void Update(float deltaTime)
    {
        for(const Entity& entity : m_Entities)
        {

        }
    }

};
}
