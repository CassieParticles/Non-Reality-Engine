#include "BaseGameLoop.h"
#include "BaseGameLoop.h"

#include <graphicsEngine/Window.h>
#include <engine/ResourceManager/ShaderManager.h>
#include <engine/ResourceManager/MeshLoader.h>
#include <engine/Rendering/Renderer.h>

BaseGameLoop::BaseGameLoop(const std::string& windowName, int windowWidth, int windowHeight)
{
	//Initialize window
	window = std::make_unique<Window>(windowName, 800, 600);
	input = window->getInput();

	//Get the device and device context
	device = window->getDevice();
	deviceContext = window->getDeviceContext();

	//Initialize other systems
	shaderManager = std::make_unique<ShaderManager>(device);
	meshLoader = std::make_unique<MeshLoader>(device);

	renderer = std::make_unique<Renderer>(device, deviceContext, shaderManager.get(), 1024);

	timer = std::make_unique<Timer>();

	scene = std::make_unique<Scene>(device, deviceContext, renderer.get(),input);
}

BaseGameLoop::~BaseGameLoop()
{
}

void BaseGameLoop::loop()
{
	//Pre-loop 
	bool frame = timer->Update();
	//Do frame every set frame
	if (!frame) { return; }

	//Game loop
	handleInput();
	update();
	render();

	//Post loop
	renderer->draw();
	window->presentBackBuffer();
}

void BaseGameLoop::handleInput()
{
	input->Update();

	scene->takeInput();
}

void BaseGameLoop::update()
{
	scene->update(timer.get());
}

void BaseGameLoop::render()
{
	scene->renderLayer(0);
}
