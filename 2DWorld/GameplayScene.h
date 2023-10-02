/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023 Wildan Wijanarko (@wildan9)
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
#include "Scene.h"

#include "Player.h"
#include "GameObject.h"
#include "rlTiles/rlTiles.h"
#include "rlCamera2D/rlCamera2D.h"
#include "rlTiles/PUGIXML/pugixml.hpp"

#include "Prop.cpp"
#include "Animal.cpp"

class GameplayScene : public Scene
{
public:
	void Start() override;
	void Update() override;
	void LoadResources() override;
	void FreeResources() override;
	void Draw() override;

	std::vector<std::shared_ptr<GameObject>> gameObjectsVec;

	inline std::shared_ptr<Player> GetPlayer() const
	{
		assert(_player != nullptr);

		return _player;
	}

private:
	RLTileMap _tileMap = {};
	RLCamera2D _camera = {};
	std::shared_ptr<Player>  _player = nullptr;
	std::shared_ptr<Animals> _animals = nullptr;
	std::unique_ptr<RLTileRenderer> _rendererMap = {};
};