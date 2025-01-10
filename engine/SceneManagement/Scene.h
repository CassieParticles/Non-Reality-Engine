#pragma once

#include <vector>

#include <engine/ObjectStructure/GameObject.h>
#include <engine/Rendering/Renderer.h>



class Timer;
class GameObjectAllocator;
class BaseGameLoop;

class Scene
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Scene(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, Renderer* renderer, InputHandler* input,BaseGameLoop* baseGameLoop);
	~Scene();

	GameObject* CreateGameObject(int layer);
	void destroyGameObject(GameObject* object, int layer);



	//DONT CALL DIRECTLY, use the other version, this is only public since it is a friend function
	GameObject* moveGameObject(GameObjectAllocator* from, GameObjectAllocator* to, GameObject* gameObject);
	GameObject* moveGameObject(int from, int to, GameObject* gameObject);

	void takeInput();
	void update(Timer* timer);
	void PhysUpdate(Timer* timer);
	void renderLayer(int layer, bool renderPortals);

protected:

	std::vector<GameObjectAllocator> layers;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;


	Renderer* renderer;
	InputHandler* input;
	BaseGameLoop* baseGameLoop;
};