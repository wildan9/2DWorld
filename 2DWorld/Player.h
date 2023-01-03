/* by Wildan R. */

#pragma once

#include <memory>
#include <vector>
#include "Animation.h"

class Player : public BaseAnimation
{
public:
	Player();
	~Player();
	Vector2D GetPosition() const;
	Vector2D GetDirection() const;
	Rectangle GetRectangle() const;
	bool IsPunch() const;
	bool IsInvisible() const;
	float GetFacing() const;
	float GetStamina() const;
	float GetSpeed() const;
	void SetPosition(const Vector2D pos);
	void SetStamina(bool isDragonInside);
	void Stop();
	void OnLand();
	void OnWater();
	void OnHorse(bool isOnHorse);
	void Draw();

private:
	bool _isWalk, _isDragonInside, _isOnHorse;
	float _timer, _facing, _stamina;
	float Row() const, Timer() const;
	void UpdateTexture();
	int LoadTextureFile(const char* texture);
	const float _updateTime = 0.084f;
	Vector2D _texturePos, _textureLastPos;
	Sound _landStep, _waterStep, _horseStep;
	std::unique_ptr<std::vector<Texture2D>> _textures;
};