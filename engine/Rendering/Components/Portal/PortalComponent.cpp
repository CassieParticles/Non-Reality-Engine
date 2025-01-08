#include "PortalComponent.h"

#include <engine/ObjectStructure/GameObject.h>

#include<engine/Rendering/Components/MeshComponent.h>

PortalComponent::PortalComponent(GameObject* gameObject,Renderer* renderer):RenderComponent{gameObject,renderer}
{
	
}

PortalComponent::~PortalComponent()
{
}

void PortalComponent::linkPortal(PortalComponent* other)
{
	linkedPortal = other;
}

void PortalComponent::Render(bool RenderPortals)
{
	if (RenderPortals)
	{
		renderer->addRenderCall<DrawPortalSurface>({ 0 });
	}
}
