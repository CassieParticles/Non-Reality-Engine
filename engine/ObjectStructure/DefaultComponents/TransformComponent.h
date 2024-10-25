#pragma once
#include "DataComponent.h"

#include <DirectXMath.h>


class TransformComponent : public DataComponent
{
public:
	TransformComponent(GameObject* gameObject, DirectX::XMFLOAT3 position = { 0,0,0 }, DirectX::XMFLOAT3 rotation = { 0,0,0 }, DirectX::XMFLOAT3 scale = { 0,0,0 }, int layer = 0)
		:DataComponent{ gameObject },position {position}, rotation{ rotation }, scale{ scale }, layer{ layer } {}

	TransformComponent(TransformComponent& other) :DataComponent{other} { position = other.position; rotation = other.rotation; scale = other.scale; layer = other.layer; }
	TransformComponent(TransformComponent&& other):DataComponent{other}
	{
		position = std::move(other.position);
		rotation = std::move(other.rotation);
		scale = std::move(other.scale);
		layer = std::move(other.layer);

		gameObject = std::move(other.gameObject);
	}

	TransformComponent& operator=(TransformComponent& other)
	{
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		layer = other.layer;
		gameObject = other.gameObject;

		return *this;
	}

	//Position in space
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	//Position in layers
	int layer;
};