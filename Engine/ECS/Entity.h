#pragma once
#include <string>

#include "ECSDefinitions.h"

class Entity final
{
	friend class Registry;
public:
	Entity(EntityId entId) :entityId{ entId } {}

	Entity(Entity& other);
	Entity(Entity&& other);

	Entity operator=(Entity& RHS);

private:
	Entity(const std::string& name, EntityId entityId);

	EntityId entityId;
};