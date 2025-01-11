#include "PlayerMoveComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <graphicsEngine/VectorMathOverloads.h>

PlayerMoveComponent::PlayerMoveComponent(GameObject* gameObject) :UpdateComponent{ gameObject }
{
	moveDirection = DirectX::XMFLOAT3(0, 0, 0);
	rotateDirection = DirectX::XMFLOAT2(0, 0);
}

PlayerMoveComponent::PlayerMoveComponent(PlayerMoveComponent& other) :UpdateComponent{ other }
{
	other.moveDirection = moveDirection;
}

PlayerMoveComponent::PlayerMoveComponent(PlayerMoveComponent&& other):UpdateComponent{other}
{
	other.moveDirection = std::move(moveDirection);
}

PlayerMoveComponent::~PlayerMoveComponent()
{

}

void PlayerMoveComponent::Update(Timer* timer)
{
	TransformComponent* transform = gameObject->getComponent<TransformComponent>();

	//Translate camera
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transform->position);
	DirectX::XMVECTOR travelDist = DirectX::XMLoadFloat3(&moveDirection);

	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0, 1, 0, 0), transform->rotation.y);
	
	pos += DirectX::XMVector3Transform(travelDist, rotationMat) * timer->getDeltaTime();

	DirectX::XMStoreFloat3(&transform->position, pos);
	//Rotate camera
	DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transform->rotation);
	DirectX::XMVECTOR rotation = DirectX::XMLoadFloat2(&rotateDirection);

	rot += rotation;

	DirectX::XMStoreFloat3(&transform->rotation,rot);

	moveDirection = DirectX::XMFLOAT3(0,0,0);
	rotateDirection = DirectX::XMFLOAT2(0, 0);
}
