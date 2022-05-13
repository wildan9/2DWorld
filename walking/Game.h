/* by Wildan R. */

#pragma once

#include "FileUtils.h"
#include "Camera2D.h"
#include "GameObject.h"
#include <string>

class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	const int _windowWidth{ 512 };
	const int _windowHeight{ 512 };
	const std::string& _windowTitle{ "Walking" };
	raylib::Camera2D _camera;
};