#include <iostream>
#include <tracy/Tracy.hpp>
#include <cstdint>
#include <type_traits>

#include "Engine/ECS/ECS.h"

int main()
{
	FrameMark;
	Registry* registry = Registry::getRegistry();

	EntityId entity0 = 0;
	EntityId entity1 = 1;
	EntityId entity2 = 2;

	registry->AddComponent<TagComponent>(entity0, "Entity 0");
	registry->AddComponent<TagComponent>(entity1, "Entity 1");
	registry->AddComponent<TagComponent>(entity2, "Entity 2");


	registry->AddComponent<TransformComponent>(entity0);
	registry->AddComponent<TransformComponent>(entity1);
	registry->AddComponent<TransformComponent>(entity2);

	while (true)
	{
		registry->RemoveComponent<TagComponent>(entity0);
		registry->AddComponent<TagComponent>(entity0, "Entity 0");
	}



	Registry::DestroyRegistry();
}