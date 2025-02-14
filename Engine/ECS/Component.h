#pragma once

#include <glm/vec3.hpp>
#include <string>
#include "ECSDefinitions.h"

#include <iostream>

class IComponentRegistry;

class BaseComponent
{
	friend class IComponentRegistry;
public:


	EntityId getEntityId() { return entityId; }
protected:
	EntityId entityId;

	virtual void OnComponentAdded() {}
	virtual void OnComponentCopied() {}
	virtual void OnComponentRemoved() {}
};

class TransformComponent final: public BaseComponent
{
public:
	TransformComponent(glm::vec3 position = {}, glm::vec3 rotation = {}, glm::vec3 scale = { 1,1,1 }):position{position}, rotation{rotation},scale{scale} {}
	TransformComponent(TransformComponent& other) :position{ other.position }, rotation{ other.rotation }, scale{ other.scale } {}
	TransformComponent operator=(TransformComponent& rhs) { return rhs; }

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class TagComponent final: public BaseComponent
{
public:
	TagComponent(const std::string& name) :name{ name } {}
	TagComponent(TagComponent& other) { name = other.name; }
	TagComponent operator=(TagComponent& rhs) { return rhs; }
	std::string name;

	operator const std::string&() const { return name; }
};