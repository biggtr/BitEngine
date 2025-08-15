#pragma once
#include "EntityManager.h"



namespace BitEngine 
{

template<typename TComponent, typename ...TArgs>
TComponent& Entity::AddComponent(TArgs&& ...args)
{
    return m_EntityManager->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
TComponent& Entity::GetComponent()
{
    return m_EntityManager->GetComponent<TComponent>(*this);

}
template<typename TComponent> 
b8 Entity::HasComponent()
{
    return m_EntityManager->HasComponent<TComponent>(*this);
}
template<typename TComponent> 
void Entity::RemoveComponent()
{
    m_EntityManager->RemoveComponent<TComponent>(*this);
}
inline void Entity::KillEntity() 
{
    m_EntityManager->KillEntity(*this);
}

}
