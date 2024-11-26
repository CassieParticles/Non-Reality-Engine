#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

class PortalInternalsComponent : public RenderComponent
{
public:
	PortalInternalsComponent(GameObject* gameObject, Renderer* renderer);

	void Render(bool RenderPortals) override;
protected:

};