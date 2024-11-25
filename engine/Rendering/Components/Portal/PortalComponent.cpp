#include "PortalComponent.h"

#include <engine/ObjectStructure/GameObject.h>

#include<engine/Rendering/Components/MeshComponent.h>

PortalComponent::PortalComponent(GameObject* gameObject,Renderer* renderer):RenderComponent{gameObject,renderer}
{
	//Check if mesh component already exists
	if (gameObject->getComponent<MeshComponent>())
	{
		//Destroy old mesh component
	}

	//Add relevant components
	MeshComponent* portalMesh = gameObject->addRenderComponent<MeshComponent>();
}