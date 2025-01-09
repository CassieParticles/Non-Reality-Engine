#pragma once

#include <engine/ObjectStructure/Component.h>

class Timer;

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(GameObject* gameObject) :Component{ gameObject } {}
	PhysicsComponent(PhysicsComponent& other) :Component{ other } {}
	PhysicsComponent(PhysicsComponent&& other) :Component{ other } {}
	~PhysicsComponent() {}

	virtual void Update(Timer* timer) = 0;
};