#include <iostream>
#include <tracy/Tracy.hpp>
#include <cstdint>
#include <type_traits>

#include "Engine/ECS/ECS.h"
#include "Engine/testComp.h"

int main()
{
	FrameMark;
	Registry* registry = Registry::getRegistry();

	Entity entity0 = registry->CreateEntity("Entity 0");

	EntityId entId = entity0;

	Entity entity0Copy = entity0.CopyEntity();

	Registry::DestroyRegistry();
}