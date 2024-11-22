#include "MeshLoader.h"
#include "MeshLoader.h"

#include <iostream>
#include <DirectXMath.h>

MeshLoader::MeshLoader(Microsoft::WRL::ComPtr<ID3D11Device> device):device{device}
{
	addPlaneMesh();
	addCubeMesh();
}

MeshLoader::MeshLoader(MeshLoader&& other)
{
	device = std::move(device);
	meshes = std::move(meshes);
}

MeshLoader::~MeshLoader()
{
}


Mesh* MeshLoader::addMesh(const std::string& name, Mesh::MeshVertex* meshVertices, int vertexCount, int* indices, int indexCount, bool dynamic)
{
	int key = std::hash<std::string>{}(name);

	auto val = meshes.find(key);
	if (val != meshes.end())
	{
		//Name already exists within map
		std::cerr << "Error: Model already exists\n";
		return nullptr;
	}

	meshes[key] = std::make_unique<Mesh>(device.Get(), meshVertices, vertexCount, indices, indexCount, dynamic);

	return meshes[key].get();
}

Mesh* MeshLoader::getMesh(const std::string& name)
{
	int key = std::hash<std::string>{}(name);

	auto val = meshes.find(key);
	if (val == meshes.end())
	{
		std::cerr << "Error: could not find model\n";
		return nullptr;
	}

	//Return raw ptr to mesh
	return val->second.get();
}

void MeshLoader::addPlaneMesh()
{
	int gridSize = 50;
	float step = 1.0f / gridSize;
	Mesh::MeshVertex* vertices = new Mesh::MeshVertex[(gridSize + 1) * (gridSize + 1)];
	int* indices = new int[gridSize * gridSize * 6];

	//Create the vertices
	for (int i = 0; i < gridSize + 1; ++i)
	{
		for (int j = 0; j < gridSize + 1; ++j)
		{
			int index = j * (gridSize + 1) + i;
			vertices[index].position = DirectX::XMFLOAT3(j * step, 0, i * step);
			vertices[index].normal = DirectX::XMFLOAT3(0, 1, 0);
			vertices[index].uv = DirectX::XMFLOAT2(j * step, i * step);
		}
	}

	//Create the indices
	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			int index = (j * gridSize + i) * 6;
			int bottomLeft = j * (gridSize + 1) + i;

			indices[index + 0] = bottomLeft;
			indices[index + 1] = bottomLeft + 1;
			indices[index + 2] = bottomLeft + gridSize + 2;

			indices[index + 3] = bottomLeft;
			indices[index + 4] = bottomLeft + gridSize + 2;
			indices[index + 5] = bottomLeft + gridSize + 1;
		}
	}

	addMesh("Plane", vertices, (gridSize + 1) * (gridSize + 1), indices, gridSize * gridSize * 6, false);

	delete[] vertices;
}

void MeshLoader::addCubeMesh()
{
	Mesh::MeshVertex cubeVertices[6 * 4]
	{
		//+X
		{{ 0.5,-0.5,-0.5},{1,0,0},{0,1}},
		{{ 0.5, 0.5,-0.5},{1,0,0},{0,0}},
		{{ 0.5, 0.5, 0.5},{1,0,0},{1,0}},
		{{ 0.5,-0.5, 0.5},{1,0,0},{1,1}},
		//+Z
		{{ 0.5,-0.5, 0.5},{0,0,1},{0,1}},
		{{ 0.5, 0.5, 0.5},{0,0,1},{0,0}},
		{{-0.5, 0.5, 0.5},{0,0,1},{1,0}},
		{{-0.5,-0.5, 0.5},{0,0,1},{1,1}},
		//-X
		{{-0.5,-0.5, 0.5},{-1,0,0},{0,1}},
		{{-0.5, 0.5, 0.5},{-1,0,0},{0,0}},
		{{-0.5, 0.5,-0.5},{-1,0,0},{1,0}},
		{{-0.5,-0.5,-0.5},{-1,0,0},{1,1}},
		//-Z
		{{-0.5,-0.5,-0.5},{0,0,-1},{0,1}},
		{{-0.5, 0.5,-0.5},{0,0,-1},{0,0}},
		{{ 0.5, 0.5,-0.5},{0,0,-1},{1,0}},
		{{ 0.5,-0.5,-0.5},{0,0,-1},{1,1}},
		//+Y
		{{ 0.5, 0.5,-0.5},{0,1,0},{0,1}},
		{{-0.5, 0.5,-0.5},{0,1,0},{0,0}},
		{{-0.5, 0.5, 0.5},{0,1,0},{1,0}},
		{{ 0.5, 0.5, 0.5},{0,1,0},{1,1}},
		//-Y
		{{ 0.5,-0.5, 0.5},{0,-1,0},{0,1}},
		{{-0.5,-0.5, 0.5},{0,-1,0},{0,0}},
		{{-0.5,-0.5,-0.5},{0,-1,0},{1,0}},
		{{ 0.5,-0.5,-0.5},{0,-1,0},{1,1}},
	};

	int indices[6 * 6]
	{
		//+X
		0,1,2,
		0,2,3,
		//+Z
		4,5,6,
		4,6,7,
		//-X
		8,9,10,
		8,10,11,
		//-Z
		12,13,14,
		12,14,15,
		//+Y
		16,17,18,
		16,18,19,
		//-Y
		20,21,22,
		20,22,23
	};

	addMesh("Cube", cubeVertices, 6 * 4, indices, 6 * 6, false);
}
