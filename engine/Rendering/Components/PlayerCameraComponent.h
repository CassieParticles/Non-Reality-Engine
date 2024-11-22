#pragma once

#include <graphicsEngine/View.h>
#include <engine/ObjectStructure/RenderComponent.h>

class PlayerCameraComponent : public RenderComponent
{
public:
	PlayerCameraComponent(GameObject* gameObject, Renderer* renderer);
	PlayerCameraComponent(PlayerCameraComponent& other);
	PlayerCameraComponent(PlayerCameraComponent&& other);
	~PlayerCameraComponent();

	void setProjectionMatrixPespective(float FOV, float aspectRatio, float nearZ, float farZ) { view.setProjectionMatrixPespective(FOV, aspectRatio, nearZ, farZ); }
	void setProjectionMatrixOrthographic(float viewWidth, float viewHeight, float nearZ, float farZ) { view.setProjectionMatrixOrthographic(viewWidth, viewHeight, nearZ, farZ); }

	DirectX::XMMATRIX getProjectionMatrix() { return view.getProjectionMatrix(); }

	void Render() override;
protected:
	View view;
};