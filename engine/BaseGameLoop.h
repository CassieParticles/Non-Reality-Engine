#pragma once

#include <memory>
#include <string>

#include <graphicsEngine/Window.h>

#include <engine/ResourceManager/ShaderManager.h>
#include <engine/ResourceManager/MeshLoader.h>
#include <engine/ResourceManager/TextureLoader.h>
#include <engine/Rendering/Renderer.h>

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

	void loop();

	virtual void handleInput();
	virtual void update();
	virtual void physUpdate();
	virtual void render();
	virtual void guiRender();
	virtual void exit();

	void setPlayer(GameObject** gameObject) { playerObject = gameObject; }
	GameObject** getPlayerVar() { return playerObject; }

	bool getShouldClose() { return window->getWindowShouldClose(); }
protected:
	//Systems
	std::unique_ptr<Window> window;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<MeshLoader> meshLoader;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Scene> scene;
	std::unique_ptr<TextureLoader> textureLoader;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	//TEMP - Improved input system will be created
	InputHandler* input;

	std::unique_ptr<Timer> timer;

	bool drawPortalInternals=true;


protected:
	//This is needed to track which object the player is on 
	GameObject** playerObject{};

};