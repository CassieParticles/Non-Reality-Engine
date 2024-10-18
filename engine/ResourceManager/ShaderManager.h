#pragma once

#include <unordered_map>
#include <string>

#include <engine/Shader.h>

class ShaderManager
{
public:
	ShaderManager(const ComPtr<ID3D11Device>& device);
	ShaderManager(ShaderManager& other) = delete;	//Disable copy constructor, should only be one manager
	ShaderManager(ShaderManager&& other);


	VertexShader* getVertexShader(const std::wstring& filePath);
	PixelShader* getPixelShader(const std::wstring& filePath);
protected:
	ComPtr<ID3D11Device> device;

	static bool instantiated;
	//Shaders are stored in unique ptrs to ensure ownership, and passed out as raw ptrs

	//hash of filepath, shader wrapper
	std::unordered_map<int, std::unique_ptr<VertexShader>> vertexShaders;
	std::unordered_map<int, std::unique_ptr<PixelShader>> pixelShaders;
	//TODO: 
};