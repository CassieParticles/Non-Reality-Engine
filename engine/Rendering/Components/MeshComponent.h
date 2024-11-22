#pragma once
#include <engine/ObjectStructure/RenderComponent.h>

#include <graphicsEngine/Pipeline/Mesh.h>
#include <engine/ResourceManager/MeshLoader.h>
#include <engine/ResourceManager/TextureLoader.h>
#include <engine/Rendering/Renderer.h>

#include <string>

//class Texture2D;

class MeshComponent :public RenderComponent
{
public:
	MeshComponent(GameObject* gameObject,Renderer* renderer);
	MeshComponent(MeshComponent& other);
	MeshComponent(MeshComponent&& other);
	~MeshComponent();

	void setMesh(const std::string& name, MeshLoader* meshLoader);
	void setTexture(const std::string& name, TextureLoader* meshLoader);

	void Render() override;
protected:
	Mesh* mesh;
	Texture2D* texture;

};