/* by Wildan R. */

#include "Map.h"

Map::Map()
{

}

Map::~Map()
{
	UnloadTexture(_dreamland);
	UnloadTexture(_desert);
}

MapSize Map::GetDreamlandSize() const
{
	return MapSize{ _dreamland.width, _dreamland.height }; // 768 x 768 
}

MapSize Map::GetDesertSize() const
{
	return MapSize{ _desert.width, _desert.height }; // 768 x 768 
}

Vector2D Map::GetDreamlandPos() const
{
	return _dreamlandPos;
}

Vector2D Map::GetDesertPos() const
{
	return _desertPos;
}

Rectangle Map::GetMapLine1() const
{
	return Rectangle{ _desertPos.x - 480.0f, 0.0f, 100.0f, (float)_dreamland.height * _mapScale };
}

Rectangle Map::GetMapLine2() const
{
	return Rectangle{ (float)_dreamland.width * _mapScale * 2.5f, 0.0f, 100.0f, (float)_desert.height * _mapScale };
}

float Map::GetMapScale() const
{
	return _mapScale;
}

void Map::Draw()
{
	DrawTextureEx(_dreamland, _dreamlandPos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_desert, _desertPos, 0.0f, _mapScale, WHITE);
}