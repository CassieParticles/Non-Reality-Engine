#include "BaseGameLoop.h"

#include <graphicsEngine/Window.h>
#include <engine/ResourceManager/ShaderManager.h>
#include <engine/ResourceManager/MeshLoader.h>
#include <engine/Rendering/Renderer.h>

BaseGameLoop::BaseGameLoop(const std::string& windowName, int windowWidth, int windowHeight)
{
	//Initialize window
	window = std::make_unique<Window>(windowName, 800, 600);

	//Get the device and device context
	device = window->getDevice();
	deviceContext = window->getDeviceContext();

	//Initialize other systems
	shaderManager = std::make_unique<ShaderManager>(device);
	meshLoader = std::make_unique<MeshLoader>(device);

	renderer = std::make_unique<Renderer>(device, deviceContext, shaderManager.get(), 1024);

	scene = std::make_unique<Scene>(device, deviceContext, renderer.get());

	input = window->getInput();
}

BaseGameLoop::~BaseGameLoop()
{
}

void BaseGameLoop::handleInput()
{
	input->Update();
}

void BaseGameLoop::update()
{
}

void BaseGameLoop::render()
{
}
