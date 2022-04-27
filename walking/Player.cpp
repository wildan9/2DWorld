/* by Wildan R. */

#include "Player.h"

// ---------------- Public Functions ------------------------------------------

Player::Player(Vector2& pos)
{
	m_texture_pos = pos;
}

Player::~Player()
{
	UnloadTexture(m_texture);
	UnloadTexture(m_texture_idle);
	UnloadTexture(m_texture_walk);
	UnloadTexture(m_texture_punch);
	UnloadSound(m_land_step);
	UnloadSound(m_water_step);
}

Vector2 Player::GetPosition() const
{
	return m_texture_pos;
}

void Player::SetPosition(Vector2& pos)
{
	m_texture_pos = pos;
}

Rectangle Player::GetCollision() const
{
	return Rectangle{
		m_texture_pos.x, m_texture_pos.y,
		1.5f * (float)m_texture.width / row(),
		1.5f * (float)m_texture.height
	};
}

float Player::GetFacing() const
{
	return m_facing;
}

bool Player::IsPunch() const
{
	if (IsKeyDown(KEY_E) && !m_is_walk) return 1;
	else return 0;
}

void Player::Stop()
{
	m_texture_pos = m_texture_last_pos;
}

void Player::OnLand()
{
	if (speed() == 2.0f) m_timer += frame_time() * 0.23f;
	else m_timer += frame_time() * 0.33f;

	if (m_is_walk && m_timer >= m_update_time)
	{
		m_timer = 0.0f;
		PlaySound(m_land_step);
	}
}

void Player::OnWater()
{
	if (speed() == 2.0f) m_timer += frame_time() * 0.11f;
	else m_timer += frame_time() * 0.13;

	if (m_is_walk && m_timer >= m_update_time)
	{
		m_timer = 0.0f;
		PlaySound(m_water_step);
	}
}

void Player::Draw()
{
	m_texture_last_pos = m_texture_pos;

	if (Vector2Length(direction()) != 0.0f)
	{
		m_is_walk = 1;
		m_texture_pos = m_texture_pos.Subtract(direction().Normalize().Scale(speed()));
		m_texture = m_texture_walk;
		if (direction().x < 0.0f)  m_facing = 1.0f;
		if (direction().x > 0.0f)  m_facing = -1.0f;
	}
	else
	{
		m_is_walk = 0;
		m_texture = m_texture_idle;
	}

	if (IsPunch())  m_texture = m_texture_punch;

	Animate(m_texture_pos, m_texture, frame_time(), 1.5f, row(), m_facing, timer());
}

// ---------------- Private Functions ------------------------------------------

Vec2 Player::direction() const
{
	Vector2 direction{};

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) direction.x -= 1.0f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) direction.x += 1.0f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) direction.y -= 1.0f;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) direction.y += 1.0f;

	return direction;
}

float Player::row() const
{
	if (IsPunch() && !m_is_walk) return 3.0f;
	else if (!m_is_walk) return 2.0f;
	else if (m_is_walk) return 6.0f;
}

float Player::speed() const
{
	if (IsKeyDown(KEY_SPACE)) return 4.0f; // walk fast
	else return 2.0f; // walk slow
}

float Player::timer() const
{
	if (m_is_walk) return 0.0f;
	else if (IsPunch() && !m_is_walk) return 0.02f;
	else return -0.2f;
}

float Player::frame_time() const
{
	return GetFrameTime();
}