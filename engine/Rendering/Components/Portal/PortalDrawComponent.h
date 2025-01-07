#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

class PortalDrawComponent : public RenderComponent
{
public:
	PortalDrawComponent(GameObject* gameObject, Renderer* renderer);
	~PortalDrawComponent();

	void Render(bool RenderPortals) override;
protected:

};