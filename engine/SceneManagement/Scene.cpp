#include "Scene.h"

Scene::Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer):device{device},deviceContext{deviceContext},renderer{renderer}
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

}
