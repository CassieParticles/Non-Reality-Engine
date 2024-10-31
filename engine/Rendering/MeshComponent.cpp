#include "MeshComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <iostream>

MeshComponent::MeshComponent(GameObject* gameObject) :RenderComponent{ gameObject }, mesh{ nullptr }
{
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(DirectX::XMMATRIX);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	gameObject->getDevice()->CreateBuffer(&desc, 0, &worldMatrixBuffer);
}

MeshComponent::MeshComponent(MeshComponent& other):RenderComponent{gameObject}
{
	mesh = other.mesh;
}

MeshComponent::MeshComponent(MeshComponent&& other):RenderComponent{gameObject}
{
	mesh = std::move(other.mesh);
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::setMesh(const std::string& name, MeshLoader* meshLoader)
{
	mesh = meshLoader->getMesh(name);
}

void MeshComponent::Render()
{
	//Get device context
	ID3D11DeviceContext* deviceContext = gameObject->getDeviceContext();

	//Update mesh world matrix
	TransformComponent* trans = gameObject->getComponent<TransformComponent>();
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
	worldMatrix = DirectX::XMMatrixTranslation(trans->position.x, trans->position.y, trans->position.z) * worldMatrix;
	worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(trans->rotation.x,trans->rotation.y,trans->rotation.z) * worldMatrix;
	worldMatrix = DirectX::XMMatrixScaling(trans->scale.x, trans->scale.y, trans->scale.z) * worldMatrix;
	
	D3D11_MAPPED_SUBRESOURCE map{};
	HRESULT err = deviceContext->Map(worldMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	if (FAILED(err))
	{
		std::cerr << "Uh oh\n";
	}
	memcpy(map.pData, &worldMatrix, sizeof(worldMatrix));
	deviceContext->Unmap(worldMatrixBuffer.Get(), 0);

	mesh->useMesh(deviceContext);

	deviceContext->VSSetConstantBuffers(1, 1, worldMatrixBuffer.GetAddressOf());

	deviceContext->DrawIndexed(mesh->getVertexCount(), 0, 0);
}

