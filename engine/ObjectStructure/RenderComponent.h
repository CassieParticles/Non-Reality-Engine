#pragma once
#include "Component.h"

class RenderComponent : public Component
{
public:
	RenderComponent(GameObject* gameObject) :Component{ gameObject } {}
	RenderComponent(RenderComponent& other) :Component{ other } {}
	RenderComponent(RenderComponent&& other) :Component{ other } {}

	//TODO: Add time manager
	virtual void Render() = 0;
};