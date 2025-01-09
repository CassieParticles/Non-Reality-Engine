#pragma once
#include "Component.h"

class DataComponent : public Component
{
public:
	DataComponent(GameObject* gameObject) :Component{ gameObject } {}
	DataComponent(DataComponent& other) :Component{ other } {}
	DataComponent(DataComponent&& other) :Component{ other } {}
	~DataComponent() {}

	virtual void Action() = 0;
};