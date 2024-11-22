#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <wrl.h>
#include <DirectXMath.h>
#include <d3d11.h>

#include <graphicsEngine/Pipeline/Texture2D.h>

class TextureLoader
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	TextureLoader(ComPtr<ID3D11Device> device);
	TextureLoader(TextureLoader& other) = delete;
	TextureLoader(TextureLoader&& other);
	~TextureLoader();

	//Add a texture based on existing data
	Texture2D* addColour(const std::string& name, float red, float green, float blue);

	//Load a texture from provided file path
	Texture2D* loadTextureFromFile(const std::string& name, const std::string& filePath);

	Texture2D* getColour(const std::string& name);
protected:
	ComPtr<ID3D11Device> device;
	std::unordered_map<int, std::unique_ptr<Texture2D>> textures;
};