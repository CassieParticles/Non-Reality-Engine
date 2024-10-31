#pragma once

#include <memory>

class Window;
class ShaderManager;
class MeshLoader;

class GameLoop
{
public:
	GameLoop();
	~GameLoop();
protected:
	//Systems
	std::unique_ptr<Window> window;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<MeshLoader> MeshLoader;
};