/* by Wildan R. */

#include <array>
#include "Animation.h"

class Rhino : public BaseAnimation
{
public:
	~Rhino()
	{
		UnloadTexture(_texture);
	}

	void SetPosition(const Vector2D& pos);

	void Draw(const float deltaTime);

	Rectangle GetCollision();

private:
	Texture2D _texture{ LoadTexture("textures/animals/rhino_idle.png") };
	Vector2D _texturePos{};
};

inline void Rhino::SetPosition(const Vector2D& pos)
{
	_texturePos = pos;
}

inline void Rhino::Draw(const float deltaTime)
{
	Animate(_texturePos, _texture, deltaTime, 2.0f, 8.0f);
}

inline Rectangle Rhino::GetCollision()
{
	return Rectangle{
		static_cast<float>(_texturePos.x + 12.0f),
		static_cast<float>(_texturePos.y + 25.0f),
		static_cast<float>(2.0f * _texture.width / 8.0f - 30.0f),
		static_cast<float>(2.0f * _texture.height - 25.0f)
	};
}

class Bat : public BaseAnimation
{
public:
	~Bat()
	{
		UnloadTexture(_texture);
	}

	void SetPosition(const Vector2D& pos);

	void SetFlyRadius(const float flyRadius);

	void Draw(const float deltaTime);

private:
	Texture2D _texture{ LoadTexture("textures/animals/bat_fly.png") };
	Vector2D _speed{ 1.5f, 1.0f };
	Vector2D _texturePos{};
	float _facing{ 1.0f };
	float _flyRadius{};
};

inline void Bat::SetFlyRadius(const float flyRadius)
{
	_flyRadius = flyRadius;
}

inline void Bat::SetPosition(const Vector2D& pos)
{
	_texturePos = pos;
}

inline void Bat::Draw(const float deltaTime)
{
	_texturePos.x += _speed.x;
	_texturePos.y += _speed.y;

	if (_texturePos.x >= _flyRadius || _texturePos.x <= 0)
	{
		_speed.x *= -1.0f;
		_facing *= -1.0f;
	}
	if (_texturePos.y >= _flyRadius || _texturePos.y <= 0) _speed.y *= -1.0f;

	Animate(_texturePos, _texture, deltaTime, 2.0f, 6.0f);
}

class Chicken : public BaseAnimation
{
public:
	~Chicken()
	{
		UnloadTexture(_texture);
	}

	void Draw(const float deltaTime);

private:
	const float _speed{ 1.0f };
	float _facing{ -1.0f };
	bool _animate{ 0 };
	Texture2D _texture{ LoadTexture("textures/animals/chicken_walk.png") };
	Vector2D _texturePos{};
};

inline void Chicken::Draw(const float deltaTime)
{
	int x = 1, y = 0;

	if (_texturePos.x == 320.0f)
	{
		x = 0;
		y = 1;
	}
	if (_texturePos.y == 300.0f)
	{
		x = 2;
		y = 0;
	}
	if (_texturePos.x < 0.0f)
	{
		x = 1;
		y = 2;
	}

	switch (x)
	{
	case 1:
		_texturePos.x += _speed;
		_facing = -1.0f;
		break;
	case 2:
		_texturePos.x -= _speed;
		_facing = 1.0f;
		break;
	default:
		break;
	}

	switch (y)
	{
	case 1:
		_texturePos.y += _speed;
		break;
	case 2:
		_texturePos.y -= _speed;
		break;
	default:
		break;
	}

	if (_texturePos.y != 0.0f) _animate = 1;

	Animate(_texturePos, _texture, deltaTime, 1.2f, 7.0f, _facing, 0.0f, _animate);
}

class Crocodile : public BaseAnimation
{
public:
	~Crocodile()
	{
		UnloadTexture(_texture);
		UnloadTexture(_textureWalk);
		UnloadTexture(_textureHurt);
		UnloadSound(_gettingPunched);
	}

	Vector2D GetPosition()
	{
		return _texturePos;
	}

	void Walk()
	{
		_row = 12.0f;
		_texture = _textureWalk;
		_isWalk = 1;
		_rotate = 0.0f;
	}

	void Hurt()
	{
		_row = 1.0f;
		_texture = _textureHurt;
		_isWalk = 0;

		_rotate = (_speed < 0.0f) ? 10.0f : 355.0f;

		_timer += GetFrameTime();
		if (_timer >= _updateTime)
		{
			_timer = -0.32f;
			PlaySound(_gettingPunched);
		}
	}

	Rectangle GetCollision()
	{
		return Rectangle{
			_texturePos.x, _texturePos.y,
			1.2f * (float)_texture.width / _row,
			1.2f * (float)_texture.height
		};
	}

	void Draw(const float deltaTime)
	{
		if (_isWalk)
		{
			_texturePos.x += _speed;

			if (_texturePos.x >= 1400.0f || _texturePos.x <= 0)
			{
				_speed *= -1.0f;
				_facing *= -1.0f;
			}
		}

		Animate(_texturePos, _texture, deltaTime, 2.0f, _row, _facing, 0.0f, 1, _rotate);
	}

private:
	float _speed{ 1.0f };
	float _row{};
	float _timer{};
	float _facing{ 1.0f };
	float _updateTime{ 0.0834f };
	float _rotate{ 0.0f };
	bool _isWalk{};
	Vector2D _texturePos{};
	Texture2D _texture{ LoadTexture("textures/animals/crocodile_walk.png") };
	Texture2D _textureWalk{ LoadTexture("textures/animals/crocodile_walk.png") };
	Texture2D _textureHurt{ LoadTexture("textures/animals/crocodile_hurt.png") };
	Sound _gettingPunched{ LoadSound("sounds/getting_punched.wav") };
};

class Animals
{
public:
	Animals()
	{
		for (auto& rhino : rhinos)
		{
			Vector2D rhinoPos{ (float)GetRandomValue(2600, 3500), (float)GetRandomValue(10, 450) };

			rhino.SetPosition(rhinoPos);
		}

		for (auto& bat : _bats)
		{
			Vector2D batPos{ (float)GetRandomValue(500, 950), (float)GetRandomValue(50, 155) };

			bat.SetPosition(batPos);

			bat.SetFlyRadius((float)GetRandomValue(1000, 1500));
		}
	}

public:
	Crocodile crocodile;
	Chicken chicken;

	std::array<Rhino, 3> rhinos{};

	void Draw(const float deltaTime);

private:
	std::array<Bat, 16> _bats{};
};

inline void Animals::Draw(const float deltaTime)
{
	for (auto& rhino : rhinos) rhino.Draw(deltaTime);

	for (auto& bat : _bats) bat.Draw(deltaTime);

	crocodile.Draw(deltaTime);

	chicken.Draw(deltaTime);
}