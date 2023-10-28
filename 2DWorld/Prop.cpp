/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2023 Wildan Wijanarko (@wildan9)
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#include "raylib.h"

#include "GameObject.h"

struct MagicFruit
{
    MagicFruit(const Vector2 pos) : position{ pos } {}
    ~MagicFruit() { UnloadTexture(texture); }

    Rectangle GetRectangle()
    {
        return Rectangle{
            static_cast<float>(position.x),
            static_cast<float>(position.y),
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        };
    }

    void Draw() { DrawTextureV(texture, position, WHITE); }

    Texture2D texture{ LoadTexture("resources/textures/magic_fruit/apple.png") };
    
    Vector2 position;
};

struct Flower
{
    Flower(const Vector2 pos) : position{ pos } {}
    ~Flower() { UnloadTexture(texture); }

    void Draw() { DrawTextureV(texture, position, WHITE); }

    Texture2D texture{ LoadTexture("resources/textures/natural_objects/small_purple_flower.png") };
    
    Vector2 position;
};

class House : public GameObject
{
public:
    House()
    {
        Start();
    }

    ~House()
    {
        _textures.Clear();
    }

protected:
    void Start() override
    {
        _textures.LoadTextureFile("resources/textures/house/house1.png");

        _pCurrentTexture = &_textures[0];

        _position = { 1340.0f, 170.0f, 3.0f };

        _rectangle = { _position.x, _position.y, 90.0f, 140.0f };

        name = "House";

        _scale = 0.8f;
    }

    void Update() override
    {
        _rectangle = { _position.x + 10.0f, _position.y + 40.0f, 90.0f, 140.0f };

        Animate(1, 1, 0);

        if (isOnTrigger) _position.z = 1.0f;
        else _position.z = 3.0f;
    }
};

class Ark : public GameObject
{
public:
    Ark()
    {
        Start();
    }

    ~Ark()
    {
        _textures.Clear();
    }

protected:
    void Start() override
    {
        _textures.LoadTextureFile("resources/textures/ark/ark.png");

        _pCurrentTexture = &_textures[0];

        _position = { -10.0f, 900.0f, 3.0f };

        _rectangle = { _position.x, _position.y, 440.0f, 200.0f };

        name = "Ark";

        _scale = 0.4f;
    }

    void Update() override
    {
        _rectangle = { _position.x, _position.y + 30.0f, 440.0f, 200.0f };

        Animate(1, 1, 0);

        if (isOnTrigger) _position.z = 1.0f;
        else _position.z = 3.0f;
    }
};