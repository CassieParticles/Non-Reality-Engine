#include <iostream>

#include <engine/Window.h>

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>("Test window", 800, 600);

	while (!window->getWindowShouldClose())
	{
		window->clearBackBuffer();

		window->presentBackBuffer();
	}

	return 0;
}