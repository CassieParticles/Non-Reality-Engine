#pragma once
#include <engine/ObjectStructure/RenderComponent.h>

#include <graphicsEngine/Pipeline/Mesh.h>
#include <engine/ResourceManager/MeshLoader.h>

#include <string>

class MeshComponent :public RenderComponent
{
public:
	MeshComponent(GameObject* gameObject);
	MeshComponent(MeshComponent& other);
	MeshComponent(MeshComponent&& other);
	~MeshComponent();

	void setMesh(const std::string& name, MeshLoader* meshLoader);

	void Render() override;
protected:
	Mesh* mesh;

	Microsoft::WRL::ComPtr<ID3D11Buffer> worldMatrixBuffer;
};