#include "Registry.h"

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
