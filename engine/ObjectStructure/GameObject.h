#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <memory>

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

	ID3D11Device* getDevice() { return device.Get(); }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext.Get(); }

	//Definition needs to exist within header file
	template<typename T>
	T* addComponent()
	{
		//Get the type of component T is
		if (std::is_base_of<DataComponent,T>())
		{
			//Check if the component already exists
			for (int i = 0; i < dataComponents.size(); ++i)
			{
				DataComponent* component = dataComponents.at(i).get();
				T* compTest = dynamic_cast<T*>(component);
				if (compTest)
				{
					return compTest;
				}
			}
			//dataComponents.emplace_back(std::make_unique<T>(this));
		
		}
		if (std::is_base_of<InputComponent, T>())
		{
			//Check if the component already exists
			for (int i = 0; i < inputComponents.size(); ++i)
			{
				InputComponent* component = inputComponents.at(i).get();
				T* compTest = dynamic_cast<T*>(component);
				if (compTest)
				{
					return compTest;
				}
			}
			//inputComponents.emplace_back(std::make_unique<T>(this));
		}
		if (std::is_base_of<UpdateComponent, T>())
		{
			//Check if the component already exists
			for (int i = 0; i < updateComponents.size(); ++i)
			{
				UpdateComponent* component = updateComponents.at(i).get();
				T* compTest = dynamic_cast<T*>(component);
				if (compTest)
				{
					return compTest;
				}
			}
			//updateComponents.emplace_back(std::make_unique<T>(this));
		}
		if (std::is_base_of<RenderComponent, T>())
		{
			//Check if the component already exists
			for (int i = 0; i < renderComponents.size(); ++i)
			{
				RenderComponent* component = renderComponents.at(i).get();
				T* compTest = dynamic_cast<T*>(component);
				if (compTest)
				{
					return compTest;
				}
			}
			//Component does not yet exist, so create it
			//renderComponents.emplace_back(std::make_unique<T>(this));
		}

		return nullptr;
	}

	template<typename T>
	T* getComponent()
	{

	}
protected:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	TransformComponent transformComponent;

	std::vector<std::unique_ptr<DataComponent>> dataComponents;
	std::vector<std::unique_ptr<InputComponent>> inputComponents;
	std::vector<std::unique_ptr<UpdateComponent>> updateComponents;
	std::vector<std::unique_ptr<RenderComponent>> renderComponents;
};