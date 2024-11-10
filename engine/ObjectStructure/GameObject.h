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

#include <engine/Rendering/Renderer.h>
#include <engine/SceneManagement/Timer.h>


class GameObject
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer);
	GameObject(GameObject& other);
	GameObject(GameObject&& other);
	~GameObject();

	ID3D11Device* getDevice() { return device.Get(); }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext.Get(); }

	void HandleInput();
	void Update(Timer* timer);
	void Render();

	//Definition needs to exist within header file
	template<typename T>
	T* addDataComponent()
	{
		//Ensure 2 transform components do not exist
		if (std::is_base_of<TransformComponent, T>)
		{
			return &transformComponent;
		}
		//Check if component exists
		for (int i = 0; i < dataComponents.size(); ++i)
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
			dataComponents.emplace_back(std::make_unique<T>(this));
			//Component last element in array, add
			return dynamic_cast<T*>(dataComponents.at(dataComponents.size() - 1).get());
		}
	}

	template<typename T>
	T* addInputComponent()
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
		inputComponents.emplace_back(std::make_unique<T>(this));
		return dynamic_cast<T*>(inputComponents.at(inputComponents.size() - 1).get());
	}

	template<typename T>
	T* addUpdateComponent()
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
		updateComponents.emplace_back(std::make_unique<T>(this));
		return dynamic_cast<T*>(updateComponents.at(updateComponents.size() - 1).get());
	}

	template<typename T>
	T* addRenderComponent()
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
		renderComponents.emplace_back(std::make_unique<T>(this,renderer));
		return dynamic_cast<T*>(renderComponents.at(renderComponents.size() - 1).get());
	}

	template<typename T>
	T* getComponent()
	{
		//Check if the component is the transform component
		if (std::is_base_of<TransformComponent, T>())
		{
			//Technically an unsafe cast, but the only time this will call is when T is type TransformComponent anyway
			return (T*)(&transformComponent);
		}
		//Get data component
		if (std::is_base_of<DataComponent, T>())
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
		}
		//Get input component
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
		}
		//Get update component
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
		}
		//Get render component
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
		}

		//Couldn't find component, return nullptr
		return nullptr;
	}


protected:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	Renderer* renderer;

	TransformComponent transformComponent;

	std::vector<std::unique_ptr<DataComponent>> dataComponents;
	std::vector<std::unique_ptr<InputComponent>> inputComponents;
	std::vector<std::unique_ptr<UpdateComponent>> updateComponents;
	std::vector<std::unique_ptr<RenderComponent>> renderComponents;
};