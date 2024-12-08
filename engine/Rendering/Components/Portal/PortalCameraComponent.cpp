#include "PortalCameraComponent.h"

#include <engine/ObjectStructure/GameObject.h>

PortalCameraComponent::PortalCameraComponent(GameObject* gameObject, Renderer* renderer) :RenderComponent{ gameObject,renderer }
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

	//Get the view matrix that will be used
		//Calculate the vector from the player to this portal
		//Invert the x and z components
		//Rotate it based on the difference between both portals' rotations
		//Add it to the other portal's position
		//Rotate camera view based on difference in rotation
		//Calculate view matrix from this

	//Calculate the plane of the other portal in camera space
	DirectX::XMFLOAT4 nearClipPlane;

	//Get the projection matrix that will be used
	DirectX::XMFLOAT4X4 obliqueMatrix = obliqueView.calcProjectionMatrixNearClip(nearClipPlane);
	
	
}