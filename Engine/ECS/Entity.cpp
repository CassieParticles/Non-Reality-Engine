#include "Entity.h"

#include "Registry.h"

Entity::Entity(const std::string& name, EntityId entityId):entityId{entityId}
{
	Registry* registry = Registry::getRegistry();

	registry->AddComponent<TransformComponent>(entityId);
	registry->AddComponent<TagComponent>(entityId, name);
}

Entity::Entity(Entity& other)
{
	entityId = other.entityId;
}

Entity::Entity(Entity&& other)
{
	entityId = other.entityId;
}

Entity Entity::operator=(Entity& RHS)
{
	return RHS;
}

void Entity::DestroyEntity()
{
	Registry::getRegistry()->DestroyEntity(entityId);
}

Entity Entity::CopyEntity()
{
	return Registry::getRegistry()->CopyEntity(entityId);
}
