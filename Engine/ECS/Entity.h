#pragma once
#include <string>

#include "ECSDefinitions.h"
#include "Registry.h"
#include "Component.h"

class Entity final
{
	friend class Registry;
public:
	Entity(EntityId entId) :entityId{ entId } {}

	Entity(Entity& other);
	Entity(Entity&& other);

	Entity operator=(Entity& RHS);

	//Add component C to entity
	template<Component C, typename... Args>
	C* AddComponent(Args...);
	//Remove component C from entity
	template<Component C>
	void RemoveComponent();
	//Check whether entity has componene C
	template<Component C>
	bool HasComponent();
	//Get component C if attached to gameObject
	template<Component C>
	C* GetComponent();


	//DEstroy entity (destroys all game objects)
	void DestroyEntity();
	//Get Entity ID
	EntityId getEntityId() { return entityId; }
private:
	Entity(const std::string& name, EntityId entityId);

	EntityId entityId;
};

template<Component C, typename ...Args>
inline C* Entity::AddComponent(Args... args)
{
	return Registry::getRegistry()->AddComponent<C>(entityId,args...);
}

template<Component C>
inline void Entity::RemoveComponent()
{
	Registry::getRegistry()->RemoveComponent<C>(entityId);
}

template<Component C>
inline bool Entity::HasComponent()
{
	return Registry::getRegistry()->HasComponent<C>(entityId);
}

template<Component C>
inline C* Entity::GetComponent()
{
	return Registry::getRegistry()->GetComponent<C>(entityId);
}
