#pragma once

#include <engine/ObjectStructure/RenderComponent.h>
#include <engine/Rendering/Components/PlayerCameraComponent.h>
#include <engine/Rendering/ObliqueView.h>

class PortalCameraComponent : public RenderComponent
{
public:
	PortalCameraComponent(GameObject* gameObject, Renderer* renderer);
	PortalCameraComponent(PortalCameraComponent& other) = delete;
	PortalCameraComponent(PortalCameraComponent&& other) = default;
	~PortalCameraComponent();

	void setPlayer(GameObject* player);

	void Render(bool RenderPortals) override;
protected:
	ObliqueView obliqueView;

	GameObject* player;
};