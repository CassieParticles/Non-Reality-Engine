#include "PortalCameraComponent.h"

#include <graphicsEngine/VectorMathOverloads.h>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Components/Portal/PortalComponent.h>
#include <engine/Rendering/Components/MeshComponent.h>

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

	//Map the portal surface into the stencil buffer(MAPPORTALSURFACE)

	//Get world matrix of game object
	DirectX::XMMATRIX worldMatrix = gameObject->getComponent<TransformComponent>()->calcWorldMatrix();
	DirectX::XMFLOAT4X4 worldMatrixSta;
	DirectX::XMStoreFloat4x4(&worldMatrixSta, worldMatrix);

	Mesh* mesh = gameObject->getComponent<MeshComponent>()->getMesh();

	renderer->addRenderCall<DrawMesh>({ 0,mesh,nullptr,worldMatrixSta });

	//Set the camera matrix to the oblique camera matrix to render the other scene()
	renderer->addRenderCall<DrawPortalSetCamera>({ 0,calcMatrix()});

	//Draw the scene visible through the portal(PORTALDRAWSCENE)==============================
	renderer->addRenderCall<DrawPortalInternals>({ 0 });

	//Draw other layer
	scene->renderLayer(gameObject->getComponent<PortalComponent>()->getOtherPortal()->getGameObject()->getComponent<TransformComponent>()->layer, false);
	

	//Draw the portal with the other scene as a texture(PORTALDRAWPORTAL)=========================

	renderer->addRenderCall<DrawPortal>({ 0,mesh,worldMatrixSta });

	//Reset render state to default (PORTALEND)===================================================
	renderer->addRenderCall<PortalEnd>({ 0 });

}

DirectX::XMFLOAT4X4 PortalCameraComponent::calcMatrix()
{
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
	//CA.x *= -1;
	//CA.y *= -1;
	//CA.z *= -1;
	DirectX::XMVECTOR cameraDisp = DirectX::XMLoadFloat3(&CA);

	//Get the difference between the angles
	DirectX::XMVECTOR thisPortalAngle = DirectX::XMLoadFloat3(&thisPortalTransform->rotation);
	DirectX::XMVECTOR otherPortalAngle = DirectX::XMLoadFloat3(&otherPortalTransform->rotation);
	DirectX::XMVECTOR deltaAngle = otherPortalAngle - thisPortalAngle;
	DirectX::XMVECTOR angleOffset = deltaAngle + DirectX::XMVECTOR{ 0,3.14159f,0,0 };
	DirectX::XMMATRIX deltaAngleMat = DirectX::XMMatrixRotationRollPitchYawFromVector(angleOffset);

	//Rotate the camera displacement
	cameraDisp = DirectX::XMVector3Transform(cameraDisp, deltaAngleMat);

	//Get the position of the camera
	DirectX::XMVECTOR otherCameraPos = DirectX::XMLoadFloat3(&otherPortalTransform->position);
	otherCameraPos += cameraDisp;

	//Get the rotation of the other camera (forward and up vector)
	DirectX::XMVECTOR playerRotation = DirectX::XMLoadFloat3(&playerTransform->rotation);
	playerRotation += angleOffset;
	DirectX::XMMATRIX playerRotationMat = DirectX::XMMatrixRotationRollPitchYawFromVector(playerRotation);
	DirectX::XMVECTOR playerLookAt = DirectX::XMVector3Transform({ 0,0,1,0 }, playerRotationMat);
	DirectX::XMVECTOR playerLookUp = DirectX::XMVector3Transform({ 0,1,0,0 }, playerRotationMat);

	//Create view matrix from these
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookToLH(otherCameraPos, playerLookAt, playerLookUp);

	//Calculate near clip plane (portal suface) in camera spaceFor 


	//Get portal surface in model space
	DirectX::XMFLOAT4 planeSurfaceComp;
	DirectX::XMVECTOR planeSurface = { 0,0,-1,0 };

	DirectX::XMMATRIX otherPortalWorldMatrix = otherPortalTransform->calcWorldMatrix();
	otherPortalWorldMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, otherPortalWorldMatrix));

	//Transform the plane into world space using the inverse transpose of the other portal's world matrix
	planeSurface = DirectX::XMVector4Transform(planeSurface, otherPortalWorldMatrix);


	//Transform the plane into view space using the inverse transpose of the camera's view matrix
	DirectX::XMMATRIX viewMatrixForSurface = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, viewMatrix));
	planeSurface = DirectX::XMVector4Transform(planeSurface, viewMatrixForSurface);

	//Get the projection matrix that will be used
	DirectX::XMStoreFloat4(&planeSurfaceComp, planeSurface);
	DirectX::XMFLOAT4X4 obliqueMatrixComp = obliqueView.calcProjectionMatrixNearClip(planeSurfaceComp);

	//Get the view matrix

	DirectX::XMMATRIX obliqueMatrix = DirectX::XMLoadFloat4x4(&obliqueMatrixComp);
	DirectX::XMMATRIX cameraMatrix = viewMatrix * obliqueMatrix;
	DirectX::XMFLOAT4X4 cameraMatrixComp;
	DirectX::XMStoreFloat4x4(&cameraMatrixComp, cameraMatrix);

	return cameraMatrixComp;
}
