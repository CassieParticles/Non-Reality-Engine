#pragma once

#include <wrl.h>
#include <d3d11.h>

//Specific component within the transform component
#include "DefaultComponents/TransformComponent.h"

#include "DataComponent.h"
#include "InputComponent.h"
#include "UpdateComponent.h"
#include "RenderComponent.h"


class GameObject
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	GameObject(GameObject& other);
	GameObject(GameObject&& other);
	~GameObject();
protected:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	TransformComponent transformComponent;
};