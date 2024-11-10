#pragma once

#include <vector>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Renderer.h>

class Scene
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer);
	~Scene();

	GameObject* createGameObject(int layer);

	void takeInput();
	void update();
	void renderLayer(int layer);
protected:
	std::vector<std::unique_ptr<GameObject>> layers[8];

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	Renderer* renderer;
};