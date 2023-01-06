/* by Wildan R. */

#include "Map.h"

Map::Map() {}

Map::~Map()
{
	UnloadTexture(_wildanEmpire);
	UnloadTexture(_desert);
	UnloadTexture(_river1);
	UnloadTexture(_river2);
	UnloadTexture(_river3);
	UnloadTexture(_river4);
}

std::array<Rectangle, 4> Map::GetMapRiverRects() const
{
	std::array<Rectangle, 4> river{
		Rectangle{ _river1Pos.x, _river1Pos.y, (float)_river1.width * _mapScale - 160.0f, (float)_river1.height * _mapScale },
		Rectangle{ _river2Pos.x, _river2Pos.y, (float)_river2.width * _mapScale, (float)_river2.height * _mapScale },
		Rectangle{ _river3Pos.x, _river3Pos.y, (float)_river3.width * _mapScale, (float)_river3.height * _mapScale - 270.0f },
		Rectangle{ _river4Pos.x, _river4Pos.y, (float)_river4.width * _mapScale, (float)_river4.height * _mapScale }
	};

	return river;
}

void Map::Draw()
{
	DrawTextureEx(_wildanEmpire, _wildanEmpirePos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_desert, _desertPos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_river1, _river1Pos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_river2, _river2Pos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_river3, _river3Pos, 0.0f, _mapScale, WHITE);
	DrawTextureEx(_river4, _river4Pos, 0.0f, _mapScale, WHITE);
}