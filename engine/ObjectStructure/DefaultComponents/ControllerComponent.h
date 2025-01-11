#pragma once

#include <engine/ObjectStructure/InputComponent.h>
#include <graphicsEngine/Input.h>

class ControllerComponent : public InputComponent
{
public:
	ControllerComponent(GameObject* gameObject);
	ControllerComponent(ControllerComponent& other);
	ControllerComponent(ControllerComponent&& other);
	~ControllerComponent();

	void setMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	float getMoveSpeed() { return moveSpeed; }

	void Input(InputHandler* input) override;
protected:
	float moveSpeed = 1;
};