#pragma once

#include <memory>
#include <string>

class Window;
class Input;
class ShaderManager;
class MeshLoader;
class Renderer;

class VertexShader;
class PixelShader;

#include <graphicsEngine/Input.h>
#include <graphicsEngine/View.h>
#include <engine/SceneManagement/Timer.h>
#include <engine/SceneManagement/Scene.h>

class BaseGameLoop
{
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	BaseGameLoop(const std::string& windowName, int windowWidth, int windowHeight);
	~BaseGameLoop();

	virtual void handleInput();
	virtual void update();
	virtual void render();

	bool getShouldClose() { return window->getWindowShouldClose(); }
protected:
	//Systems
	std::unique_ptr<Window> window;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<MeshLoader> meshLoader;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Scene> scene;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	//TEMP - Improved input system will be created
	Input* input;

	std::unique_ptr<Timer> timer;

protected:

};