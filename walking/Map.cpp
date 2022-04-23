/* by Wildan R. */

#include "Map.h"

Map::Map()
{

}

Map::~Map()
{
	UnloadTexture(m_dreamland);
	UnloadTexture(m_desert);
}

Size Map::GetDreamlandSize() const
{
	return Size{ m_dreamland.width, m_dreamland.width };
}

Size Map::GetDesertSize() const
{
	return Size{ m_desert.width, m_desert.width };
}

Vector2 Map::GetDreamlandPos() const
{
	return m_dreamland_pos;
}

Vector2 Map::GetDesertPos() const
{
	return m_desert_pos;
}

Rectangle Map::GetMapLine1() const
{
	return Rectangle{ m_desert_pos.x - 480.0f, 0.0f, 100.0f, (float)m_dreamland.height * m_map_scale };
}

Rectangle Map::GetMapLine2() const
{
	return Rectangle{ (float)m_dreamland.width * m_map_scale * 2.5f, 0.0f, 100.0f, (float)m_desert.height * m_map_scale };
}

float Map::GetMapScale() const
{
	return m_map_scale;
}

void Map::Draw()
{
	DrawTextureEx(m_dreamland, m_dreamland_pos, 0.0f, m_map_scale, WHITE);
	DrawTextureEx(m_desert, m_desert_pos, 0.0f, m_map_scale, WHITE);
}