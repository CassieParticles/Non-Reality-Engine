#include <iostream>

#include <engine/Window.h>
#include <engine/Shader.h>
#include <engine/InputLayout.h>
#include <engine/Mesh.h>

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>("Test window", 800, 600);

	Microsoft::WRL::ComPtr<ID3D11Device> device = window->getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = window->getDeviceContext();

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

	VertexShader vs{ device.Get(),L"shaders/vertex.hlsl" };
	PixelShader ps{ device.Get(),L"shaders/fragment.hlsl" };

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

	inputLayout.addInputLayout(device.Get(), vs.getByteCode(), inputArr, 3);

	while (!window->getWindowShouldClose())
	{
		window->clearBackBuffer();
		window->bindRenderTarget();

		inputLayout.useInputLayout(deviceContext.Get());
		mesh.useMesh(deviceContext.Get());

		vs.bindShader(deviceContext.Get());
		ps.bindShader(deviceContext.Get());

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		

		deviceContext->DrawIndexed(3, 0, 0);

		window->presentBackBuffer();
	}

	return 0;
}