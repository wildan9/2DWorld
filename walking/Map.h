/* by Wildan R. */

#pragma once

#include "CoreLib.h"

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
	Vector2D GetDreamlandPos() const;
	Vector2D GetDesertPos() const;
	Rectangle GetMapLine1() const;
	Rectangle GetMapLine2() const;
	float GetMapScale() const;
	void Draw();

private:
	const float _mapScale{ 2.0f };
	Texture2D _dreamland{ LoadTexture("textures/maps/dreamland.png") };
	Texture2D _desert{ LoadTexture("textures/maps/desert.png") };
	Vector2D _dreamlandPos{};
	Vector2D _desertPos{ _dreamland.width * _mapScale + 760.0f, 0.0f };
};