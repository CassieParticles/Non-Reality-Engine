#pragma once

#include <engine/ObjectStructure/PhysicsComponent.h>

class PortalSurfaceCollider : public PhysicsComponent
{
public:
	PortalSurfaceCollider(GameObject* gameObject);
	~PortalSurfaceCollider();

	void Update(Timer* timer) override;
protected:
	GameObject* player{};
};