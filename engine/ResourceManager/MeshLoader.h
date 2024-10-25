#pragma once
#include <graphicsEngine/Pipeline/Mesh.h>

#include <memory>
#include <string>
#include <unordered_map>

class MeshLoader
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	MeshLoader(Microsoft::WRL::ComPtr<ID3D11Device> device);
	MeshLoader(MeshLoader& other) = delete;	//Only a single mesh loader should exist
	MeshLoader(MeshLoader&& other);
	~MeshLoader();

	//Add mesh given certain vertices
	Mesh* addMesh(const std::string& name, Mesh::MeshVertex* meshVertices, int vertexCount, int* indices, int indexCount, bool dynamic);
	//Load obj given file path
	//TODO: Add .obj loading to asset loader

	Mesh* getMesh(const std::string& name);

protected:
	ComPtr<ID3D11Device> device;
	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;

	void addPlaneMesh();

	/*Types of base mesh
	*	-Plane - Done
	*	-Cube
	*	-Sphere
	*	-Disc
	*	-Cylinder
	*	-Capsule
	*	-Cone
	*/
	//Basic meshes
};