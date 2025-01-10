#pragma once

#include <engine/ObjectStructure/RenderComponent.h>
#include <engine/Rendering/ObliqueView.h>

class Scene;

class PortalComponent : public RenderComponent
{
	friend class PortalSurfaceCollider;
public:
	PortalComponent(GameObject* gameObject,Renderer* renderer);
	PortalComponent(PortalComponent& other);
	PortalComponent(PortalComponent&& other);
	~PortalComponent();

	PortalComponent* getOtherPortal() { return linkedPortal;}

	void linkPortal(PortalComponent* other);
	void setPlayer(GameObject** player);
	void SetScene(Scene* scene) { this->scene = scene; }
	
	void Render(bool RenderPortals) override;
protected:
	DirectX::XMFLOAT4X4 calcMatrix();

	PortalComponent* linkedPortal;

	ObliqueView obliqueView;


	GameObject** player{};
	Scene* scene;
};