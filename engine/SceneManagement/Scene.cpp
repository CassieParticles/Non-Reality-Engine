#include "Scene.h"
#include "Scene.h"
#include "Scene.h"

#include "GameObjectAllocator.h"

Scene::Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer,InputHandler* input) :device{ device }, deviceContext{ deviceContext }, renderer{ renderer },input{input}
{
	std::cout << "Create scene\n";
	for (int i = 0; i < 8; ++i)
	{
		layers.emplace_back( 65535 );
	}
}

Scene::~Scene()
{
}

GameObject* Scene::CreateGameObject(int layer)
{
	return layers[layer].CreateGameObject(device, deviceContext, renderer,input);
}

void Scene::destroyGameObject(GameObject* object, int layer)
{
	layers[layer].DestroyGameObject(object);
}

void Scene::takeInput()
{
	for (int i = 0; i < 8; ++i)
	{
		layers[i].handleInput();
	}
}

void Scene::update(Timer* timer)
{
	for (int i = 0; i < 8; ++i)
	{
		layers[i].Update(timer);
	}
}

void Scene::renderLayer(int layer, bool renderPortals)
{
	layers[layer].Render(renderPortals);
}

GameObject* Scene::moveGameObject(GameObjectAllocator* from, GameObjectAllocator* to, GameObject* gameObject)
{
	//Get the index of the game object, and the index of where it will go
	int oldIndex = from->getIndex(gameObject);
	int newIndex = to->freeIndexStack.top();
	to->freeIndexStack.pop();

	//Get the address of where the object will be copied to
	GameObject* newPtr = (GameObject*) (to->gameObjectArray.data() + newIndex * sizeof(GameObject));
	
	//Copy the data over (fucked up and evil)
	memcpy(newPtr, gameObject, sizeof(GameObject));

	newPtr->moveGOLocation();

	//Destroy old game object (don't ues builtin method, we don't want to call the destructor)
	*(char*)gameObject = 0;
	from->freeIndexStack.push(oldIndex);
	--from->gameObjectCount;

	//Increment gameObjectCount of old array
	++to->gameObjectCount;

	

	return newPtr;
}

GameObject* Scene::moveGameObject(int from, int to, GameObject* gameObject)
{
	if (from > layers.size() || from < 0 || to > layers.size() || to < 0)
	{
		std::cerr << "Layer provided is invalid\n";
		return gameObject;
	}

	GameObjectAllocator* fromPtr = &layers[from];
	GameObjectAllocator* toPtr = &layers[to];

	GameObject* go = moveGameObject(fromPtr, toPtr, gameObject);

	go->getComponent<TransformComponent>()->layer = to;

	return go;
}
