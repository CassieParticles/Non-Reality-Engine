#include "PortalSurfaceCollider.h"

#include <GraphicsBaseFramework/graphicsEngine/VectorMathOverloads.h>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/Portal/PortalComponent.h>
#include <engine/SceneManagement/Scene.h>

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
			if (!portalComponent->getOtherPortal())
			{
				return;
			}
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
	DirectX::XMFLOAT3 lastPosComp = (*player)->getComponent<TransformComponent>()->getPositionLastFrame();
	DirectX::XMFLOAT3 currPosComp = (*player)->getComponent<TransformComponent>()->position;
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
				TeleportPlayer(QComp);
			}
		}
	}
}

void PortalSurfaceCollider::TeleportPlayer(DirectX::XMFLOAT3 collisionPoint)
{
	TransformComponent* thisPortalTransform = gameObject->getComponent<TransformComponent>();
	TransformComponent* otherPortalTransform = gameObject->getComponent<PortalComponent>()->getOtherPortal()->gameObject->getComponent<TransformComponent>();
	TransformComponent* playerTransform = (*player)->getComponent<TransformComponent>();

	//Rotate the player

	//Get the angle between the portals
	DirectX::XMVECTOR thisPortalAngle = DirectX::XMLoadFloat3(&thisPortalTransform->rotation);
	DirectX::XMVECTOR otherPortalAngle = DirectX::XMLoadFloat3(&otherPortalTransform->rotation);
	DirectX::XMVECTOR deltaAngle = otherPortalAngle - thisPortalAngle;
	DirectX::XMVECTOR angleOffset = deltaAngle + DirectX::XMVECTOR{ 0,3.14159f,0,0 };

	//Rotate the player by this angle
	DirectX::XMFLOAT3 angleOffsetComp;
	DirectX::XMStoreFloat3(&angleOffsetComp, angleOffset);
	playerTransform->rotation.x += angleOffsetComp.x;
	playerTransform->rotation.y += angleOffsetComp.y;
	playerTransform->rotation.z += angleOffsetComp.z;

	//Move the player to the other portal

	//Get the vector from the portal centre to the point the player passed through
	DirectX::XMFLOAT3 CP;
	CP.x = collisionPoint.x - thisPortalTransform->position.x;
	CP.y = collisionPoint.y - thisPortalTransform->position.y;
	CP.z = collisionPoint.z - thisPortalTransform->position.z;

	//Calculate how much movement the player still has left to do 
	DirectX::XMFLOAT3 leftMovement;
	leftMovement.x = playerTransform->position.x - collisionPoint.x;
	leftMovement.y = playerTransform->position.y - collisionPoint.y;
	leftMovement.z = playerTransform->position.z - collisionPoint.z;

	CP.x += leftMovement.x;
	CP.y += leftMovement.y;
	CP.z += leftMovement.z;

	//Rotate this by the difference between the portals
	DirectX::XMVECTOR CPDisp = DirectX::XMLoadFloat3(&CP);
	DirectX::XMMATRIX angleOffsetMat = DirectX::XMMatrixRotationRollPitchYawFromVector(angleOffset);
	CPDisp = DirectX::XMVector3Transform(CPDisp, angleOffsetMat);
	DirectX::XMStoreFloat3(&CP, CPDisp);

	//Calcualte the new position on the portal
	DirectX::XMFLOAT3 newPosition;
	newPosition.x = otherPortalTransform->position.x + CP.x;
	newPosition.y = otherPortalTransform->position.y + CP.y;
	newPosition.z = otherPortalTransform->position.z + CP.z;



	//Set the new position
	playerTransform->position = newPosition;
	playerTransform->setPositionLastFrame(newPosition);

	//Transport player to the other layer
	gameObject->getComponent<PortalComponent>()->scene->moveGameObject(thisPortalTransform->layer, otherPortalTransform->layer, *player);
}
