#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

#include <engine/Rendering/Components/PlayerCameraComponent.h>
#include <engine/Rendering/ObliqueView.h>
#include <engine/SceneManagement/Scene.h>

class PortalCameraComponent : public RenderComponent
{
public:
	PortalCameraComponent(GameObject* gameObject, Renderer* renderer);
	PortalCameraComponent(PortalCameraComponent& other) = delete;
	PortalCameraComponent(PortalCameraComponent&& other) = default;
	~PortalCameraComponent();

	void setPlayer(GameObject* player);
	void SetScene(Scene* scene) { this->scene = scene; }

	void Render(bool RenderPortals) override;

	DirectX::XMFLOAT4X4 calcMatrix();
protected:
	ObliqueView obliqueView;

	GameObject* player;
	Scene* scene;
};