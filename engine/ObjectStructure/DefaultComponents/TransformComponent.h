#pragma once
#include "../DataComponent.h"

#include <DirectXMath.h>


class TransformComponent final: public DataComponent
{
public:
	TransformComponent(GameObject* gameObject, DirectX::XMFLOAT3 position = { 0,0,0 }, DirectX::XMFLOAT3 rotation = { 0,0,0 }, DirectX::XMFLOAT3 scale = { 1,1,1 }, int layer = 0)
		:DataComponent{ gameObject },position {position}, rotation{ rotation }, scale{ scale }, layer{ layer } {}

	TransformComponent(TransformComponent& other);
	TransformComponent(TransformComponent&& other);

	TransformComponent& operator=(TransformComponent& other);

	void Action() override {}

	//For use in meshes and rendered objects
	DirectX::XMMATRIX calcWorldMatrix();
	//For use in views
	DirectX::XMMATRIX calcViewMatrix();

	//Position in space
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	//What layer object is on do not modify, needs to be public to allow other components to change
	int layer;

protected:

};