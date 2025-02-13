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

	template<Component C, typename... Args>
	C* AddComponent(Args...);
	template<Component C>
	void RemoveComponent();
	template<Component C>
	bool HasComponent();
	template<Component C>
	C* GetComponent();

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
