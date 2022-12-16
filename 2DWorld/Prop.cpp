/* by Wildan R. */

#include "Math2D.h"

struct NaturalObject
{
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
		DrawTextureV(_bigStone, _bigStone1Pos, WHITE);
		DrawTextureV(_bigStone, _bigStone2Pos, WHITE);
	}

private:
	Texture2D _bigStone{ LoadTexture("textures/natural_objects/big_stone.png") };
	Vector2D _bigStone1Pos{ 1600.0f, 700.0f };
	Vector2D _bigStone2Pos{ 5400.0f, 600.0f };
};

static const char* treeTextures[4]
{
	"textures/natural_objects/trees_1.png",
	"textures/natural_objects/trees_2.png",
	"textures/natural_objects/trees_3.png",
	"textures/natural_objects/trees_4.png"
};

struct Tree
{
	Tree() {}

	~Tree()
	{
		UnloadTexture(_texture);
	}

	void LoadTextureFile()
	{
		_texture = LoadTexture(treeTextures[GetRandomValue(0, 3)]);
	}

	void SetPosition(const Vector2D position)
	{
		_position = position;
	}

	Rectangle GetCollision()
	{
		return Rectangle{
			static_cast<float>(_position.x),
			static_cast<float>(_position.y),
			static_cast<float>(_texture.width * _textureScale),
			static_cast<float>(_texture.height * _textureScale)
		};
	}

	void Draw()
	{
		DrawTextureEx(_texture, _position, 0.0f, _textureScale, WHITE);
	}

private:
	Texture2D _texture{};
	Vector2D _position{};
	const float _textureScale{ 3.0f };
};

struct MagicFruit
{
    ~MagicFruit()
    {
        UnloadTexture(_texture);
    }

    MagicFruit(const Vector2D position)
    {
        _position = position;
    }

    MagicFruit() {}

    Rectangle GetCollision()
    {
        return Rectangle{
            static_cast<float>(_position.x),
            static_cast<float>(_position.y),
            static_cast<float>(_texture.width),
            static_cast<float>(_texture.height)
        };
    }

    void LoadTexture(Texture2D texture)
    {
        _texture = texture;
    }

    void Draw()
    {
        DrawTextureV(_texture, _position, WHITE);
    }

private:
    Texture2D _texture{};
    Vector2D _position{ 585.0f, 760.0f };
};

struct Flower
{
    Flower(const Vector2D& position)
    {
        _position = position;
        _texture = LoadTexture("textures/natural_objects/small_purple_flower.png");
    }

    ~Flower()
    {
        UnloadTexture(_texture);
    }

    void Draw()
    {
        DrawTextureV(_texture, _position, WHITE);
    }

private:
    Texture2D _texture{};
    Vector2D _position{};
};

struct Carrot
{
    Carrot(const Vector2D& position)
    {
        _position = position;
        _texture = &_textureSmall;
        _textureSmall = LoadTexture("textures/natural_objects/carrot/small.png");
        _textureMedium = LoadTexture("textures/natural_objects/carrot/medium.png");
        _textureBig = LoadTexture("textures/natural_objects/carrot/big.png");
        _carrotAge = 0;
    }

    ~Carrot()
    {
        if (_carrotStates == _CarrotStates::READY)
        {
            UnloadTexture(_textureBig);
        }
        else
        {
            UnloadTexture(_textureSmall);
            UnloadTexture(_textureMedium);
            UnloadTexture(_textureBig);
        }
    }

    void Draw()
    {
        if (_carrotAge < 30.0f) _carrotAge += GetFrameTime();

        if (_carrotAge > 12.0f && _carrotAge < 28.0f) _texture = &_textureMedium;

        if (_carrotAge > 28.0f && _carrotStates != _CarrotStates::READY) _carrotStates = _CarrotStates::BIG;

        switch (_carrotStates)
        {
        case _CarrotStates::BIG:

            UnloadTexture(_textureSmall);
            UnloadTexture(_textureMedium);

            _texture = &_textureBig;
            _carrotStates = _CarrotStates::READY;

            break;
        default:
            break;
        }

        DrawTextureV(*_texture, _position, WHITE);
    }

private:
    Texture2D* _texture{};
    Texture2D  _textureSmall{};
    Texture2D  _textureMedium{};
    Texture2D  _textureBig{};
    Vector2D   _position{};
    float      _carrotAge{};
    enum class _CarrotStates
    {
        SMALL = 0,
        BIG,
        READY
    };
    _CarrotStates _carrotStates = _CarrotStates::SMALL;
};

struct Prop
{
	NaturalObject naturalObj;

	void Draw()
	{
		naturalObj.Draw();
	}
};