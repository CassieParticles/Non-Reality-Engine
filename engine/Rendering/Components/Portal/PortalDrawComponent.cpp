#include "PortalDrawComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/MeshComponent.h>

PortalDrawComponent::PortalDrawComponent(GameObject* gameObject, Renderer* renderer):RenderComponent(gameObject,renderer)
{

}

PortalDrawComponent::~PortalDrawComponent()
{
}

void PortalDrawComponent::Render(bool RenderPortals)
{
	if (RenderPortals)
	{
		Mesh* mesh = gameObject->getComponent<MeshComponent>()->getMesh();

		DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
		DirectX::XMFLOAT4X4 worldMatrixSta;
		DirectX::XMStoreFloat4x4(&worldMatrixSta, worldMatrix);

		renderer->addRenderCall<DrawPortal>({0,mesh,worldMatrixSta});

		renderer->addRenderCall<PortalEnd>({ 0 });
	}
}
