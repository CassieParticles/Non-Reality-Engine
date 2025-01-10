#pragma once

#include <engine/ObjectStructure/PhysicsComponent.h>
#include <DirectXMath.h>

class PortalSurfaceCollider : public PhysicsComponent
{
public:
	PortalSurfaceCollider(GameObject* gameObject);
	~PortalSurfaceCollider();

	void Update(Timer* timer) override;
protected:
	GameObject** player{};

	void TeleportPlayer(DirectX::XMFLOAT3 collisionPoint);
};