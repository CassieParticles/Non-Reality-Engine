#include "Scene.h"

Scene::Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer):device{device},deviceContext{deviceContext},renderer{renderer}
{
}

Scene::~Scene()
{
}

GameObject* Scene::createGameObject(int layer)
{
	GameObject* gameObject = new GameObject(device, deviceContext, renderer);
	layers[layer].emplace_back(gameObject);

	return gameObject;
}

void Scene::takeInput()
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < layers[i].size(); ++i)
		{
			layers[i].at(j)->HandleInput();
		}
	}
}

void Scene::update(Timer* timer)
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < layers[i].size(); ++i)
		{
			layers[i].at(j)->Update(timer);
		}
	}
}

void Scene::renderLayer(int layer)
{
	for (int i = 0; i < layers[layer].size(); ++i)
	{
		layers[layer].at(i)->Render();
	}
}
