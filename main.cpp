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
	Entity entity1 = registry->CreateEntity("Entity 1");
	Entity entity2 = registry->CreateEntity("Entity 2");

	entity0.AddComponent<TestCompA>();
	entity0.HasComponent<TestCompA>();
	entity0.GetComponent<TestCompA>();
	entity0.RemoveComponent<TestCompA>();


	Registry::DestroyRegistry();
}