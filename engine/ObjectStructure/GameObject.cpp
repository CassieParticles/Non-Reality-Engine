#include "GameObject.h"
#include "GameObject.h"
#include <engine/Rendering/Components/MeshComponent.h>

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer, InputHandler* input) :device{ device }, deviceContext{ deviceContext }, renderer{ renderer }, input{ input }, transformComponent { this }
{
	
}

GameObject::GameObject(GameObject&& other):transformComponent{std::move(other.transformComponent)}
{
	device = std::move(other.device);
	deviceContext = std::move(other.deviceContext);

	dataComponents = std::move(other.dataComponents);
	inputComponents = std::move(other.inputComponents);
	updateComponents = std::move(other.updateComponents);
	renderComponents = std::move(other.renderComponents);
}

GameObject::~GameObject()
{

}

GameObject& GameObject::operator=(GameObject&& other)
{
	if (this == &other) {
		return *this;
	}
	

	return other;
}

void GameObject::HandleInput()
{
	for (int i = 0; i < inputComponents.size(); ++i)
	{
		inputComponents.at(i)->Input(input);
	}
}

void GameObject::Update(Timer* timer)
{
	for (int i = 0; i < updateComponents.size(); ++i)
	{
		updateComponents.at(i)->Update(timer);
	}
}

void GameObject::PhysUpdate(Timer* timer)
{
	for (int i = 0; i < physicsComponents.size(); ++i)
	{
		physicsComponents.at(i)->Update(timer);
	}
}

void GameObject::Render(bool RenderPortals)
{
	for (int i = 0; i < renderComponents.size(); ++i)
	{
		renderComponents.at(i)->Render(RenderPortals);
	}
}

void GameObject::moveGOLocation()
{
	transformComponent.setGameObject(this);
	for (int i = 0; i < dataComponents.size(); ++i)
	{
		dataComponents.at(i)->setGameObject(this);
	}
	for (int i = 0; i < inputComponents.size(); ++i)
	{
		inputComponents.at(i)->setGameObject(this);
	}
	for (int i = 0; i < updateComponents.size(); ++i)
	{
		updateComponents.at(i)->setGameObject(this);
	}
	for (int i = 0; i < renderComponents.size(); ++i)
	{
		renderComponents.at(i)->setGameObject(this);
	}
}


