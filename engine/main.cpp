#include <iostream>

#include <engine/Window.h>
#include <engine/Shader.h>

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>("Test window", 800, 600);

	Microsoft::WRL::ComPtr<ID3D11Device> device = window->getDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = window->getDeviceContext();



	VertexShader vs{ device.Get(),L"shaders/vertex.hlsl" };

	while (!window->getWindowShouldClose())
	{
		window->clearBackBuffer();

		window->presentBackBuffer();
	}

	return 0;
}