/* by Wildan R. */

#pragma once

#include "Animation.h"
#include "Vec2.h"

class Player : public BaseAnimation
{
public:
	Player(Vector2& pos);
	~Player();
	Vector2 GetPosition() const;
	void SetPosition(Vector2& pos);
	Rectangle GetCollision() const;
	float GetFacing() const;
	bool IsPunch() const;
	void Stop();
	void OnLand();
	void OnWater();
	void Draw();

private:
	bool m_is_walk{ 0 };
	float m_timer{};
	float m_facing{ 1.0f };
	const float m_update_time{ 0.0834f };
	Vector2 m_texture_last_pos{};
	Vec2 m_texture_pos{};
	Vec2 direction() const;
	Texture2D m_texture{ LoadTexture("textures/character/friendly_man_idle.png") };
	Texture2D m_texture_idle{ LoadTexture("textures/character/friendly_man_idle.png") };
	Texture2D m_texture_punch{ LoadTexture("textures/character/friendly_man_punch.png") };
	Texture2D m_texture_walk{ LoadTexture("textures/character/friendly_man_walk.png") };
	Sound m_land_step{ LoadSound("sounds/land_step.wav") };
	Sound m_water_step{ LoadSound("sounds/water_step.wav") };
	float row() const;
	float speed() const;
	float timer() const;
	float frame_time() const;
};
