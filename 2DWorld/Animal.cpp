/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2025 Wildan R Wijanarko
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

#include "Animal.h"

void Bat::Start() 
{
    pos = pos;
    speed = Vector2{ 0.7f, 0.9f };
    scl = 0.6f;
    facing = 1.0f;

    const std::vector<std::string> texturesPaths
    {
        "resources/textures/animals/bat/fly.png"
    };

    model = LoadModel2D(texturesPaths);
    model.animData = CreateAnimData();
}

Bat::~Bat()
{
    UnloadModel2D(model);
}

void Bat::Update()
{
    float frameSpeed = 10.0f;
    int numFrames = 6;

    animCurrTexture = 1;

    model.trans.pos = pos;
    model.trans.scl = 1.0f;
    model.trans.rot = 0.0f;

    const float flyRadius = 1200.0f;

    if (pos.x >= flyRadius || pos.x <= 0)
    {
        speed.x *= -1.0f;
        facing *= -1.0f;
    }
    if (pos.y >= flyRadius || pos.y <= 0)
    {
        speed.y *= -1.0f;
    }

    pos = pos + speed;

    UpdateAnim(model, facing, frameSpeed, numFrames, animCurrTexture, 1);
}

void Bat::Draw() const
{
    DrawRectangleLines(pos.x, pos.y, 15, 15, GREEN);
    DrawModel2D(model);
}

void Horse::Start()
{
    pos = pos;
    speed = Vector2{ 0.7f, 0.9f };
    scl = 0.6f;
    facing = 1.0f;

    const std::vector<std::string> texturesPaths
    {
        "resources/textures/animals/horse/idle.png"
    };

    model = LoadModel2D(texturesPaths);
    model.animData = CreateAnimData();
}

Horse::~Horse()
{
    UnloadModel2D(model);
}

void Horse::Update()
{
    float frameSpeed = 4.4f;
    int numFrames = 13;

    animCurrTexture = 1;

    model.trans.pos = pos;
    model.trans.scl = 0.8f;
    model.trans.rot = 0.0f;

    UpdateAnim(model, facing, frameSpeed, numFrames, animCurrTexture, 1);
}

void Horse::Draw() const
{
    DrawRectangleLines(pos.x, pos.y, 15, 15, GREEN);
    DrawModel2D(model);
}

void Frog::Start()
{
    frameCol = 8;
    frameRow = 3;
    selectedRow = 0;
    frameFacing = 1.0f;
    frameScale = 0.7f;
    frameSpeed = 4;

    pos = Vector2{514.0f, 380.0f};

    sprite = std::make_unique<Sprite>(pos, "resources/Spritesheet/frog_sprite_sheet.png", frameCol, frameRow, frameFacing);
    rad = 10.0f;
}

void Frog::Update()
{
    sprite->Update(Vector2{pos.x - rad - 5, pos.y - rad - 5}, frameScale, frameSpeed, selectedRow, frameFacing, 6);
}

void Frog::Draw() const
{
    DrawCircleLinesV(pos, rad, GREEN);
    sprite->Draw();
}

Frog::~Frog()
{

}