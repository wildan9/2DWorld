/* by Wildan R. */

#include "Player.h"

// ---------------- Public Functions ------------------------------------------

Player::Player(Vector2D pos)
{
	_texturePos = pos;
}

Player::~Player()
{
	UnloadTexture(_texture);
	UnloadTexture(_textureIdle);
	UnloadTexture(_textureWalk);
	UnloadTexture(_texturePunch);
	UnloadSound(_landStep);
	UnloadSound(_waterStep);
}

Vector2D Player::GetPosition() const
{
	return _texturePos;
}

void Player::SetPosition(Vector2D pos)
{
	_texturePos = pos;
}

Rectangle Player::GetCollision() const
{
	return Rectangle{
		_texturePos.x, _texturePos.y,
		1.5f * (float)_texture.width / row(),
		1.5f * (float)_texture.height
	};
}

float Player::GetFacing() const
{
	return _facing;
}

bool Player::IsPunch() const
{
	if (IsKeyDown(KEY_E) && !_isWalk) return 1;
	else return 0;
}

void Player::Stop()
{
	_texturePos = _textureLastPos;
}

void Player::OnLand()
{
	if (speed() == 2.0f) _timer += frame_time() * 0.23f;
	else _timer += frame_time() * 0.33f;

	if (_isWalk && _timer >= _updateTime)
	{
		_timer = 0.0f;
		PlaySound(_landStep);
	}
}

void Player::OnWater()
{
	if (speed() == 2.0f) _timer += frame_time() * 0.11f;
	else _timer += frame_time() * 0.13f;

	if (_isWalk && _timer >= _updateTime)
	{
		_timer = 0.0f;
		PlaySound(_waterStep);
	}
}

void Player::Draw()
{
	_textureLastPos = _texturePos;

	if (direction().Length() != 0.0f)
	{
		_isWalk = 1;
		_texturePos = _texturePos.Subtract(direction().Normalize().Scale(speed()));
		_texture = _textureWalk;
		if (direction().x < 0.0f) _facing = 1.0f;
		if (direction().x > 0.0f) _facing = -1.0f;
	}
	else
	{
		_isWalk = 0;
		_texture = _textureIdle;
	}

	if (IsPunch()) _texture = _texturePunch;

	Animate(_texturePos, _texture, frame_time(), 1.5f, row(), _facing, timer());
}

// ---------------- Private Functions ------------------------------------------

Vector2D Player::direction() const
{
	Vector2D direction{};

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction.x -= 1.0f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) direction.x += 1.0f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) direction.y -= 1.0f;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) direction.y += 1.0f;

	return direction;
}

float Player::row() const
{
	if (IsPunch() && !_isWalk) return 3.0f;
	else if (!_isWalk) return 2.0f;
	else if (_isWalk) return 6.0f;
}

float Player::speed() const
{
	if (IsKeyDown(KEY_SPACE)) return 4.0f; // walk fast
	else return 2.0f; // walk slow
}

float Player::timer() const
{
	if (_isWalk) return 0.0f;
	else if (IsPunch() && !_isWalk) return 0.02f;
	else return -0.2f;
}

float Player::frame_time() const
{
	return GetFrameTime();
}
