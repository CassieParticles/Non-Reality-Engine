#pragma once
#include "Component.h"

#include <graphicsEngine/Input.h>

class InputComponent :public Component
{
public:
	InputComponent(GameObject* gameObject) :Component{ gameObject } {}
	InputComponent(InputComponent& other) :Component{ other } {}
	InputComponent(InputComponent&& other) :Component{ other } {}
	
	//TODO: Add time manager
	virtual void Input(InputHandler* input) = 0;
};
