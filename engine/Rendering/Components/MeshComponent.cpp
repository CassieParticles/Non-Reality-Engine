#include "MeshComponent.h"
#include "MeshComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <iostream>

MeshComponent::MeshComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }, mesh{ nullptr }
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

MeshComponent::MeshComponent(MeshComponent& other):RenderComponent{gameObject,renderer}
{
	mesh = other.mesh;
}

MeshComponent::MeshComponent(MeshComponent&& other):RenderComponent{gameObject,renderer}
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

	//Get world matrix of game object
	DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
	DirectX::XMFLOAT4X4 worldMatrixSta;
	DirectX::XMStoreFloat4x4(&worldMatrixSta, worldMatrix);

	renderer->addRenderCall<DrawMesh>({ 0,mesh,worldMatrixSta });
}

