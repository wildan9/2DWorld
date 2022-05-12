/* by Wildan R. */

#include "Header.h"

const Rectangle invisibleFences[]
{
	Rectangle{ 980.414f, 280.586f, 150.0f, 560.0f },	
	Rectangle{ 980.414f + 280.0f, 280.586f + 180.0f, 150.0f, 450.0f },
	Rectangle{ 40.0f, 252.0f, 250.0f, 0.2f },
	Rectangle{ 40.0f, 900.0f, 350.0f, 0.2f }
};

class NaturalObject
{
public:
	NaturalObject()
	{

	}

	~NaturalObject()
	{
		UnloadTexture(_bigStone);
	}

	Rectangle GetBigStone1Coll()
	{
		return Rectangle{
			static_cast<float>(_bigStone1Pos.x + 45.0f),
			static_cast<float>(_bigStone1Pos.y + 35.0f),
			static_cast<float>(_bigStone.width * 0.65f),
			static_cast<float>(_bigStone.height * 0.65f)
		};
	}

	Rectangle GetBigStone2Coll()
	{
		return Rectangle{
			static_cast<float>(_bigStone2Pos.x + 45.0f),
			static_cast<float>(_bigStone2Pos.y + 35.0f),
			static_cast<float>(_bigStone.width * 0.65f),
			static_cast<float>(_bigStone.height * 0.65f)
		};
	}

	void Draw()
	{
		DrawTextureV(_bigStone, _bigStone1Pos.ToVector2(), WHITE);
		DrawTextureV(_bigStone, _bigStone2Pos.ToVector2(), WHITE);
	}

private:
	Texture2D _bigStone{ LoadTexture("textures/natural_objects/big_stone.png") };
	Vector2D _bigStone1Pos{ 1600.0f, 700.0f };
	Vector2D _bigStone2Pos{ 3400.0f, 600.0f };
};


struct Prop
{
	NaturalObject naturalObj;

	void Draw()
	{
		naturalObj.Draw();
	}
};