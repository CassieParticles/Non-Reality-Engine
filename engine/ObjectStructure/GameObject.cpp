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


