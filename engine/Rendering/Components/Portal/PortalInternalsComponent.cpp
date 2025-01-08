#include "PortalInternalsComponent.h"

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/MeshComponent.h>

#include "PortalComponent.h"

PortalInternalsComponent::PortalInternalsComponent(GameObject* gameObject, Renderer* renderer):RenderComponent{gameObject,renderer}
{

}

void PortalInternalsComponent::Render(bool RenderPortals)
{
	if (!RenderPortals) { return; }
	//Enable drawing the internals
	renderer->addRenderCall<DrawPortalInternals>({ 0 });

	//Draw other layer
	//TODO: Draw other layer
	if (otherPortalScene == -1)
	{
		if (PortalComponent* otherPortalComp = gameObject->getComponent<PortalComponent>()->getOtherPortal())
		{
			otherPortalScene = otherPortalComp->getGameObject()->getComponent<TransformComponent>()->layer;
		}
	}
	else
	{
		scene->renderLayer(otherPortalScene, false);
	}

	Mesh* mesh = gameObject->getComponent<MeshComponent>()->getMesh();

	DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
	DirectX::XMFLOAT4X4 worldMatrixSta;
	DirectX::XMStoreFloat4x4(&worldMatrixSta, worldMatrix);

	renderer->addRenderCall<DrawPortal>({ 0,mesh,worldMatrixSta });

	renderer->addRenderCall<PortalEnd>({ 0 });

}
