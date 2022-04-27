/* by Wildan R. */

#pragma once

#include "raylib.h"

struct MapSize
{
	int width;
	int height;
};

class Map
{
public:
	Map();
	~Map();
	MapSize GetDreamlandSize() const;
	MapSize GetDesertSize() const;
	Vector2 GetDreamlandPos() const;
	Vector2 GetDesertPos() const;
	Rectangle GetMapLine1() const;
	Rectangle GetMapLine2() const;
	float GetMapScale() const;
	void Draw();

private:
	const float m_map_scale{ 2.0f };
	Texture2D m_dreamland{ LoadTexture("textures/maps/dreamland.png") };
	Texture2D m_desert{ LoadTexture("textures/maps/desert.png") };
	Vector2 m_dreamland_pos{};
	Vector2 m_desert_pos{ m_dreamland.width * m_map_scale + 760.0f, 0.0f };
};