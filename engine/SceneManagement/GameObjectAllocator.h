#pragma once

#include <vector>
#include <stack>
#include <wrl.h>
#include <d3d11.h>

#include <engine/SceneManagement/Scene.h>

class Timer;
class GameObject;
class Renderer;
class InputHandler;

class GameObjectAllocator
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObjectAllocator(wchar_t size);
	GameObjectAllocator(GameObjectAllocator& other) = delete;
	GameObjectAllocator(GameObjectAllocator&& other);
	~GameObjectAllocator();

	//Create Game object on the stack, then return pointer to the game object
	GameObject* CreateGameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,Renderer* renderer,InputHandler* input);

	//Delete game object, set it's data to null, then add it's index to the stack
	void DestroyGameObject(GameObject* gameObject);

	friend GameObject* Scene::moveGameObject(GameObjectAllocator* from, GameObjectAllocator* to, GameObject* gameObject);
	
	void handleInput();
	void Update(Timer* timer);
	void PhysUpdate(Timer* timer);
	void Render(bool renderPortals);
protected:
	int getIndex(GameObject* gameObject);

	int gameObjectCount;
	int size;
	std::vector<char> gameObjectArray;
	std::stack<wchar_t> freeIndexStack;
};