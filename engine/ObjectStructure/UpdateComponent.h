#pragma once
#include "Component.h"

class Timer;

class UpdateComponent : public Component
{
public:
	UpdateComponent(GameObject* gameObject) :Component{ gameObject } {}
	UpdateComponent(UpdateComponent& other) :Component{ other } {}
	UpdateComponent(UpdateComponent&& other) :Component{ other } {}
	~UpdateComponent() {}

	//TODO: Add time manager
	virtual void Update(Timer* timer) = 0;
};