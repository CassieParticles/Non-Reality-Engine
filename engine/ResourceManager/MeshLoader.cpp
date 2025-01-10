#include "MeshLoader.h"
#include "MeshLoader.h"
#include "MeshLoader.h"

#include <iostream>
#include <DirectXMath.h>

MeshLoader::MeshLoader(Microsoft::WRL::ComPtr<ID3D11Device> device):device{device}
{
	addPlaneMesh();
	addCubeMesh();
	addQuadMesh();
	addSphereMesh();
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

void MeshLoader::addQuadMesh()
{
	Mesh::MeshVertex quadVertices[4]
	{
		{{-0.5f, 0.5f,0},{0,0,-1},{0,0}},	//TL
		{{ 0.5f, 0.5f,0},{0,0,-1},{1,0}},	//TR
		{{ 0.5f,-0.5f,0},{0,0,-1},{1,1}},	//BR
		{{-0.5f,-0.5f,0},{0,0,-1},{0,1}},	//BL
	};

	int indices[6] =
	{
		0,1,2,
		0,2,3
	};

	addMesh("Quad", quadVertices, 4, indices, 6, false);
}

//Thank you to https://danielsieger.com/blog/2021/03/27/generating-spheres.html for this
void MeshLoader::addSphereMesh()
{
	std::vector<Mesh::MeshVertex> vertices;
	std::vector<int> indices;

	constexpr int numSlices = 30;
	constexpr int numStacks = 30;

	constexpr float invSlice = 1.0f / numSlices;
	constexpr float invStack = 1.0f / numStacks;

	//Assemble vertex array

	//Add top vertices
	vertices.push_back({ DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT2{ 0,0 } });

	//Add vertices within sphere
	for (int stack = 0; stack < numStacks-1; ++stack)
	{
		float phi = 3.14159f * (stack + 1) / numStacks;
		for (int slice = 0; slice < numSlices + 1; ++slice)
		{
			float theta = 2 * 3.14159 * slice / numSlices;
			Mesh::MeshVertex vertex{};
			vertex.position = DirectX::XMFLOAT3{ sin(phi) * cos(theta),cos(phi),sin(phi) * sin(theta) };
			vertex.normal = vertex.position;
			vertex.uv = DirectX::XMFLOAT2{slice * invSlice, stack * invStack};

			vertices.push_back(vertex);
		}
	}

	//Add bottom vertex
	vertices.push_back({ DirectX::XMFLOAT3{ 0,-1,0 }, DirectX::XMFLOAT3{ 0,-1,0 }, DirectX::XMFLOAT2{ 0,1 } });

	//Assemble index array


	for (int i = 0; i < numSlices; ++i)
	{
		//Top triangle
		indices.push_back(0);
		indices.push_back((i + 1) % numSlices + 1);
		indices.push_back(i + 1);

		//Bottom triangle
		indices.push_back(vertices.size() - 1);
		indices.push_back(i + numSlices * (numStacks - 2) + 1);
		indices.push_back((i + 1) % numSlices + numSlices * (numStacks - 2) + 1);
	}

	for (int stack = 0; stack < numStacks - 2; ++stack)
	{
		int stack0 = stack * numSlices + 1;
		int stack1 = (stack + 1) * numSlices + 1;
		for (int slice = 0; slice < numSlices; ++slice)
		{
			//Get the 4 corners of the quad
			int slice0 = stack0 + slice;
			int slice1 = stack0 + (slice + 1) % numSlices;
			int slice2 = stack1 + (slice + 1) % numSlices;
			int slice3 = stack1 + slice;

			//Triangle 1
			indices.push_back(slice0);
			indices.push_back(slice1);
			indices.push_back(slice2);

			//Triangle 2
			indices.push_back(slice0);
			indices.push_back(slice2);
			indices.push_back(slice3);
		}
	}

	addMesh("Sphere", vertices.data(), vertices.size(), indices.data(), indices.size(), false);
}
