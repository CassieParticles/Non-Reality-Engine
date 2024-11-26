#include "PortalComponent.h"

#include <engine/ObjectStructure/GameObject.h>

#include<engine/Rendering/Components/MeshComponent.h>
#include <engine/Rendering/Components/Portal/PortalEndComponent.h>

PortalComponent::PortalComponent(GameObject* gameObject,Renderer* renderer):RenderComponent{gameObject,renderer}
{
	//Check if mesh component already exists
	if (gameObject->getComponent<MeshComponent>())
	{
		//Destroy old mesh component
	}
}

PortalComponent::~PortalComponent()
{
}

void PortalComponent::Render()
{
	renderer->addRenderCall<DrawPortalSurface>({ 0 });
}
