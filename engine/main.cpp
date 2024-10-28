#include <iostream>

#include <graphicsEngine/VectorMathOverloads.h>

#include <graphicsEngine/Window.h>
#include <graphicsEngine/View.h>

#include <graphicsEngine/Pipeline/InputLayout.h>
#include <graphicsEngine/Pipeline/Mesh.h>

#include <glfw3.h>


#include <engine/ResourceManager/ShaderManager.cpp>
#include <engine/ResourceManager/MeshLoader.h>

#include <graphicsEngine/Input.h>

#include <engine/ObjectStructure/GameObject.h>

#include <engine/Rendering/MeshComponent.h>

#include <string>

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>("Test window", 800, 600);

	Microsoft::WRL::ComPtr<ID3D11Device> device = window->getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = window->getDeviceContext();

	std::unique_ptr<ShaderManager> shaderManager= std::make_unique<ShaderManager>( device );

	std::unique_ptr<MeshLoader> meshLoader = std::make_unique<MeshLoader>(device);

	GameObject gameObject{device,deviceContext};

	gameObject.addRenderComponent<MeshComponent>();

	MeshComponent* meshComp = gameObject.getComponent<MeshComponent>();
	meshComp->setMesh("Plane",meshLoader.get());
	TransformComponent* transComp = gameObject.getComponent<TransformComponent>();
	transComp->scale.x = 20;
	transComp->scale.y = 2;
	transComp->scale.z = 2;
	transComp->position.x = 3;

	//transComp->rotation.x = 3.14159 / 4;
	
	View view{ device.Get(),{0,1,-5} };

	view.setProjectionMatrixPespective(45 * 3.14159f / 180, window->getAspectRatio(), 0.1f, 1000.f);

	Input* input = window->getInput();

	InputLayout inputLayout{};

	VertexShader* vs = shaderManager->getVertexShader(L"shaders/vertex.hlsl");
	PixelShader* ps = shaderManager->getPixelShader(L"shaders/fragment.hlsl");


	D3D11_INPUT_ELEMENT_DESC inputArr[3]
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			sizeof(float) * 3,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			sizeof(float) * 6,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	inputLayout.addInputLayout(device.Get(), vs->getByteCode(), inputArr, 3);

	while (!window->getWindowShouldClose())
	{
		transComp->rotation.y += 0.001f;

		window->clearBackBuffer();
		window->bindRenderTarget();

		if (input->getKeyDown(GLFW_KEY_D))
		{
			view.setRotation(view.loadRotation() + DirectX::XMVECTOR{0, 0.001f, 0, 0});
		}
		if (input->getKeyDown(GLFW_KEY_A))
		{
			view.setRotation(view.loadRotation() + DirectX::XMVECTOR{0, -0.001f, 0, 0});

		}

		view.updateView(deviceContext.Get());

		inputLayout.useInputLayout(deviceContext.Get());
		//mesh->useMesh(deviceContext.Get());

		vs->bindShader(deviceContext.Get());
		ps->bindShader(deviceContext.Get());



		ID3D11Buffer* cBuffers[1]{ view.getCameraBuffer() };

		deviceContext->VSSetConstantBuffers(0, 1, cBuffers);



		//mesh2->useMesh(deviceContext.Get());
		meshComp->Render();

		window->presentBackBuffer();
	}

	return 0;
}