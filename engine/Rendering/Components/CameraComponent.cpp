#include "CameraComponent.h"
#include <algorithm>

#include <engine/ObjectStructure/GameObject.h>

CameraComponent::CameraComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }, view{}
{
}

CameraComponent::CameraComponent(CameraComponent& other):RenderComponent{other}
{
	view = other.view;
}

CameraComponent::CameraComponent(CameraComponent&& other):RenderComponent{other}
{
	view = std::move(other.view);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Render()
{
	renderer->addMainCamera(this->gameObject);

}
