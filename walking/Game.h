/* by Wildan R. */

#pragma once

#include "CoreLib.h"
#include "Camera2D.h"
#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	const int _windowWidth{ 512 };
	const int _windowHeight{ 512 };
	raylib::Camera2D _camera;
};

enum class GameScreen
{
	LOGO = 0, TITLE, GAMEPLAY
};