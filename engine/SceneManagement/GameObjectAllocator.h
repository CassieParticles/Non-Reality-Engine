#pragma once

#include <wrl.h>
#include <vector>
#include <stack>
#include <d3d11.h>


class Timer;
class GameObject;
class Renderer;

class GameObjectAllocator
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	GameObjectAllocator(wchar_t size);
	GameObjectAllocator(GameObjectAllocator& other) = delete;
	GameObjectAllocator(GameObjectAllocator&& other) = delete;
	~GameObjectAllocator();

	//Create Game object on the stack, then return pointer to the game object
	GameObject* createGameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,Renderer* renderer);
	//Delete game object, set it's data to null, then add it's index to the stack
	void DestroyGameObject(GameObject* gameObject);
	
	void handleInput();
	void Update(Timer* timer);
	void Render();
protected:
	std::vector<char> gameObjectArray;
	int gameObjectCount;
	std::stack<wchar_t> freeIndexStack;
};