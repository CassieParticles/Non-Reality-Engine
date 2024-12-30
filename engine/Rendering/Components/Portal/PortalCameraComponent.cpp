#include "PortalCameraComponent.h"
#include "PortalCameraComponent.h"
#include "PortalCameraComponent.h"

#include <graphicsEngine/VectorMathOverloads.h>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/Portal/PortalComponent.h>

//Portal camera is in charge of rendering the internals of the portal it is attached to, this means it moves around the other portal

PortalCameraComponent::PortalCameraComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }
{

}

PortalCameraComponent::~PortalCameraComponent()
{
}

void PortalCameraComponent::setPlayer(GameObject* player)
{
	PlayerCameraComponent* camera = player->getComponent<PlayerCameraComponent>();
	if (!camera) { return; }	//No camera, object passed in wasn't player
	this->player = player;
	
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMStoreFloat4x4(&projectionMatrix, camera->getProjectionMatrix());

	obliqueView.setProjectionMatrix(projectionMatrix);
}

void PortalCameraComponent::Render(bool shouldDrawPortals)
{
	if (!shouldDrawPortals) { return; }

	GameObject* otherPortal = gameObject->getComponent<PortalComponent>()->getOtherPortal()->getGameObject();

	//Move the camera to the updated position
	TransformComponent* thisPortalTransform = gameObject->getComponent<TransformComponent>();	//The portal this camera is "rendering to"
	TransformComponent* playerTransform = player->getComponent<TransformComponent>();
	TransformComponent* otherPortalTransform = otherPortal->getComponent<TransformComponent>();	//The portal this camera will be moving around

	//Get the vector from the player camera to this portal
	DirectX::XMFLOAT3 CA;
	CA.x = playerTransform->position.x - thisPortalTransform->position.x;
	CA.y = playerTransform->position.y - thisPortalTransform->position.y;
	CA.z = playerTransform->position.z - thisPortalTransform->position.z;

	//Multiply x,y, and z components by -1
	CA.x *= -1;
	CA.y *= -1;
	CA.z *= -1;
	DirectX::XMVECTOR cameraDisp = DirectX::XMLoadFloat3(&CA);

	//Get the difference between the angles
	DirectX::XMVECTOR thisPortalAngle = DirectX::XMLoadFloat3(&thisPortalTransform->rotation);
	DirectX::XMVECTOR otherPortalAngle = DirectX::XMLoadFloat3(&otherPortalTransform->rotation);
	DirectX::XMVECTOR deltaAngle = otherPortalAngle - thisPortalAngle;
	DirectX::XMMATRIX deltaAngleMat = DirectX::XMMatrixRotationRollPitchYawFromVector(deltaAngle);

	//Rotate the camera displacement
	cameraDisp = DirectX::XMVector3Transform(cameraDisp,deltaAngleMat);

	//Get the position of the camera
	DirectX::XMVECTOR cameraPos = DirectX::XMLoadFloat3(&otherPortalTransform->position);
	cameraPos += cameraDisp;

	//Get the rotation of the other camera (forward and up vector)
	DirectX::XMFLOAT3 playerRotation = playerTransform->rotation;
	DirectX::XMMATRIX playerRotationMat = DirectX::XMMatrixRotationRollPitchYaw(playerRotation.x, playerRotation.y, playerRotation.z);
	DirectX::XMVECTOR playerLookAt = DirectX::XMVector3Transform({ 0,0,1,0 }, playerRotationMat);
	playerLookAt = DirectX::XMVector3Transform(playerLookAt, deltaAngleMat);
	DirectX::XMVECTOR playerLookUp = DirectX::XMVector3Transform({ 0,1,0,0 }, playerRotationMat);
	playerLookUp = DirectX::XMVector3Transform(playerLookUp, deltaAngleMat);

	//Create view matrix from these
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookToLH(cameraDisp, playerLookAt, playerLookUp);

	//Calculate near clip plane in camera space
	

	//Get portal surface in model space
	DirectX::XMVECTOR planeSurface = { 0,0,-1,1 };

	//Convert into world space (multiply by inverse transpose of world matrix)
	DirectX::XMMATRIX OtherPortalWorldMatrix = otherPortalTransform->calcWorldMatrix();
	DirectX::XMMATRIX invTrsWrldMat = DirectX::XMMatrixTranspose(OtherPortalWorldMatrix);
	invTrsWrldMat = DirectX::XMMatrixInverse(nullptr,OtherPortalWorldMatrix);
	planeSurface = DirectX::XMVector4Transform(planeSurface, invTrsWrldMat);

	//Convert into camera space (multiple by inverse transpose of view matrix)
	DirectX::XMMATRIX invTrsViewMat = DirectX::XMMatrixTranspose(viewMatrix);
	invTrsViewMat = DirectX::XMMatrixInverse(nullptr,invTrsViewMat);
	planeSurface = DirectX::XMVector4Transform(planeSurface, invTrsViewMat);

	DirectX::XMFLOAT4 planeSurfaceComp;
	DirectX::XMStoreFloat4(&planeSurfaceComp, planeSurface);

	//Get the projection matrix that will be used
	DirectX::XMFLOAT4X4 obliqueMatrix = obliqueView.calcProjectionMatrixNearClip(planeSurfaceComp);
	
	
}