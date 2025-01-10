#include "BaseGameLoop.h"
#include "BaseGameLoop.h"

#include <graphicsEngine/Window.h>
#include <engine/ResourceManager/ShaderManager.h>
#include <engine/ResourceManager/MeshLoader.h>
#include <engine/ResourceManager/TextureLoader.h>
#include <engine/Rendering/Renderer.h>


#include <imgui.h>
#include <imgui_impl_dx11.h>
//#include <imgui_impl_glfw.h>
#include <imgui_impl_glfw.cpp>	//Evil solution 

BaseGameLoop::BaseGameLoop(const std::string& windowName, int windowWidth, int windowHeight)
{
	//Initialize window
	window = std::make_unique<Window>(windowName, windowWidth, windowHeight);
	input = window->getInput();

	//Get the device and device context
	device = window->getDevice();
	deviceContext = window->getDeviceContext();

	//Initialize other systems
	shaderManager = std::make_unique<ShaderManager>(device);
	meshLoader = std::make_unique<MeshLoader>(device);
	textureLoader = std::make_unique<TextureLoader>(device);

	renderer = std::make_unique<Renderer>(device, deviceContext, shaderManager.get(),textureLoader.get(),meshLoader.get(), window.get(), 1024);

	timer = std::make_unique<Timer>();

	scene = std::make_unique<Scene>(device, deviceContext, renderer.get(),input,this);

	
	//Set up dearImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOther(window->getWindow(), true);
	ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
}

BaseGameLoop::~BaseGameLoop()
{
}

void BaseGameLoop::loop()
{
	//Check if loop calculation should occur
	bool frame = timer->Update();
	//Do frame every set frame
	if (!frame) { return; }
	//Pre-loop 
	
	//Initialize imgui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	//Game loop
	handleInput();
	update();
	physUpdate();
	render();
	guiRender();

	//Post loop
	renderer->draw();

	//Render imgui to screen
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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

void BaseGameLoop::physUpdate()
{
	scene->PhysUpdate(timer.get());
}

void BaseGameLoop::render()
{
	window->clearBackBuffer();
	window->bindRenderTarget();
	scene->renderLayer((*playerObject)->getComponent<TransformComponent>()->layer,drawPortalInternals);
}

void BaseGameLoop::guiRender()
{
}

void BaseGameLoop::exit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
