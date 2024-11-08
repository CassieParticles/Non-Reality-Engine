#include "TransformComponent.h"
#include "TransformComponent.h"
#include "TransformComponent.h"
#include "TransformComponent.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(TransformComponent& other) :DataComponent{ other }
{
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	layer = other.layer;
}

TransformComponent::TransformComponent(TransformComponent&& other) :DataComponent{ other }
{
	position = std::move(other.position);
	rotation = std::move(other.rotation);
	scale = std::move(other.scale);
	layer = std::move(other.layer);

	gameObject = std::move(other.gameObject);
}

TransformComponent& TransformComponent::operator=(TransformComponent& other)
{
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	layer = other.layer;
	gameObject = other.gameObject;

	return *this;
}

DirectX::XMMATRIX TransformComponent::calcWorldMatrix()
{
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	worldMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z) * worldMatrix;
	worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * worldMatrix;
	worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * worldMatrix;

	return worldMatrix;
}

DirectX::XMMATRIX TransformComponent::calcViewMatrix()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	DirectX::XMVECTOR viewDirection = DirectX::XMVector3TransformCoord(DirectX::XMVECTOR{0, 0, 1, 1}, rotationMatrix);
	DirectX::XMVECTOR upDirection = DirectX::XMVector3TransformCoord(DirectX::XMVECTOR{0, 1, 0, 1}, rotationMatrix);

	return DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&position), viewDirection, upDirection);
}
