/* by Wildan R. */

#pragma once

#include "Header.h"
#include "Camera2D.h"
#include "Player.h"
#include "Map.h"
#include "Animal.cpp"
#include "Prop.cpp"
#include <string>

class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	const int m_window_width{ 512 };
	const int m_window_height{ 512 };
	const std::string& m_window_title{ "Walking" };
	raylib::Camera2D m_camera;
};