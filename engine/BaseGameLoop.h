#pragma once

#include <memory>
#include <string>

class Window;
class Input;
class ShaderManager;
class MeshLoader;

class VertexShader;
class PixelShader;

#include <graphicsEngine/Input.h>
#include <graphicsEngine/View.h>

//TEMP - Will be put in renderer
#include <graphicsEngine/Pipeline/InputLayout.h>

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

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	//TEMP - Improved input system will be created
	Input* input;

	//TEMP - Will be put in renderer
	InputLayout inputLayout;

	//TEMP - This will be private when shaders aren't directly needed
protected:
	VertexShader* baseVertexShader;
	PixelShader* basePixelShader;

};