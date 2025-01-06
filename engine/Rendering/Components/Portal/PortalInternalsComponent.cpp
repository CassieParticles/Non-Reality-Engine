#include "PortalInternalsComponent.h"

#include <engine/ObjectStructure/GameObject.h>

#include "PortalComponent.h"

PortalInternalsComponent::PortalInternalsComponent(GameObject* gameObject, Renderer* renderer):RenderComponent{gameObject,renderer}
{

}

void PortalInternalsComponent::Render(bool RenderPortals)
{
	if (!RenderPortals) { return; }
	//Enable drawing the internals
	renderer->addRenderCall<DrawPortalInternals>({ 0 });

	//Draw other layer
	//TODO: Draw other layer
	scene->renderLayer(0, false);
}
