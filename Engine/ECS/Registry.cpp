#include "Registry.h"

#include "Entity.h"

Registry* Registry::registry = nullptr;

Registry* Registry::getRegistry()
{
	if (!registry)
	{
		registry = new Registry();
	}
	return registry;
}

void Registry::DestroyRegistry()
{
	delete registry;
}

Entity Registry::CreateEntity(const std::string& name)
{
	ZoneScopedN("Create entity");
	return Entity(name, nextFreeId++);
}

void Registry::DestroyEntity(EntityId entId)
{
	ZoneScopedN("Destroy entity");
	for (auto& it : registries)
	{
		it.second->RemoveComponent(entId);
	}
}

Entity Registry::CopyEntity(Entity original)
{
	ZoneScopedN("Copy entity");
	Entity newEnt = Entity(nextFreeId++);
	for (auto& it : registries)
	{
		it.second->CopyComponent(original.getEntityId(), newEnt.getEntityId());
	}

	return newEnt;
}
