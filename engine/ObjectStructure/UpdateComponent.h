#pragma once
#include "Component.h"

class UpdateComponent : public Component
{
public:
	UpdateComponent(GameObject* gameObject) :Component{ gameObject } {}
	UpdateComponent(UpdateComponent& other) :Component{ other } {}
	UpdateComponent(UpdateComponent&& other) :Component{ other } {}

	//TODO: Add time manager
	void Update();
};