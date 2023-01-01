/* by Wildan R. */

#include "Player.h"

// ---------------- Public Functions ------------------------------------------

Player::Player() 
	: _texturePos{ 30.0f, 120.0f }
	, _textures{ std::make_unique<std::vector<Texture2D>>() }
	, _landStep{ LoadSound("sounds/land_step.wav") }
	, _waterStep{ LoadSound("sounds/water_step.wav") }
	, _horseStep{ LoadSound("sounds/horse_step.wav") }
	, _isWalk{ 0 }
	, _isDragonInside{ 0 }
	, _isRidingHorse{ 0 }
	, _timer{}
	, _facing{ 1.0f }
	, _stamina{ 6.0f }
{
	_textures->push_back(Texture2D{});

	LoadTextureFile("textures/character/friendly_man_idle.png");
	LoadTextureFile("textures/character/friendly_man_punch.png");
	LoadTextureFile("textures/character/friendly_man_walk.png");
	LoadTextureFile("textures/character/friendly_man_riding_horse_idle.png");
	LoadTextureFile("textures/character/friendly_man_riding_horse.png");
}

Player::~Player()
{
	for (const auto& texture : *_textures)
	{
		UnloadTexture(texture);
	}

	_textures->clear();

	UnloadSound(_landStep);
	UnloadSound(_waterStep);
	UnloadSound(_horseStep);
}

Vector2D Player::GetPosition() const
{
	return _texturePos;
}

Vector2D Player::GetDirection() const
{
	Vector2D direction{};

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction.x -= 1.0f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) direction.x += 1.0f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) direction.y -= 1.0f;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) direction.y += 1.0f;

	return direction;
}

Rectangle Player::GetRectangle() const
{
	const Texture2D& texture = _textures->at(0);

	return Rectangle{
		_texturePos.x, _texturePos.y,
		1.5f * (float)texture.width / Row(),
		1.5f * (float)texture.height
	};
}

float Player::GetFacing() const
{
	return _facing;
}

float Player::GetStamina() const
{
	if (_isDragonInside) return 9.0f;
	else return _stamina;
}

float Player::GetSpeed() const
{
	if (_isRidingHorse && IsKeyDown(KEY_SPACE) && GetDirection().Length() != 0 && _stamina > 0) return 6.5f;
	if (IsKeyDown(KEY_SPACE) && GetDirection().Length() != 0 && _stamina > 0) return 4.0f; // walk fast
	else return 2.0f; // walk slow
}

bool Player::IsPunch() const
{
	if (IsKeyDown(KEY_E) && !_isWalk) return 1;
	else return 0;
}

bool Player::IsInvisible() const
{
	if (IsKeyDown(KEY_LEFT_SHIFT)) return 1;
	else return 0;
}

void Player::SetPosition(const Vector2D pos)
{
	_texturePos = pos;
}

void Player::SetStamina(bool isDragonInside)
{
	_stamina = 6.0f;
	_isDragonInside = isDragonInside;
}

void Player::Stop()
{
	_texturePos = _textureLastPos;
}

void Player::OnLand()
{
	if (_isRidingHorse)
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

void Player::OnHorse(bool isRidingHorse)
{
	_isRidingHorse = isRidingHorse;
}

void Player::Draw()
{
	UpdateTexture();

	if (!IsKeyDown(KEY_LEFT_SHIFT))
	{
		const float size = (_isRidingHorse) ? 2.0f : 1.5f;

		Animate(_texturePos, _textures->at(0), GetFrameTime(), size, Row(), _facing, Timer());
	}
}

// ---------------- Private Functions ------------------------------------------

float Player::Row() const
{
	if (_isRidingHorse)
	{
		if (!_isWalk) return 13.0f;
		else return 6.0f;
	}

	if (IsPunch() && !_isWalk) return 3.0f;
	else if (_isWalk) return 6.0f;

	return 2.0f;
}

float Player::Timer() const
{
	if (_isRidingHorse) if (!_isWalk) return -0.09f;

	if (_isWalk) return 0.0f;
	else if (IsPunch() && !_isWalk) return 0.02f;

	return -0.2f;
}

void Player::UpdateTexture()
{
#ifdef _DEBUG
	_stamina = 6.0f;
#else
	if (!_isDragonInside)
	{
		if (IsKeyDown(KEY_SPACE) && GetDirection().Length() != 0 && _stamina > 0.0f)
		{
			_stamina -= GetFrameTime();
		}
		else if (!IsKeyDown(KEY_SPACE) && GetDirection().Length() == 0)
		{
			if (_stamina < 6.0f && _stamina > -0.1f)
			{
				_stamina += GetFrameTime();
			}
		}
	}
#endif

	_textureLastPos = _texturePos;

	if (GetDirection().Length() != 0)
	{
		_isWalk = 1;

		_texturePos = _texturePos
			.Subtract(GetDirection()
				.Normalize()
				.Scale(GetSpeed()));

		_textures->at(0) = (_isRidingHorse) ? _textures->at(5) : _textures->at(3);

		if (GetDirection().x < 0.0f) _facing = 1.0f;
		if (GetDirection().x > 0.0f) _facing = -1.0f;
	}
	else
	{
		_isWalk = 0;

		_textures->at(0) = (_isRidingHorse) ? _textures->at(4) : _textures->at(1);
	}

	if (IsPunch()) _textures->at(0) = _textures->at(2);
}

int Player::LoadTextureFile(const char* texture)
{
	_textures->push_back(LoadTexture(texture));
	return int(_textures->size() - 1);
}