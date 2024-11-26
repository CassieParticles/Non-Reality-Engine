#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

class PortalEndComponent : public RenderComponent
{
public:
	PortalEndComponent(GameObject* gameObject, Renderer* renderer);

	void Render() override;
protected:
	
};