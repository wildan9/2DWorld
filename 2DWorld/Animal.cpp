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

#include "Animal.h"

bool isBatDead = 0;

void Bat::Start()
{
	name = "Bat";

	_position = { (float)GetRandomValue(280, 310), (float)GetRandomValue(930, 990), -1.0f };
	_facing = 1.0f;

	_textures.LoadTextureFile("resources/textures/animals/bat/fly.png");
	_textures.LoadTextureFile("resources/textures/animals/bat/death.png");
}

void Bat::Update()
{
	if (_isDead) _pCurrentTexture = &_textures[1];
	else _pCurrentTexture = &_textures[0];

	_rectangle = { _position.x, _position.y, 28.0f, 28.0f };

	_position.x += _speed.x;
	_position.y += _speed.y;

	if (_isDead && _fallTimer >= 0) _fallTimer -= GetFrameTime();

	if (_fallTimer < 0 && !healed)
	{
		if (GetCurrentFrame() == _numFrames - 1)
		{
			_animate = 0;
		}
	}
	else if (healed)
	{
		_animate = 1;
		_isDead  = 0;
	}

	isBatDead = _isDead;

	if (_position.x >= _flyRadius || _position.x <= 0)
	{
		_speed.x *= -1.0f;
		_facing *= -1.0f;
	}
	if (_position.y >= _flyRadius || _position.y <= 0) _speed.y *= -1.0f;

	Animate(_frameSpeed, _numFrames, 2.0f, _animate);
}

void Bat::Heal()
{
	_speed = { 1.5f, 1.0f };
	_isDead = 0;
	_numFrames = 6;
	_frameSpeed = 10;
	_facing *= -1.0f;

	healed = 1;
}

void Bat::Death()
{
	_speed = { 0.0f, 0.0f };
	_isDead = 1;
	_numFrames = 5;
	_frameSpeed = 6;
}

bool IsBatDead()
{
	return isBatDead;
}

void Chicken::Start()
{
	name = "Chicken";

	_facing = -1.0f;
	_animate = 0;
	_position.z = 0.0f;
	_rotation = 0.0f;

	_textures.LoadTextureFile("resources/textures/animals/chicken/walk.png");

	_pCurrentTexture = &_textures[0];

	_rectangle = { _position.x, _position.y, 35.0f, 35.0f };
}

void Chicken::Update()
{
	Animate(12, 7, 1.0f, _animate);

	int x = 1, y = 0;

	if (_position.x == 545.0f)
	{
		x = 0;
		y = 1;
	}
	if (_position.y == 372.0f)
	{
		x = 2;
		y = 0;
	}
	if (_position.x < 0.0f)
	{
		x = 1;
		y = 2;
	}

	switch (x)
	{
	case 1:
		_position.x += _speed;
		_facing = -1.0f;
		break;
	case 2:
		_position.x -= _speed;
		_facing = 1.0f;
		break;
	default:
		break;
	}

	switch (y)
	{
	case 1:
		_position.y += _speed;
		break;
	case 2:
		_position.y -= _speed;
		break;
	default:
		break;
	}

	if (_position.y != 0.0f) _animate = 1;

	_rectangle = { _position.x, _position.y, 35.0f, 35.0f };
}

void Crocodile::Walk()
{
	_textures[0] = _textures[1];

	_isWalk = 1;
	_rotation  = 0.0f;
	_numFrames = 12.0f;
}

void Crocodile::Hurt()
{
	_numFrames = 1.0f;
	_textures[0] = _textures[2];
	_isWalk = 0;

	_rotation = (_speed < 0.0f) ? 10.0f : 355.0f;

	_timer += GetFrameTime();
	if (_timer >= _updateTime)
	{
		_timer = -0.32f;
		PlaySound(_gettingPunched);
	}
}

void Crocodile::Start()
{
	_facing = 1.0f;
	_speed = 1.0f;
	_position.z = 1.0f;

	_numFrames = 0;
	_timer = 0;
	_rotation = 0;
	_isWalk = 0;

	_gettingPunched = LoadSound("resources/sounds/getting_punched.wav");

	_textures.push_back(Texture2D());
	_textures.LoadTextureFile("resources/textures/animals/crocodile/walk.png");
	_textures.LoadTextureFile("resources/textures/animals/crocodile/hurt.png");

	_pCurrentTexture = &_textures[0];

	name = "Crocodile";
}

void Crocodile::Update()
{
	if (_isWalk)
	{
		_position.x += _speed;

		if (_position.x >= 1100.0f || _position.x <= 0)
		{
			_speed *= -1.0f;
			_facing *= -1.0f;
		}
	}

	_rectangle = {
		_position.x + 4.0f, _position.y + 15.0f,
		1.2f * (float)_pCurrentTexture->width / _numFrames,
		1.2f * (float)_pCurrentTexture->height
	};

	Animate(12, _numFrames, 1.8f);
}

void Rhino::Start()
{
	name = "Rhino";

	_textures.LoadTextureFile("resources/textures/animals/rhino/idle.png");
	_textures.LoadTextureFile("resources/textures/animals/rhino/walk.png");

	_position = { 220.0f, 500.0f, 3.0f };

	_rectangle = { _position.x, _position.y, 100.0f, 70.0f };
}

void Rhino::Update()
{
	if (isOnTriger)
	{
		_numFrames = 6;
		_pCurrentTexture = &_textures[1];
	}
	else
	{
		_numFrames = 8;
		_pCurrentTexture = &_textures[0];
	}

	_rectangle = { _position.x, _position.y, 100.0f, 70.0f };

	Animate(10, _numFrames, 2.0f);
}

void Horse::Start()
{
	name = "Horse";

	_textures.LoadTextureFile("resources/textures/animals/horse/idle.png");

	_position = { 240.0f, 400.0f, 3.0f };
	_facing = 1.0f;

	_pCurrentTexture = &_textures[0];

	_rectangle = { _position.x, _position.y, 80.0f, 60.0f };
}

void Horse::Update()
{
	Animate(6, 13, 1.5f);
}

void Animals::Start(std::vector<std::shared_ptr<GameObject>>& gameObjectsVec)
{
	for (auto& rhino : rhinos) rhino = std::make_shared<Rhino>();

	rhinos[0]->SetPosition({ 24.0f,  105.0f });
	rhinos[1]->SetPosition({ 168.0f, 143.0f });
	rhinos[2]->SetPosition({ 67.0f,  263.0f });

	rhinos[1]->SetFacing(rhinos[1]->GetFacing() * -1.0f);

	for (auto& rhino : rhinos)
	{
		gameObjectsVec.push_back(rhino);
	}
}

void Animals::Update(const float playerSpeed, Vector2 playerDirection, const float playerFacing)
{
	for (auto& rhino : rhinos)
	{
		if (rhino->isOnTriger)
		{
			// Check if playerDirection is not zero before using it
			if (Vector2Length(playerDirection) > 0.0f)
			{
				rhino->SetPosition(rhino->GetPosition() - Vector2Scale(Vector2Normalize(playerDirection), playerSpeed));
				rhino->SetFacing(playerFacing * -1.0f);
			}
		}
	}

	if (crocodile->isOnTriger) crocodile->Hurt();
	else crocodile->Walk();
}