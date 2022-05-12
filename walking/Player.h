/* by Wildan R. */

#pragma once

#include "Animation.h"

class Player : public BaseAnimation
{
public:
	Player(Vector2D& pos);
	~Player();
	Vector2D GetPosition() const;
	void SetPosition(Vector2D& pos);
	Rectangle GetCollision() const;
	float GetFacing() const;
	bool IsPunch() const;
	void Stop();
	void OnLand();
	void OnWater();
	void Draw();

private:
	bool _isWalk{ 0 };
	float _timer{};
	float _facing{ 1.0f };
	float row() const;
	float speed() const;
	float timer() const;
	float frame_time() const;
	const float _updateTime{ 0.0834f };
	Vector2D _texturePos{};
	Vector2D _textureLastPos{};
	Vector2D direction() const;
	Sound _landStep{ LoadSound("sounds/land_step.wav") };
	Sound _waterStep{ LoadSound("sounds/water_step.wav") };
	Texture2D _texture{ LoadTexture("textures/character/friendly_man_idle.png") };
	Texture2D _textureIdle{ LoadTexture("textures/character/friendly_man_idle.png") };
	Texture2D _texturePunch{ LoadTexture("textures/character/friendly_man_punch.png") };
	Texture2D _textureWalk{ LoadTexture("textures/character/friendly_man_walk.png") };
};
