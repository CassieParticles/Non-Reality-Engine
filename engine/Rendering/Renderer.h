#pragma once
#include <vector>
#include <wrl.h>

#include <graphicsEngine/Pipeline/Mesh.h>


//Contains information needed to draw an object
struct RenderCall
{
	Mesh* mesh;
	DirectX::XMFLOAT4X4 worldMatrix;
};

class Renderer
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Renderer();
	~Renderer();

	void pushDraw(Mesh* mesh);

	void render();
protected:
	std::vector<RenderCall> renderStack;
	RenderCall* base;
	RenderCall* end;

	//Used for rendering components
	ComPtr<ID3D11Buffer> worldMatrixBuffer;

	void reset();


};