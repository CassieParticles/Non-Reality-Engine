#pragma once

#include <utility>

class GameObject;

class Component
{
public:
	Component(GameObject* gameObject) :gameObject{ gameObject } {}
	Component(Component& other) { gameObject = other.gameObject; }
	Component(Component&& other) { gameObject = std::move(other.gameObject); }

	//FOR INTERNAL USE ONLY, DO NOT CALL
	void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
protected:
	GameObject* gameObject;
};