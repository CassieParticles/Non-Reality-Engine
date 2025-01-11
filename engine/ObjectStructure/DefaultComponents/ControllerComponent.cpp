#include "ControllerComponent.h"

#include <engine/ObjectStructure/GameObject.h>

#include <GLFW/glfw3.h>

#include "PlayerMoveComponent.h"

ControllerComponent::ControllerComponent(GameObject* gameObject):InputComponent{gameObject}
{
	if (!gameObject->getComponent<PlayerMoveComponent>())
	{
		gameObject->addUpdateComponent<PlayerMoveComponent>();
	}
}

ControllerComponent::ControllerComponent(ControllerComponent& other):InputComponent{other}
{

}

ControllerComponent::ControllerComponent(ControllerComponent&& other):InputComponent{other}
{

}

ControllerComponent::~ControllerComponent()
{
}

void ControllerComponent::Input(InputHandler* input)
{
	PlayerMoveComponent* playerMoveComp = gameObject->getComponent<PlayerMoveComponent>();

	if (input->getKeyDown(GLFW_KEY_W))
	{
		playerMoveComp->moveDirection.z += moveSpeed;
	}
	if (input->getKeyDown(GLFW_KEY_S))
	{
		playerMoveComp->moveDirection.z -= moveSpeed;
	}
	if (input->getKeyDown(GLFW_KEY_A))
	{
		playerMoveComp->moveDirection.x -= moveSpeed;
	}
	if (input->getKeyDown(GLFW_KEY_D))
	{
		playerMoveComp->moveDirection.x += moveSpeed;
	}
	if (input->getKeyDown(GLFW_KEY_Q))
	{
		playerMoveComp->moveDirection.y += moveSpeed;
	}
	if (input->getKeyDown(GLFW_KEY_E))
	{
		playerMoveComp->moveDirection.y -= moveSpeed;
	}

	//If mouse is centred, then rotate camera
	if (input->getMouseCentred())
	{
		playerMoveComp->rotateDirection.x = input->getCursorY() / 100.0;
		playerMoveComp->rotateDirection.y = input->getCursorX() / 100.0;
	}

	//Right click
	input->setMouseCentred(input->getMouseButtonDown(GLFW_MOUSE_BUTTON_2));
}
