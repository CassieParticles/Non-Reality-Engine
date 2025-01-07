#pragma once

#include <engine/ObjectStructure/RenderComponent.h>

#include <engine/SceneManagement/Scene.h>

class PortalInternalsComponent : public RenderComponent
{
public:
	PortalInternalsComponent(GameObject* gameObject, Renderer* renderer);

	void SetScene(Scene* scene) { this->scene = scene; }

	void Render(bool RenderPortals) override;
protected:
	Scene* scene;

	int otherPortalScene = -1;
};