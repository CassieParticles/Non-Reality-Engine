#include "Renderer.h"

#include <new>

#include <engine/Rendering/Components/CameraComponent.h>
#include <engine/ObjectStructure/DefaultComponents/TransformComponent.h>
#include <engine/ObjectStructure/GameObject.h>

Renderer::Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ShaderManager* shaderManager, size_t renderStackInitialSize):device{device},deviceContext{deviceContext}
{
	mainCamera = nullptr;
	renderQueue.resize(renderStackInitialSize);
	head = 0;
	drawCallCount = 0;

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Both buffers are identical
	device->CreateBuffer(&bufferDesc, 0, &cameraMatrixBuffer);
	device->CreateBuffer(&bufferDesc, 0, &worldMatrixBuffer);

	vertexShader = shaderManager->getVertexShader(L"shaders/vertex.hlsl");
	pixelShader = shaderManager->getPixelShader(L"shaders/fragment.hlsl");

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

	inputLayout.addInputLayout(device.Get(), vertexShader->getByteCode(), inputArr, 3);
}

Renderer::~Renderer()
{
}

void Renderer::addMainCamera(GameObject* component)
{
	if (mainCamera) { return; }
	this->mainCamera = component;
}

void Renderer::draw()
{
	//Set parameters for initial rendering
	InitRender();

	//Iterate through each draw call
	char* current = renderQueue.data();
	for (int i = 0; i < drawCallCount; ++i)
	{
		if (*current == 0)
		{
			DrawMesh* meshCall = (DrawMesh*)current;
			RenderMesh(meshCall->mesh, meshCall->worldMatrix);
			current += sizeof(DrawMesh);
		}
		if (*current == 1)
		{
			//Begin portal
			current += sizeof(PortalBegin);
		}
		if (*current == 2)
		{
			//End portal
			current += sizeof(PortalEnd);
		}
	}
	//Draw calls are all done, clear queue
	head = 0;
	mainCamera = nullptr;
	drawCallCount = 0;
}

void Renderer::resize()
{
	renderQueue.resize(renderQueue.size() * 2);
}

void Renderer::setMainCamera()
{
	CameraComponent* camera = mainCamera->getComponent<CameraComponent>();
	TransformComponent* transform = mainCamera->getComponent<TransformComponent>();

	DirectX::XMMATRIX cameraMatrix = camera->getProjectionMatrix();
	cameraMatrix = cameraMatrix * transform->calcViewMatrix();

	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(cameraMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	DirectX::XMMATRIX* data = (DirectX::XMMATRIX*)mappedData.pData;
	*data = cameraMatrix;
	deviceContext->Unmap(cameraMatrixBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(0, 1, cameraMatrixBuffer.GetAddressOf());
}

void Renderer::InitRender()
{
	setMainCamera();

	//Clear window back buffer
	//Set window render target

	//Set shaders
	vertexShader->bindShader(deviceContext.Get());
	pixelShader->bindShader(deviceContext.Get());

	//Set input layout
	inputLayout.useInputLayout(deviceContext.Get());

}

void Renderer::RenderMesh(Mesh* mesh, DirectX::XMFLOAT4X4 worldMatrix)
{
	//Set mesh's world matrix
	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(worldMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	DirectX::XMFLOAT4X4* data = (DirectX::XMFLOAT4X4*)mappedData.pData;
	*data = worldMatrix;
	deviceContext->Unmap(worldMatrixBuffer.Get(), 0);

	mesh->useMesh(deviceContext.Get());
	deviceContext->VSSetConstantBuffers(1, 1, worldMatrixBuffer.GetAddressOf());

	deviceContext->DrawIndexed(mesh->getVertexCount(), 0, 0);
}

template<>
void Renderer::addRenderCall<DrawMesh>(DrawMesh drawCall)
{
	drawCall.flag = 0;
	addRenderCallPriv<DrawMesh>(drawCall);
}

template<>
void Renderer::addRenderCall<PortalBegin>(PortalBegin drawCall)
{
	drawCall.flag = 1;
	addRenderCallPriv<PortalBegin>(drawCall);
}

template<>
void Renderer::addRenderCall<PortalEnd>(PortalEnd drawCall)
{
	drawCall.flag = 2;
	addRenderCallPriv<PortalEnd>(drawCall);
}