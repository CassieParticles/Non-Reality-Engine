#include "ShaderManager.h"
#include <cassert>

bool ShaderManager::instantiated = false;

ShaderManager::ShaderManager(const ComPtr<ID3D11Device>& device)
{
	//Prevent multiple instances being created
	assert(!instantiated);
	instantiated = true;

	this->device = device;
}

ShaderManager::ShaderManager(ShaderManager&& other)
{
	vertexShaders = std::move(other.vertexShaders);
	pixelShaders = std::move(other.pixelShaders);
}

VertexShader* ShaderManager::getVertexShader(const std::wstring& filePath)
{
	int key = std::hash<std::wstring>{}(filePath);

	auto val = vertexShaders.find(key);
	if (val == vertexShaders.end())
	{
		//Shader not found, creating
		std::cout << "Not found\n";
		vertexShaders.emplace(key, std::make_unique<VertexShader>(device.Get(),filePath));
		val = vertexShaders.find(key);
	}

	return val->second.get();
}

PixelShader* ShaderManager::getPixelShader(const std::wstring& filePath)
{
	int key = std::hash<std::wstring>{}(filePath);

	auto val = pixelShaders.find(key);
	if (val == pixelShaders.end())
	{
		//Shader not found, creating
		std::cout << "Not found\n";
		pixelShaders.emplace(key, std::make_unique<PixelShader>(device.Get(), filePath));
		val = pixelShaders.find(key);
	}

	return val->second.get();
}
