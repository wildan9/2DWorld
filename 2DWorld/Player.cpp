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

#include "Player.h"

// ---------------- Public Functions ------------------------------------------

float playerSize = 1.1f;

Player::Player() 
	: _landStep{ LoadSound("resources/sounds/land_step.wav") }
	, _waterStep{ LoadSound("resources/sounds/water_step.wav") }
	, _horseStep{ LoadSound("resources/sounds/horse_step.wav") }
	, _isWalk{ 0 }
	, _isDragonInside{ 0 }
	, _isOnHorse{ 0 }
	, _timer{}
	, _stamina{ 6.0f }
{
	Start();
}

Player::~Player()
{
	_textures.Clear();
	UnloadSound(_landStep);
	UnloadSound(_waterStep);
	UnloadSound(_horseStep);
}

Vector2 Player::GetDirection() const
{
	Vector2 direction = { 0 };

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction.x -= 1.0f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT )) direction.x += 1.0f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN )) direction.y -= 1.0f;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP   )) direction.y += 1.0f;

	return direction;
}

float Player::GetSpeed() const
{
	if (_isOnHorse && IsKeyDown(KEY_SPACE) && Vector2Length(GetDirection()) != 0) return 6.5f;
	if (IsKeyDown(KEY_SPACE) && Vector2Length(GetDirection()) != 0 && _stamina > 0) return 4.0f;
	else return 2.0f;
}

void Player::OnLand()
{
	if (_isOnHorse)
	{
		if (GetSpeed() == 2.0f) _timer += GetFrameTime() * 0.15f;
		else _timer += GetFrameTime() * 0.21f;

		if (_isWalk && _timer >= _updateTime)
		{
			_timer = 0.0f;
			PlaySound(_horseStep);
		}
	}
	else
	{
		if (GetSpeed() == 2.0f) _timer += GetFrameTime() * 0.23f;
		else _timer += GetFrameTime() * 0.33f;

		if (_isWalk && _timer >= _updateTime)
		{
			_timer = 0.0f;
			PlaySound(_landStep);
		}
	}
}

void Player::OnWater()
{
	if (GetSpeed() == 2.0f) _timer += GetFrameTime() * 0.11f;
	else _timer += GetFrameTime() * 0.13f;

	if (_isWalk && _timer >= _updateTime)
	{
		_timer = 0.0f;
		PlaySound(_waterStep);
	}
}

void Player::Start()
{
	_position = { 380.0f, 420.0f, 2.0f };
	_facing = -1.0f;

	_textures.push_back(Texture2D());

	_textures.LoadTextureFile("resources/textures/character/friendly_man_idle.png");
	_textures.LoadTextureFile("resources/textures/character/friendly_man_punch.png");
	_textures.LoadTextureFile("resources/textures/character/friendly_man_walk.png");
	_textures.LoadTextureFile("resources/textures/character/horse_riding/idle.png");
	_textures.LoadTextureFile("resources/textures/character/horse_riding/walk.png");

	_pCurrentTexture = &_textures[0];

	_rectangle =
	{ 
		_position.x, _position.y + 5.0f,
		playerSize * (float)_pCurrentTexture->width / NumFrames() * 1.0f * 0.90f,
		playerSize * (float)_pCurrentTexture->height * 1.0f * 0.90f
	};
}

void Player::Update()
{
#ifdef _DEBUG
	_stamina = 6.0f;
#else
	if (!_isDragonInside && !_isOnHorse)
	{
		if (IsKeyDown(KEY_SPACE) && Vector2Length(GetDirection()) != 0 && _stamina > 0.0f)
		{
			_stamina -= GetFrameTime();
		}
		else if (!IsKeyDown(KEY_SPACE) && Vector2Length(GetDirection()) == 0 && _stamina < 6.0f)
		{
			_stamina += GetFrameTime();
		}
	}
	else if (_isOnHorse && _stamina < 6.0f)
	{
		_stamina += GetFrameTime();
	}
#endif

	_lastPosition.x = _position.x;
	_lastPosition.y = _position.y;

	if (Vector2Length(GetDirection()) != 0)
	{
		_isWalk = 1;

		_position.x = _position.x - Vector2Scale(Vector2Normalize(GetDirection()), GetSpeed()).x;
		_position.y = _position.y - Vector2Scale(Vector2Normalize(GetDirection()), GetSpeed()).y;

		_textures[0] = (_isOnHorse) ? _textures[5] : _textures[3];

		if (GetDirection().x < 0.0f) _facing = 1.0f;
		if (GetDirection().x > 0.0f) _facing = -1.0f;
	}
	else
	{
		_isWalk = 0;

		_textures[0] = (_isOnHorse) ? _textures[4] : _textures[1];
	}

	if (!_isOnHorse && IsPunch()) _textures[0] = _textures[2];

	playerSize = (_isOnHorse) ? 1.4f : 1.1f;

	const float recSize = (_isOnHorse) ? 0.9f : 1.0f;

	_pCurrentTexture = &_textures[0];

	if (_isOnHorse && IsKeyDown(KEY_LEFT_CONTROL))
	{
		_rectangle = { 0 };
	}
	else
	{
		_rectangle =
		{ 
			_position.x, _position.y + 5.0f,
			playerSize * (float)_pCurrentTexture->width / NumFrames() * recSize * 0.90f,
			playerSize * (float)_pCurrentTexture->height * recSize * 0.90f
		};
	}

	const bool animate = (_isOnHorse && IsKeyDown(KEY_LEFT_CONTROL)) ? 0 : 1;

	_rotation = (!animate) ? ((Vector2Length(GetDirection()) != 0) ? ((GetFacing() != -1.0f) ? ((GetSpeed() > 5) ? -45.0f : -25.0f) : (GetSpeed() > 5) ? 45.0f : 25.0f) : 0.0f) : 0.0f;

	Animate(FrameSpeed(), NumFrames(), playerSize, animate);
}

// ---------------- Private Functions ------------------------------------------

int Player::FrameSpeed() const
{
	if (_isOnHorse && !_isWalk) return 6;

	if (_isWalk) return 12;
	else if (IsPunch() && !_isWalk) return 16;

	return 4;
}

float Player::NumFrames() const
{
	if (_isOnHorse)
	{
		if (!_isWalk) return 13.0f;
		else return 6.0f;
	}

	if (!_isOnHorse && IsPunch() && !_isWalk) return 3.0f;
	else if (_isWalk) return 6.0f;

	return 2.0f;
}