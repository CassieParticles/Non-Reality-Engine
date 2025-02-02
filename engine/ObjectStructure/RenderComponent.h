#pragma once
#include "Component.h"

#include <engine/Rendering/Renderer.h>

class RenderComponent : public Component
{
public:
	RenderComponent(GameObject* gameObject, Renderer* renderer) :Component{ gameObject }, renderer{ renderer } {}
	RenderComponent(RenderComponent& other) :Component{ other } {}
	RenderComponent(RenderComponent&& other) :Component{ other } {}
	~RenderComponent() {}

	//TODO: Add time manager
	virtual void Render(bool RenderPortals) = 0;

	Renderer* renderer;
};