#pragma once

#include <vector>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Renderer.h>
#include <engine/SceneManagement/GameObjectAllocator.h>

class Timer;

class Scene
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer, InputHandler* input);
	~Scene();

	GameObject* createGameObject(int layer);
	void destroyGameObject(GameObject* object, int layer);

	void takeInput();
	void update(Timer* timer);
	void renderLayer(int layer);
protected:
	GameObjectAllocator layers[8];

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	Renderer* renderer;
	InputHandler* input;
};