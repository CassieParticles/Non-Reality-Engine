#include <iostream>

#include <engine/VectorMathOverloads.h>

#include <engine/Window.h>
#include <engine/View.h>

#include <engine/Pipeline/InputLayout.h>
#include <engine/Pipeline/Mesh.h>

#include <glfw3.h>


#include <engine/ResourceManager/ShaderManager.cpp>

#include <engine/Input.h>


#include <string>

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>("Test window", 800, 600);

	Microsoft::WRL::ComPtr<ID3D11Device> device = window->getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = window->getDeviceContext();

	std::unique_ptr<ShaderManager> shaderManager= std::make_unique<ShaderManager>( device );
	
	View view{ device.Get(),{0,0,-2} };

	view.setProjectionMatrixPespective(90 * 3.14159 / 180, window->getAspectRatio(), 0.1f, 1000.f);

	Input* input = window->getInput();

	Mesh mesh{};

	Mesh::MeshVertex vertices[3]
	{
		{	//Vertex 0
			DirectX::XMFLOAT3(0.0f,0.5f,0.1f),
			DirectX::XMFLOAT3(0,0, 1),
			DirectX::XMFLOAT2(0.5,0.0)
		},
		{	//Vertex 1
			DirectX::XMFLOAT3(0.5f,-0.5f,0.1f),
			DirectX::XMFLOAT3(0,0, 1),
			DirectX::XMFLOAT2(1.0,1.0)
		},
		{	//Vertex 2
			DirectX::XMFLOAT3(-0.5f,-0.5f,0.1f),
			DirectX::XMFLOAT3(0,0, 1),
			DirectX::XMFLOAT2(0.0,1.0)
		},
	};

	int indices[3]
	{
		0,1,2
	};


	mesh.addVertices(device.Get(), vertices, 3, indices, 3, false);

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
		mesh.useMesh(deviceContext.Get());

		vs->bindShader(deviceContext.Get());
		ps->bindShader(deviceContext.Get());



		ID3D11Buffer* cBuffers[1]{ view.getCameraBuffer() };

		deviceContext->VSSetConstantBuffers(0, 1, cBuffers);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		

		deviceContext->DrawIndexed(3, 0, 0);

		window->presentBackBuffer();
	}

	return 0;
}