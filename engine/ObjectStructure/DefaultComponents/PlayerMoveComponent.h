#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <engine/ObjectStructure/UpdateComponent.h>
#include "ControllerComponent.h"

class PlayerMoveComponent :public UpdateComponent
{
public:
	PlayerMoveComponent(GameObject* gameObject);
	PlayerMoveComponent(PlayerMoveComponent& other);
	PlayerMoveComponent(PlayerMoveComponent&& other);
	~PlayerMoveComponent();

	void Update(Timer* timer) override;
protected:
	DirectX::XMFLOAT3 moveDirection;
	DirectX::XMFLOAT2 rotateDirection;

	friend class ControllerComponent;
};