#include "GameObjectAllocator.h"

#include <engine/ObjectStructure/GameObject.h>

GameObjectAllocator::GameObjectAllocator(wchar_t initialSize)
{
	gameObjectArray.resize(initialSize * sizeof(GameObject));
	for (int i = initialSize - 1; i >= 0; --i)
	{
		freeIndexStack.push(i);
	}
}

GameObjectAllocator::~GameObjectAllocator()
{
}

GameObject* GameObjectAllocator::createGameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,Renderer* renderer)
{
	//If array is empty, allocate new space
	if (freeIndexStack.empty())
	{
		std::cerr << "cannot add game object\n";
		return nullptr;
	}

	wchar_t index = freeIndexStack.top();
	freeIndexStack.pop();
	int offset = index * sizeof(GameObject);

	GameObject* go = new(gameObjectArray.data() + offset) GameObject(device, deviceContext, renderer);

	return go;
}
void GameObjectAllocator::DestroyGameObject(GameObject* gameObject)
{
	//Get the index within the array
	int index = gameObject - (GameObject*)gameObjectArray.data();
	//Call the destructor of GameObject
	gameObject->~GameObject();
	//Set the first byte to 0, to signal this game object is destroyed
	*(char*)gameObject = 0;
	freeIndexStack.push(index);
}

void GameObjectAllocator::handleInput()
{
	for (int i = 0; i < gameObjectArray.size(); i+=sizeof(GameObject))
	{
		//Skip empty objects
		if (gameObjectArray.at(i) == 0) { continue; }

		GameObject* GO = (GameObject*)(gameObjectArray.data() + i);
		
		GO->HandleInput();
	}
}

void GameObjectAllocator::Update(Timer* timer)
{
	for (int i = 0; gameObjectArray.size(); i += sizeof(GameObject))
	{
		//Skip empty objects
		if (gameObjectArray.at(i) == 0) { continue; }

		GameObject* GO = (GameObject*)(gameObjectArray.data() + i);

		GO->Update(timer);
	}
}

void GameObjectAllocator::Render()
{
	for (int i = 0; gameObjectArray.size(); i += sizeof(GameObject))
	{
		//Skip empty objects
		if (gameObjectArray.at(i) == 0) { continue; }

		GameObject* GO = (GameObject*)(gameObjectArray.data() + i);

		GO->Render();
	}
}