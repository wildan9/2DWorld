/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2024 Wildan R Wijanarko
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

#include "Player.h"

static float playerSize = 0.7f;
static const float updateTime = 0.084f;
static void UpdatePlayerTrans(math::trans2d& trans, math::vec2 pos, float rot, float scl);

Player::Player() 
	: _isWalk{ 0 }
	, _isDragonInside{ 0 }
	, _isOnHorse{ 0 }
	, _stamina{ 6.0f }
{
	Start();
}

Player::~Player()
{
	UnloadModel2D(_model);
}

float Player::GetSpeed() const
{
	if (_isOnHorse && IsKeyDown(KEY_SPACE) && _dir.length() != 0) return 6.5f;
	if (IsKeyDown(KEY_SPACE) && _dir.length() != 0 && _stamina > 0) return 4.0f;
	else return 2.0f;
}

void Player::Start()
{
	const std::vector<std::string> texturesPaths
	{
		"resources/textures/character/friendly_man_idle.png",
		"resources/textures/character/friendly_man_punch.png",
		"resources/textures/character/friendly_man_walk.png",
		"resources/textures/character/horse_riding/idle.png",
		"resources/textures/character/horse_riding/walk.png"
	};

	_model = LoadModel2D(texturesPaths);

	_pos = { 380.0f, 420.0f };
	_model.facing = -1.0f;

	_model.currTexture = &_model.textures->at(0);

	_model.rec =
	{ 
		_pos.x, _pos.y + 5.0f,
		playerSize * (float)_model.currTexture->width/NumFrames() * 1.0f * 0.90f,
		playerSize * (float)_model.currTexture->height * 1.0f * 0.90f
	};

	_model.animator = std::make_unique<Animator>(CreateAnimator());
}

void Player::Update()
{
	_dir = math::vec2_input_dir();

#ifdef _DEBUG
	_stamina = 6.0f;
#else
	if (!_isDragonInside && !_isOnHorse)
	{
		if (IsKeyDown(KEY_SPACE) && dir.length() != 0 && _stamina > 0.0f)
		{
			_stamina -= GetFrameTime();
		}
		else if (!IsKeyDown(KEY_SPACE) && dir.length() == 0 && _stamina < 6.0f)
		{
			_stamina += GetFrameTime();
		}
	}
	else if (_isOnHorse && _stamina < 6.0f)
	{
		_stamina += GetFrameTime();
	}
#endif

	_lastPos = _pos;

	if (_dir.length() != 0)
	{
		_isWalk = 1;

		_pos = _pos - _dir.normalize().scale(GetSpeed());

		_model.textures->at(0) = (_isOnHorse) ? _model.textures->at(5) : _model.textures->at(3);

		if (_dir.x < 0.0f) _model.facing = 1.0f;
		if (_dir.x > 0.0f) _model.facing = -1.0f;
	}
	else
	{
		_isWalk = 0;
		_model.textures->at(0) = (_isOnHorse) ? _model.textures->at(4) : _model.textures->at(1);
	}

	if (!_isOnHorse && IsPunch()) _model.textures->at(0) = _model.textures->at(2);

	playerSize = (_isOnHorse) ? 1.4f : 0.7f;

	const float recSize = (_isOnHorse) ? 0.9f : 1.0f;

	_model.currTexture = &_model.textures->at(0);

	if (_isOnHorse && IsKeyDown(KEY_LEFT_CONTROL))
	{
		_model.rec = math::rec();
	}
	else
	{
		_model.rec =
		{ 
			_pos.x, _pos.y + 5.0f,
			playerSize * (float)_model.currTexture->width / NumFrames() * recSize * 0.90f,
			playerSize * (float)_model.currTexture->height * recSize * 0.90f
		};
	}

	bool animate = (_isOnHorse && IsKeyDown(KEY_LEFT_CONTROL)) ? 0 : 1;
	float rot = (!animate) ? (_dir.length() != 0 ? ((GetFacing() != -1.0f) ? ((GetSpeed() > 5) ? -45.0f : -25.0f) : (GetSpeed() > 5) ? 45.0f : 25.0f) : 0.0f) : 0.0f;

	UpdatePlayerTrans(_model.trans, _pos, rot, playerSize);
	UpdateAnim(_model, FrameSpeed(), NumFrames(), animate);
}

int Player::FrameSpeed() const
{
	if (_isOnHorse && !_isWalk) return 6;

	if (_isWalk) return 12;
	else if (IsPunch() && !_isWalk) return 16;

	return 4;
}

int Player::NumFrames() const
{
	if (_isOnHorse)
	{
		if (!_isWalk) return 13;
		else return 6;
	}

	if (!_isOnHorse && IsPunch() && !_isWalk) return 3;
	else if (_isWalk) return 6;

	return 2;
}

static void UpdatePlayerTrans(math::trans2d& trans, math::vec2 pos, float rot, float scl)
{
	trans.pos = pos;
	trans.rot = rot;
	trans.scl = scl;
}