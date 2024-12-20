#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

class PortalComponent : public RenderComponent
{
public:
	PortalComponent(GameObject* gameObject,Renderer* renderer);
	PortalComponent(PortalComponent& other);
	PortalComponent(PortalComponent&& other);
	~PortalComponent();

	void linkPortal(PortalComponent* other);
	PortalComponent* getOtherPortal() { return linkedPortal;}
	
	void Render(bool RenderPortals) override;
protected:
	PortalComponent* linkedPortal;
};