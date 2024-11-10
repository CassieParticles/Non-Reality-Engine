#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer) :device{ device }, deviceContext{ deviceContext },renderer{renderer}, transformComponent{this}
{
	
}

GameObject::GameObject(GameObject& other):transformComponent{other.transformComponent}
{
	
}

GameObject::GameObject(GameObject&& other):transformComponent{std::move(other.transformComponent)}
{

}

GameObject::~GameObject()
{

}

void GameObject::HandleInput()
{
	for (int i = 0; i < inputComponents.size(); ++i)
	{
		inputComponents.at(i)->Input();
	}
}

void GameObject::Update(Timer* timer)
{
	for (int i = 0; i < updateComponents.size(); ++i)
	{
		updateComponents.at(i)->Update(timer);
	}
}

void GameObject::Render()
{
	for (int i = 0; i < renderComponents.size(); ++i)
	{
		renderComponents.at(i)->Render();
	}
}


