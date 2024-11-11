#include "Scene.h"
#include "Scene.h"

Scene::Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer) :device{ device }, deviceContext{ deviceContext }, renderer{ renderer }, layers{65535,65535,65535,65535,65535,65535,65535,65535}
{
}

Scene::~Scene()
{
}

GameObject* Scene::createGameObject(int layer)
{
	return layers[layer].createGameObject(device, deviceContext, renderer);
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

void Scene::renderLayer(int layer)
{
	layers[layer].Render();
}
