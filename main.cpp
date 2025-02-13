#include <iostream>
#include <tracy/Tracy.hpp>
#include <cstdint>
#include <type_traits>

#include "Engine/ECS/ECS.h"

int main()
{
	FrameMark;
	Registry* registry = Registry::getRegistry();

	Entity entity0 = registry->CreateEntity("Entity 0");
	Entity entity1 = registry->CreateEntity("Entity 1");
	Entity entity2 = registry->CreateEntity("Entity 2");

	while (true)
	{
		
	}



	Registry::DestroyRegistry();
}