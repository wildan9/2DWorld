/* by Wildan R. */

#pragma once

#include "Math2D.h"
#include <array>

struct MapSize { int width; int height; };

class Map
{
public:
	Map();
	~Map();
	void Draw();

	inline MapSize	 GetWildanEmpireSize() const { return { _wildanEmpire.width, _wildanEmpire.height }; }
	inline MapSize	 GetDesertSize() const { return { _desert.width, _desert.height }; }
	inline float	 GetMapScale() const { return { _mapScale }; }
	inline Vector2D	 GetWildanEmpirePos() const { return { _wildanEmpirePos }; }
	inline Vector2D	 GetDesertPos() const { return { _desertPos }; }
	inline Rectangle GetMapLine1() const { return { _desertPos.x - 480.0f, 0.0f, 100.0f, (float)_wildanEmpire.height * _mapScale }; }
	inline Rectangle GetMapLine2() const { return { (float)_wildanEmpire.width * _mapScale * 2.5f, 0.0f, 100.0f, (float)_desert.height * _mapScale }; }
	
	std::array<Rectangle, 4> GetMapRiverRects() const;

private:
	const float _mapScale = 2.0f;
	const Texture2D _wildanEmpire{ LoadTexture("textures/maps/wildan_empire.png") };
	const Texture2D _desert{ LoadTexture("textures/maps/desert.png") };
	const Texture2D _river1{ LoadTexture("textures/maps/river/river_1.png") };
	const Texture2D _river2{ LoadTexture("textures/maps/river/river_2.png") };
	const Texture2D _river3{ LoadTexture("textures/maps/river/river_3.png") };
	const Texture2D _river4{ LoadTexture("textures/maps/river/river_4.png") };
	const Vector2D _wildanEmpirePos{};
	const Vector2D _desertPos{ _wildanEmpire.width * _mapScale + 760.0f, 0.0f };
	const Vector2D _river1Pos{ 0.0f, 1300.0f };
	const Vector2D _river2Pos{ _river1.width * _mapScale, 1300.0f };
	const Vector2D _river3Pos{ _river1.width * _mapScale + _river2.width * _mapScale - 205.0f, 0.0f };
	const Vector2D _river4Pos{ _river1.width * _mapScale + _river2.width * _mapScale - 205.0f, _river3.height * _mapScale - 60.0f };
};