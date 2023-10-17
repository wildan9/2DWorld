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

#include <array>
#include <memory>

#include "GameObject.h"

bool IsBatDead();

class Bat : public GameObject
{
public:
	Bat() : _speed{ 1.5f, 1.0f }, _flyRadius{ (float)GetRandomValue(1000, 1500) }
	{
		Start();
	}

	~Bat()
	{
		_textures.Clear();
	}

	void Start() override;

	void Update() override;

	void Heal();

	void Death();

	inline bool IsDead() const
	{
		return _isDead;
	}

	bool healed = 0;

private:
	Vector2 _speed;
	float _flyRadius;
	bool _isDead = 0;
	bool _animate = 1;
	float _numFrames = 6;
	float _frameSpeed = 10;
	float _fallTimer = 0.5f;
};

class Chicken : public GameObject
{
public:
	Chicken()
	{
		Start();
	}

	~Chicken()
	{
		_textures.Clear();
	}

protected:
	void Start() override;

	void Update() override;

private:
	bool _animate;
	const float _speed = 1.0f;
};

class Crocodile : public GameObject
{
public:
	Crocodile()
	{
		Start();
	}

	~Crocodile()
	{
		_textures.Clear();
		UnloadSound(_gettingPunched);
	}

	void Walk();

	void Hurt();

public:
	void Start() override;

	void Update() override;

private:
	float _speed;
	float _numFrames;
	float _timer;
	float _rotation;
	bool _isWalk;
	Sound _gettingPunched;
	const float _updateTime = 0.0834f;
};

class Rhino : public GameObject
{
public:
	Rhino()
	{
		Start();
	}

	~Rhino()
	{
		_textures.Clear();
	}

protected:
	void Start() override;

	void Update() override;

private:
	float _numFrames = 8;
};

class Horse : public GameObject
{
public:
	Horse()
	{
		Start();
	}

	~Horse()
	{
		_textures.Clear();
	}

protected:
	void Start() override;

	void Update() override;
};

//class Cat : public GameObject
//{
//public:
//	Cat() 
//	{
//		Start();
//
//		_position = { 38.0f, 184.0f, 2.0f };
//	}
//
//	~Cat()
//	{
//		_textures.Clear();
//	}
//
//	void UpdatePosition(float dx, float dy)
//	{
//		transform.x = dx;
//		transform.y = dy;
//
//		SetPosition(transform);
//	}
//
//	std::vector<Vector2> path;
//	Vector2 transform = { 0.0f, 0.0f };
//	bool animate = 0;
//
//protected:
//	void Start() override
//	{
//		_textures.LoadTextureFile("resources/textures/animals/cat/walk.png");
//	}
//
//	void Update() override {}
//};

struct Animals
{
	Animals()
	{
		horse     = std::make_shared<Horse>();
		chicken   = std::make_shared<Chicken>();
		crocodile = std::make_shared<Crocodile>();
	}

	void Start(std::vector<std::shared_ptr<GameObject>>& gameObjectsVec);

	void Update(const float playerSpeed, Vector2 playerDirection, const float playerFacing);

	std::shared_ptr<Horse> horse;
	std::shared_ptr<Chicken> chicken;
	std::shared_ptr<Crocodile> crocodile;

	std::array<std::shared_ptr<Rhino>, 3> rhinos;
};