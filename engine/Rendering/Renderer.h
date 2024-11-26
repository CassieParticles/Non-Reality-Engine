#pragma once
#include <vector>
#include <wrl.h>

#include <engine/ResourceManager/ShaderManager.h>
#include <engine/ResourceManager/TextureLoader.h>

#include <graphicsEngine/Pipeline/Mesh.h>
#include <graphicsEngine/Pipeline/InputLayout.h>
#include <graphicsEngine/Pipeline/Shader.h>
#include <graphicsEngine/Pipeline/Texture2D.h>

//Draw call is packed tightly to save space
#pragma pack(push, 1)
struct DrawMesh
{
	byte flag;
	Mesh* mesh;
	Texture2D* texture;
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct PortalBegin
{
	byte flag;
};

struct PortalEnd
{
	byte flag;
};

#pragma pack(pop)

class GameObject;
class Window;
class MeshLoader;

class Renderer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Renderer(ComPtr<ID3D11Device> device,ComPtr<ID3D11DeviceContext> deviceContext,ShaderManager* shaderManager,TextureLoader* textureLoader, MeshLoader* meshLoader,Window* window,size_t renderStackInitialSize);
	Renderer(Renderer& other);
	Renderer(Renderer&& other);
	~Renderer();
	Renderer& operator=(Renderer& other) { return other; }

	//Public call, ensures T is one of 3 calls above
	template<typename T>
	void addRenderCall(T drawCall);

	void addMainCamera(GameObject* component);

	void draw();

	ID3D11ShaderResourceView* getRenderOutput() { return defaultRenderTarget->getSRV(); }
protected:
	//Head is the index of the head, to get head ptr, do renderQueue.data() + head
	std::vector<char> renderQueue;
	int head;
	int drawCallCount;

	//Device and context for D3D11, window for the default render targets
	Window* window;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	//Parameters needed for draw calls
	GameObject* mainCamera;

	ComPtr<ID3D11Buffer> cameraMatrixBuffer;	
	ComPtr<ID3D11Buffer> worldMatrixBuffer;

	InputLayout inputLayout;
	VertexShader* defaultVertexShader;
	PixelShader* defaultPixelShader;

	VertexShader* screenVertexShader;
	PixelShader* screenPixelShader;
	Mesh* screenMesh;

	ComPtr<ID3D11DepthStencilState> defaultDepthStencil;
	ComPtr<ID3D11DepthStencilState> portalSurfaceDepthStencil;
	ComPtr<ID3D11DepthStencilState> portalInsideDepthStencil;

	Texture2D* defaultRenderTarget;
	Texture2D* defaultDepthStencilTarget;

	Texture2D* portalRenderTarget;

	void resize();

	void setMainCamera();
	//Functions to carry out draw calls
	void InitRender();	//Called before all draw calls
	void RenderMesh(Mesh* mesh, Texture2D* texture, DirectX::XMFLOAT4X4 worldMatrix);

	//private call, since adding drawCall is same, this is better then repeating, but I want to prevent implicit instantiation for non-draw calls
	template<typename T>
	void addRenderCallPriv(T drawCall)
	{
		size_t callSize = sizeof(drawCall);
		size_t spaceLeft = renderQueue.size() - head;
		if (callSize > spaceLeft)
		{
			resize();
		}
		T* newItem = new(renderQueue.data() + head) T(drawCall);
		head += callSize;
		drawCallCount++;
	}
};