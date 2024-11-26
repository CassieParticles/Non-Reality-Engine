#include "Renderer.h"
#include "Renderer.h"
#include "Renderer.h"

#include <new>

#include <engine/Rendering/Components/PlayerCameraComponent.h>
#include <engine/ObjectStructure/DefaultComponents/TransformComponent.h>
#include <engine/ObjectStructure/GameObject.h>
#include <engine/ResourceManager/MeshLoader.h>

#include <graphicsEngine/Window.h>
#include <graphicsEngine/Pipeline/Texture2D.h>

//Forward declare the addRenderCalls to allow renderer to use it's own interface

template<>
void Renderer::addRenderCall<DrawMesh>(DrawMesh drawCall);

template<>
void Renderer::addRenderCall<DrawPortalSurface>(DrawPortalSurface drawCall);

template<>
void Renderer::addRenderCall<PortalEnd>(PortalEnd drawCall);

template<>
void Renderer::addRenderCall<ChangeShaders>(ChangeShaders drawCall);

Renderer::Renderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, ShaderManager* shaderManager, TextureLoader* textureLoader, MeshLoader* meshLoader, Window* window, size_t renderStackInitialSize) :device{ device }, deviceContext{ deviceContext },window { window }
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

	defaultVertexShader = shaderManager->getVertexShader(L"shaders/vertex.hlsl");
	defaultPixelShader = shaderManager->getPixelShader(L"shaders/fragment.hlsl");

	screenVertexShader = shaderManager->getVertexShader(L"shaders/renderScreenVertex.hlsl");
	screenPixelShader = shaderManager->getPixelShader(L"shaders/renderScreenPixel.hlsl");

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

	inputLayout.addInputLayout(device.Get(), defaultVertexShader->getByteCode(), inputArr, 3);

	//Create depth stencil states
	D3D11_DEPTH_STENCIL_DESC desc{};

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = false;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFunc = desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilFailOp = desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	device->CreateDepthStencilState(&desc, &defaultDepthStencil);

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = true;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	device->CreateDepthStencilState(&desc, &portalSurfaceDepthStencil);

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = true;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	device->CreateDepthStencilState(&desc, &portalInsideDepthStencil);

	D3D11_TEXTURE2D_DESC texDesc{};

	texDesc.Width = 1024;
	texDesc.Height = 1024;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc = { 1,0 };
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	defaultRenderTarget = textureLoader->loadTextureFromData("DefaultRenderTarget", texDesc, nullptr, 16);
	portalRenderTarget = textureLoader->loadTextureFromData("PortalRenderTarget", texDesc, nullptr, 16);
	
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	defaultDepthStencilTarget = textureLoader->loadTextureFromData("DefaultDepthStencilTarget", texDesc, nullptr, 16);

	screenMesh = meshLoader->getMesh("Quad");
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
	if (!mainCamera)
	{
		return;
	}
	//Set parameters for initial rendering
	InitRender();

	//Iterate through each draw call
	char* current = renderQueue.data();
	int index = 0;
	for (int i = 0; i < drawCallCount; ++i)
	{
		if (*current == 0)
		{
			DrawMesh* meshCall = (DrawMesh*)current;
			RenderMesh(meshCall->mesh,meshCall->texture, meshCall->worldMatrix);
			current += sizeof(DrawMesh);
			index += sizeof(DrawMesh);
		}
		else if (*current == 1)
		{
			DrawPortalSurfaceFunc();
			current += sizeof(DrawPortalSurface);
			index+=sizeof(DrawPortalSurface);
		}
		else if (*current == 2)
		{
			ResetPortalData();
			current += sizeof(PortalEnd);
			index+=sizeof(PortalEnd);
		}
		
		else if (*current == 3)
		{
			//Change shaders
			ChangeShaders* changeShaders = (ChangeShaders*)current;
			ChangeShadersFunc(changeShaders->vs, changeShaders->ps);
			current += sizeof(ChangeShaders);
			index+=sizeof(ChangeShaders);
		}
	}
	//Draw calls are all done, clear queue
	head = 0;
	mainCamera = nullptr;
	drawCallCount = 0;

	//Draw defaultrendertarget to window render target
	window->bindRenderTarget();
	//Set shaders to draw to screen
	//screenVertexShader->bindShader(deviceContext.Get());
	//screenPixelShader->bindShader(deviceContext.Get());

	ID3D11ShaderResourceView* defaultSRV[1]{defaultRenderTarget->getSRV()};
	ID3D11ShaderResourceView* emptySRV[1]{nullptr};

	deviceContext->PSSetShaderResources(0, 1, defaultSRV);

	screenMesh->useMesh(deviceContext.Get());

	deviceContext->DrawIndexed(6, 0, 0);

	deviceContext->PSGetShaderResources(0, 1, emptySRV);

	//Add setting default shaders to front of render queue
	addRenderCall<ChangeShaders>({ 0,defaultVertexShader,defaultPixelShader });
}

