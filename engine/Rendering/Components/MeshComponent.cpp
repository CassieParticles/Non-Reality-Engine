#include "MeshComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <iostream>

#include <graphicsEngine/Pipeline/Texture2D.h>
#include "Portal/PortalComponent.h"

MeshComponent::MeshComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }, mesh{ nullptr }
{

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

void MeshComponent::setTexture(const std::string& name, TextureLoader* textureLoader)
{
	texture = textureLoader->getTexture(name);
}


void MeshComponent::Render(bool RenderPortals)
{
	//Don't render as a standard mesh if part of a portal (portal rendering handled separately)
	if (gameObject->getComponent<PortalComponent>()) { return; }
	//Get world matrix of game object
	DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
	DirectX::XMFLOAT4X4 worldMatrixSta;
	DirectX::XMStoreFloat4x4(&worldMatrixSta, worldMatrix);

	renderer->addRenderCall<DrawMesh>({ 0,mesh,texture,worldMatrixSta });
}

