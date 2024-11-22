#pragma once

#include <vector>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Renderer.h>

class Timer;
class GameObjectAllocator;

class Scene
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer, InputHandler* input);
	~Scene();

	GameObject* CreateGameObject(int layer);
	void destroyGameObject(GameObject* object, int layer);

	void takeInput();
	void update(Timer* timer);
	void renderLayer(int layer);
protected:
	std::vector<GameObjectAllocator> layers;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	Renderer* renderer;
	InputHandler* input;
};