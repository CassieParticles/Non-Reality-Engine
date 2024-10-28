#pragma once
#include "Component.h"

class InputComponent :public Component
{
public:
	InputComponent(GameObject* gameObject) :Component{ gameObject } {}
	InputComponent(InputComponent& other) :Component{ other } {}
	InputComponent(InputComponent&& other) :Component{ other } {}
	
	//TODO: Add time manager
	virtual void Input() = 0;
};
