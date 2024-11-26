#include "PlayerCameraComponent.h"
#include <algorithm>

#include <engine/ObjectStructure/GameObject.h>

PlayerCameraComponent::PlayerCameraComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }, view{}
{
}

PlayerCameraComponent::PlayerCameraComponent(PlayerCameraComponent& other):RenderComponent{other}
{
	view = other.view;
}

PlayerCameraComponent::PlayerCameraComponent(PlayerCameraComponent&& other):RenderComponent{other}
{
	view = std::move(other.view);
}

PlayerCameraComponent::~PlayerCameraComponent()
{
}

void PlayerCameraComponent::Render(bool RenderPortals)
{
	renderer->addMainCamera(this->gameObject);

}