void Renderer::resize()
{
	renderQueue.resize(renderQueue.size() * 2);
}

void Renderer::setMainCamera()
{
	PlayerCameraComponent* camera = mainCamera->getComponent<PlayerCameraComponent>();
	TransformComponent* transform = mainCamera->getComponent<TransformComponent>();

	DirectX::XMMATRIX cameraMatrix = camera->getProjectionMatrix();
	cameraMatrix = transform->calcViewMatrix() * cameraMatrix;

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

	//Set input layout
	inputLayout.useInputLayout(deviceContext.Get());

	deviceContext->OMSetDepthStencilState(defaultDepthStencil.Get(), 1);

	ID3D11RenderTargetView* rtv[1] = { defaultRenderTarget->getRTV() };
	float c[4] = { 0.2,0.4,0.6,1.0 };

	deviceContext->ClearRenderTargetView(rtv[0], c);
	deviceContext->ClearDepthStencilView(defaultDepthStencilTarget->getDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetRenderTargets(1, rtv, defaultDepthStencilTarget->getDSV());

	//Add final render calls to draw to window rtv
	addRenderCall<ChangeShaders>({ 0,screenVertexShader,screenPixelShader });

}

void Renderer::RenderMesh(Mesh* mesh,Texture2D* texture, DirectX::XMFLOAT4X4 worldMatrix)
{
	//Set mesh's world matrix
	D3D11_MAPPED_SUBRESOURCE mappedData;
	deviceContext->Map(worldMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	DirectX::XMFLOAT4X4* data = (DirectX::XMFLOAT4X4*)mappedData.pData;
	*data = worldMatrix;
	deviceContext->Unmap(worldMatrixBuffer.Get(), 0);

	mesh->useMesh(deviceContext.Get());
	deviceContext->VSSetConstantBuffers(1, 1, worldMatrixBuffer.GetAddressOf());
	ID3D11ShaderResourceView* srvs[1]{ nullptr };
	if (texture)
	{
		srvs[0] = texture->getSRV();
	}
	deviceContext->PSSetShaderResources(0, 1, srvs);

	deviceContext->DrawIndexed(mesh->getVertexCount(), 0, 0);
}

void Renderer::DrawPortalSurfaceFunc()
{
	//Set depth stencil state to portalSurface depth stencil state
	deviceContext->OMSetDepthStencilState(portalSurfaceDepthStencil.Get(), 1);

	float clearColour[4] = { 0,0,0,1 };
	//Clear portal internal render target, and set render targets
	deviceContext->ClearRenderTargetView(portalRenderTarget->getRTV(), clearColour);
	deviceContext->OMSetRenderTargets(0, nullptr, defaultDepthStencilTarget->getDSV());
}

void Renderer::ResetPortalData()
{
	deviceContext->OMSetDepthStencilState(defaultDepthStencil.Get(), 1);

	ID3D11RenderTargetView* rtv[1] = { defaultRenderTarget->getRTV() };
	deviceContext->OMSetRenderTargets(1, rtv, defaultDepthStencilTarget->getDSV());
}

void Renderer::ChangeShadersFunc(VertexShader* vs, PixelShader* ps)
{
	vs->bindShader(deviceContext.Get());
	ps->bindShader(deviceContext.Get());
	//std::cout << "Changing shaders\n";
}

template<>
void Renderer::addRenderCall<DrawMesh>(DrawMesh drawCall)
{
	drawCall.flag = 0;
	addRenderCallPriv<DrawMesh>(drawCall);
}

template<>
void Renderer::addRenderCall<DrawPortalSurface>(DrawPortalSurface drawCall)
{
	drawCall.flag = 1;
	addRenderCallPriv<DrawPortalSurface>(drawCall);
}

template<>
void Renderer::addRenderCall<PortalEnd>(PortalEnd drawCall)
{
	drawCall.flag = 2;
	addRenderCallPriv<PortalEnd>(drawCall);

	ID3D11RenderTargetView* rtv[1] = { defaultRenderTarget->getRTV() };
}

template<>
void Renderer::addRenderCall<ChangeShaders>(ChangeShaders drawCall)
{
	drawCall.flag = 3;
	addRenderCallPriv<ChangeShaders>(drawCall);
}

