#pragma once

#include<unordered_map>

#include <tracy/Tracy.hpp>

#include "ECSDefinitions.h"
#include "ComponentRegistry.h"

class Entity;

class Registry final
{
public:
	static Registry* getRegistry();
	static void DestroyRegistry();

	//Add component to entity entId, args for constructor passed after entId
	template<Component C, typename... Args>
	C* AddComponent(EntityId entId, Args... args);
	//remove component from entity entId
	template<Component C>
	void RemoveComponent(EntityId entId);
	//Check if entity entId has component
	template<Component C>
	bool HasComponent(EntityId entId);
	//Get attached component to entId
	template<Component C>
	C* GetComponent(EntityId entId);

	//Create new entity
	Entity CreateEntity(const std::string& name = "Entity");
	//Destroy all entity components
	void DestroyEntity(EntityId entId);
	
private:
	Registry() { nextFreeId = 0; }
	static Registry* registry;

	EntityId nextFreeId;

	std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> registries;
	template<Component C>
	ComponentRegistry<C>* addRegistry();
	template<Component C>
	bool hasRegistry();
	template<Component C>
	ComponentRegistry<C>* getRegistry();
};

template<Component C, typename ...Args>
inline C* Registry::AddComponent(EntityId entId, Args ...args)
{
	ZoneScopedN("Add component");
	return addRegistry<C>()->AddComponent(entId,args...);
}

template<Component C>
inline void Registry::RemoveComponent(EntityId entId)
{
	ZoneScopedN("Remove component");
	if (ComponentRegistry<C>* registry = getRegistry<C>())
	{
		registry->RemoveComponent(entId);
	}
}

template<Component C>
inline bool Registry::HasComponent(EntityId entId)
{
	ZoneScopedN("Has component");
	if (ComponentRegistry<C>* registry = getRegistry<C>())
	{
		return registry->HasComponent(entId);
	}
}

template<Component C>
inline C* Registry::GetComponent(EntityId entId)
{
	ZoneScopedN("Get component");
	if (ComponentRegistry<C>* registry = getRegistry<C>())
	{
		return reinterpret_cast<C*>(registry->GetComponent(entId));
	}
	return nullptr;
}

template<Component C>
ComponentRegistry<C>* Registry::addRegistry()
{
	ZoneScopedN("Add registry");
	if (ComponentRegistry<C>* reg = getRegistry<C>()) { return reg; }

	size_t typeHash = typeid(C).hash_code();

	registries.insert({typeHash,std::unique_ptr<IComponentRegistry>(new ComponentRegistry<C>())});
	return reinterpret_cast<ComponentRegistry<C>*>(registries.at(typeHash).get());
}

template<Component C>
inline bool Registry::hasRegistry()
{
	ZoneScopedN("Has registry");

	size_t typeHash = typeid(C).hash_code();
	return registries.contains(typeHash);
}

template<Component C>
inline ComponentRegistry<C>* Registry::getRegistry()
{
	ZoneScopedN("Get registry");
	if (!hasRegistry<C>()) { return nullptr; }

	size_t typeHash = typeid(C).hash_code();

	return reinterpret_cast<ComponentRegistry<C>*>(registries.at(typeHash).get());
}
