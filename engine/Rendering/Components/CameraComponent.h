#pragma once

#include <graphicsEngine/View.h>
#include <engine/ObjectStructure/RenderComponent.h>

class CameraComponent : public RenderComponent
{
public:
	CameraComponent(GameObject* gameObject, Renderer* renderer);
	CameraComponent(CameraComponent& other);
	CameraComponent(CameraComponent&& other);
	~CameraComponent();

	void setProjectionMatrixPespective(float FOV, float aspectRatio, float nearZ, float farZ) { view.setProjectionMatrixPespective(FOV, aspectRatio, nearZ, farZ); }
	void setProjectionMatrixOrthographic(float viewWidth, float viewHeight, float nearZ, float farZ) { view.setProjectionMatrixOrthographic(viewWidth, viewHeight, nearZ, farZ); }

	DirectX::XMMATRIX getProjectionMatrix() { return view.getProjectionMatrix(); }

	void Render() override;
protected:
	View view;
};