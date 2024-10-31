#pragma once

#include <memory>
#include <string>

class Window;
class Input;
class ShaderManager;
class MeshLoader;

class VertexShader;
class PixelShader;

//TEMP - Will be put in renderer
#include <graphicsEngine/Pipeline/InputLayout.h>

class BaseGameLoop
{
public:
	BaseGameLoop(const std::string& windowName, int windowWidth, int windowHeight);
	~BaseGameLoop();

	void handleInputs();
	void update();
	void render();
protected:
	//Systems
	std::unique_ptr<Window> window;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<MeshLoader> meshLoader;

private:
	VertexShader* baseVertexShader;
	PixelShader* basePixelShader;

	//TEMP - Improved input system will be created
	//Input* input;
	Input* input;

	//TEMP - Will be put in renderer
	InputLayout inputLayout;
};