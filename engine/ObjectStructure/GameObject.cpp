#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :device{ device }, deviceContext{ deviceContext }, transformComponent{ this }
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


