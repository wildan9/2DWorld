/* by Wildan R. */

#include "Map.h"

Map::Map() {}

Map::~Map()
{
	UnloadTexture(_wildanEmpire);
	UnloadTexture(_desert);
}

void Map::Draw()
{
	DrawTextureEx(_wildanEmpire, _wildanEmpirePos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_desert, _desertPos, 0.0f, _mapScale, WHITE);
}