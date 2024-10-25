#include "MeshComponent.h"

#include <engine/ObjectStructure/GameObject.h>

MeshComponent::MeshComponent(GameObject* gameObject) :RenderComponent{ gameObject }, mesh{ nullptr }
{
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
	ID3D11DeviceContext* deviceContext = gameObject->getDeviceContext();
	mesh->useMesh(deviceContext);

	//TODO: Make use of meshTransform component

	deviceContext->DrawIndexed(mesh->getVertexCount(), 0, 0);
}

