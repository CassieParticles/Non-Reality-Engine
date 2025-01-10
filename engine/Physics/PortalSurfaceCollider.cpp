#include "PortalSurfaceCollider.h"

#include <GraphicsBaseFramework/graphicsEngine/VectorMathOverloads.h>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/Portal/PortalComponent.h>

PortalSurfaceCollider::PortalSurfaceCollider(GameObject* gameObject):PhysicsComponent{gameObject}
{

}

PortalSurfaceCollider::~PortalSurfaceCollider()
{
}

void PortalSurfaceCollider::Update(Timer* timer)
{
	if (!player)
	{
		if (PortalComponent* portalComponent = gameObject->getComponent<PortalComponent>())
		{
			player = portalComponent->player;
		}
		else
		{
			return;
		}
	}
	//Get the plane that maps the portal surface
	DirectX::XMVECTOR plane = { 0,0,-1,0 };
	DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
	worldMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, worldMatrix));

	plane = DirectX::XMVector4Transform(plane, worldMatrix);
	DirectX::XMFLOAT4 planeComp;
	DirectX::XMStoreFloat4(&planeComp, plane);
	DirectX::XMVECTOR normal{ planeComp.x,planeComp.y,planeComp.z };
	float w = planeComp.w;

	//Get ends of the line that marks the player's movement this frame
	DirectX::XMFLOAT3 lastPosComp = player->getComponent<TransformComponent>()->getPositionLastFrame();	
	DirectX::XMFLOAT3 currPosComp = player->getComponent<TransformComponent>()->position;
	DirectX::XMVECTOR lastPos = DirectX::XMLoadFloat3(&lastPosComp);
	DirectX::XMVECTOR currPos = DirectX::XMLoadFloat3(&currPosComp);

	DirectX::XMVECTOR direction = currPos - lastPos;

	//Get dot product of normal with point on the line
	float normalDotL0;
	DirectX::XMVECTOR normalDotL0Vec = DirectX::XMVector3Dot(lastPos, normal);
	DirectX::XMStoreFloat(&normalDotL0, normalDotL0Vec);

	//Get dot product of normal with direction of line
	float normalDotDir;
	DirectX::XMVECTOR normalDotDirVec = DirectX::XMVector3Dot(direction, normal);
	DirectX::XMStoreFloat(&normalDotDir, normalDotDirVec);

	//Calculate theta
	float theta = -normalDotL0 - w;
	theta /= normalDotDir;

	//std::cout << "Theta: " << theta << '\n'; 
	if (theta > 0 && theta < 1)
	{
		//Tnransform point Q into model space to see if it's in the quad's space
		DirectX::XMVECTOR Q = lastPos;
		Q += direction * theta;
		DirectX::XMFLOAT3 QComp;
		DirectX::XMStoreFloat3(&QComp, Q);

		DirectX::XMVECTOR QProj = { QComp.x,QComp.y,QComp.z,1 };

		DirectX::XMMATRIX invsWorldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
		DirectX::XMVECTOR QMdlSpace = DirectX::XMVector3Transform(QProj,invsWorldMatrix);
		DirectX::XMFLOAT4 QMdlSpaceComp;
		DirectX::XMStoreFloat4(&QMdlSpaceComp, QMdlSpace);

		if (QMdlSpaceComp.x > -0.5 && QMdlSpaceComp.x < 0.5)
		{
			if (QMdlSpaceComp.y > -0.5 && QMdlSpaceComp.y < 0.5)
			{
				TransformComponent* playerTrans = player->getComponent<TransformComponent>();
				playerTrans->position = gameObject->getComponent<PortalComponent>()->getOtherPortal()->getGameObject()->getComponent<TransformComponent>()->position;
				playerTrans->setPositionLastFrame(gameObject->getComponent<PortalComponent>()->getOtherPortal()->getGameObject()->getComponent<TransformComponent>()->position);
			}
			
		}

		
	}
	

}
