/* by Wildan R. */

#include "Animation.h"
#include <array>

class Rhino : public BaseAnimation
{
public:
	Rhino(Vector2D& pos)
	{
		m_texture_pos = pos;
	}

	~Rhino()
	{
		UnloadTexture(m_texture);
	}

	void Draw(const float& delta_time)
	{
		Animate(m_texture_pos, m_texture, delta_time, 2.0f, 8.0f);
	}

	Rectangle GetRectColl()
	{
		return Rectangle{
			static_cast<float>(m_texture_pos.x + 12.0f),
			static_cast<float>(m_texture_pos.y + 25.0f),
			static_cast<float>(2.0f * m_texture.width / 8.0f - 30.0f),
			static_cast<float>(2.0f * m_texture.height - 25.0f)
		};
	}

private:
	Texture2D m_texture{ LoadTexture("textures/animals/rhino_idle.png") };
	Vector2D m_texture_pos{};
};


class Bat : public BaseAnimation
{
public:
	Bat(Vector2D& pos)
	{
		m_texture_pos = pos;
	}

	~Bat()
	{
		UnloadTexture(m_texture);
	}

	void Draw(const float& delta_time, const float& fly_radius)
	{
		if (true)
		{
			m_texture_pos.x += m_speed.x;
			m_texture_pos.y += m_speed.y;

			if (m_texture_pos.x >= fly_radius || m_texture_pos.x <= 0)
			{
				m_speed.x *= -1.0f;
				m_facing *= -1.0f;
			}
			if (m_texture_pos.y >= fly_radius || m_texture_pos.y <= 0) m_speed.y *= -1.0f;
		}

		Animate(m_texture_pos, m_texture, delta_time, 2.0f, 6.0f);
	}

private:
	Texture2D m_texture{ LoadTexture("textures/animals/bat_fly.png") };
	Vector2D m_speed{ 1.5f, 1.0f };
	Vector2D m_texture_pos{};
	float m_facing{ 1.0f };
};


class Chicken : public BaseAnimation
{
public:
	Chicken()
	{

	}

	~Chicken()
	{
		UnloadTexture(m_texture);
	}

	void Draw(const float& delta_time)
	{
		int x = 1, y = 0;

		if (m_texture_pos.x == 320.0f)
		{
			x = 0;
			y = 1;
		}
		if (m_texture_pos.y == 300.0f)
		{
			x = 2;
			y = 0;
		}
		if (m_texture_pos.x < 0.0f)
		{
			x = 1;
			y = 2;
		}

		switch (x)
		{
		case 1:
			m_texture_pos.x += m_speed;
			m_facing = -1.0f;
			break;
		case 2:
			m_texture_pos.x -= m_speed;
			m_facing = 1.0f;
			break;
		default:
			break;
		}

		switch (y)
		{
		case 1:
			m_texture_pos.y += m_speed;
			break;
		case 2:
			m_texture_pos.y -= m_speed;
			break;
		default:
			break;
		}

		if (m_texture_pos.y != 0.0f) m_animate = 1;

		Animate(m_texture_pos, m_texture, delta_time, 1.2f, 7.0f, m_facing, 0.0f, m_animate);
	}

private:
	const float m_speed{ 1.0f };
	float m_facing{ -1.0f };
	bool m_animate{ 0 };
	Texture2D m_texture{ LoadTexture("textures/animals/chicken_walk.png") };
	Vector2D m_texture_pos{};
};


class Crocodile : public BaseAnimation
{
public:
	Crocodile()
	{

	}

	~Crocodile()
	{
		UnloadTexture(m_texture);
		UnloadTexture(m_texture_walk);
		UnloadTexture(m_texture_hurt);
		UnloadSound(m_getting_punched);
	}

	Vector2D GetPosition()
	{
		return m_texture_pos;
	}

	float GetFacing()
	{
		return m_facing;
	}

	void Walk()
	{
		m_row = 12.0f;
		m_texture = m_texture_walk;
		m_is_walk = 1;
	}

	void Hurt()
	{
		m_row = 1.0f;
		m_texture = m_texture_hurt;
		m_is_walk = 0;

		m_timer += GetFrameTime();
		if (m_timer >= m_update_time)
		{
			m_timer = -0.32f;
			PlaySound(m_getting_punched);
		}
	}

	Rectangle GetCollision()
	{
		return Rectangle{
			m_texture_pos.x, m_texture_pos.y,
			1.2f * (float)m_texture.width / m_row,
			1.2f * (float)m_texture.height
		};
	}

	void Draw(const float& delta_time)
	{
		if (m_is_walk)
		{
			m_texture_pos.x += m_speed;

			if (m_texture_pos.x >= 1400.0f || m_texture_pos.x <= 0)
			{
				m_speed *= -1.0f;
				m_facing *= -1.0f;
			}
		}

		Animate(m_texture_pos, m_texture, delta_time, 2.0f, m_row, m_facing, 0.0f, 1);
	}

private:
	float m_speed{ 1.0f };
	float m_row{};
	float m_timer{};
	float m_facing{ 1.0f };
	float m_update_time{ 0.0834f };
	bool m_is_walk{};
	Vector2D m_texture_pos{};
	Texture2D m_texture{ LoadTexture("textures/animals/crocodile_walk.png") };
	Texture2D m_texture_walk{ LoadTexture("textures/animals/crocodile_walk.png") };
	Texture2D m_texture_hurt{ LoadTexture("textures/animals/crocodile_hurt.png") };
	Sound m_getting_punched{ LoadSound("sounds/getting_punched.wav") };
};


class Animals
{
public:
	Rhino rhino1{ Vector2D{ 800.0f, 120.0f } };
	Rhino rhino2{ Vector2D{ 950.0f, 80.0f } };
	Rhino rhino3{ Vector2D{ 650.0f, 100.0f } };

	Bat bat1{ Vector2D{ 111.0f, 111.0f } };
	Bat bat2{ Vector2D{ 212.0f, 212.0f } };
	Bat bat3{ Vector2D{ 313.0f, 313.0f } };

	Crocodile crocodile;

	Chicken chicken;

	std::array<Rhino, 3> rhinos{ rhino1, rhino2, rhino3 };

	std::array<Bat, 3> bats{ bat1, bat2, bat3 };

	void SetBatFlyRadius(const float& bat_fly_radius)
	{
		m_bat_fly_radius = bat_fly_radius;
	}

	void Draw(const float& delta_time)
	{
		for (auto& rhino : rhinos) rhino.Draw(delta_time);

		for (auto& bat : bats) bat.Draw(delta_time, m_bat_fly_radius);

		crocodile.Draw(delta_time);

		chicken.Draw(delta_time);
	}

private:
	float m_bat_fly_radius{};
};