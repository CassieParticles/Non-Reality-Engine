#include "PortalEndComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Renderer.h>

PortalEndComponent::PortalEndComponent(GameObject* gameObject, Renderer* renderer):RenderComponent{gameObject,renderer}
{
}

void PortalEndComponent::Render()
{
	renderer->addRenderCall<PortalEnd>({ 0 });
}


