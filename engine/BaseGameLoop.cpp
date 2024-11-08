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

	input = window->getInput();

	//Compile shaders for the engine
	//baseVertexShader = shaderManager->getVertexShader(L"shaders/vertex.hlsl");
	//basePixelShader = shaderManager->getPixelShader(L"shaders/fragment.hlsl");


	//TEMP - This will be in renderer
	//D3D11_INPUT_ELEMENT_DESC inputArr[3]
	//{
	//	{
	//		"POSITION",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		0,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"NORMAL",
	//		0,
	//		DXGI_FORMAT_R32G32B32_FLOAT,
	//		0,
	//		sizeof(float) * 3,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//	{
	//		"TEXCOORD",
	//		0,
	//		DXGI_FORMAT_R32G32_FLOAT,
	//		0,
	//		sizeof(float) * 6,
	//		D3D11_INPUT_PER_VERTEX_DATA,
	//		0
	//	},
	//};

	//inputLayout.addInputLayout(device.Get(), baseVertexShader->getByteCode(), inputArr, 3);
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
