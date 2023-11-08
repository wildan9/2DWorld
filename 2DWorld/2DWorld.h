/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2023 Wildan Wijanarko (@wildan9)
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <string>

#include "Audio.h"
#include "VectorMath.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "GameplayScene.h"

#include "rlTiles/rlTiles.h"
#include "rlCamera2D/rlCamera2D.h"
#include "rlTiles/PUGIXML/pugixml.hpp"

#define RAYGUI_IMPLEMENTATION
#include "extras/raygui.h"

static const int screenWidth  = 512;
static const int screenHeight = 512;

class Engine
{
public:
	void Start();
	void Update();
	void Render();
	void ShutDown();

private:
	std::shared_ptr<GameplayScene> _gameplayScene = nullptr;
	std::atomic<bool> _collisionThreadRunning = 0;
	std::atomic<bool> _isEngineShutDown = 0;
	std::vector<std::thread> _threads = {};
	std::string _currentBGM = {};
	Audio _audio = {};
};