#include "TextureLoader.h"
#include "TextureLoader.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureLoader::TextureLoader(ComPtr<ID3D11Device> device):device{device}
{

}

TextureLoader::TextureLoader(TextureLoader&& other)
{
	device = std::move(other.device);
	textures = std::move(other.textures);
}

TextureLoader::~TextureLoader()
{
}

Texture2D* TextureLoader::addColour(const std::string& name, float red, float green, float blue)
{
	//Check if colour already exists
	int key = std::hash<std::string>{}(name);

	auto val = textures.find(key);

	if (val != textures.end())
	{
		std::cerr << "Error: Texture already exists\n";
		return nullptr;
	}

	float cData[4] = { red,green,blue,1.0f };

	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc = { 1,0 };
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	textures[key] = std::make_unique<Texture2D>(device.Get(), desc, cData, sizeof(float) * 4);

	return textures[key].get();
}

Texture2D* TextureLoader::loadTextureFromFile(const std::string& name, const std::string& filePath)
{
	//Get hash of name, for key
	int key = std::hash<std::string>{}(name);

	//Check if texture already exists
	auto val = textures.find(key);

	if (val != textures.end())
	{
		std::cerr << "Error: Texture already exists\n";
		return nullptr;
	}

	//Get texture data
	int width, height;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, nullptr, 4);
	if (!data)
	{
		std::cerr << "Error: Could not find texture path specified\n";
		return nullptr;
	}
	
	//Create texture2D from texture data
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc = { 1,0 };
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	textures[key] = std::make_unique<Texture2D>(device.Get(), desc, data, sizeof(char) * 4);

	stbi_image_free(data);

	return textures[key].get();
}

Texture2D* TextureLoader::loadTextureFromData(const std::string& name, D3D11_TEXTURE2D_DESC desc, void* initialData, int typeWidth)
{
	//Get hash of name, for key
	int key = std::hash<std::string>{}(name);

	//Check if texture already exists
	auto val = textures.find(key);

	if (val != textures.end())
	{
		std::cerr << "Error: Texture already exists\n";
		return nullptr;
	}

	textures[key] = std::make_unique<Texture2D>(device.Get(), desc, initialData, typeWidth);

	return textures[key].get();
}

Texture2D* TextureLoader::getTexture(const std::string& name)
{
	int key = std::hash <std::string>{}(name);

	auto val = textures.find(key);
	if (val == textures.end())
	{
		std::cerr << "Error, could not find texture\n";
		return nullptr;
	}

	return val->second.get();
}


