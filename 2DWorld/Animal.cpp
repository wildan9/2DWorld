/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2024 Wildan R Wijanarko
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

Bat CreateBat(math::vec2 pos)
{
    Bat b;
    b.pos = pos;
    b.speed = math::vec2{ 0.7f, 0.9f };
    b.scl = 0.6f;
    b.facing = 1.0f;

    const std::vector<std::string> texturesPaths
    {
        "resources/textures/animals/bat/fly.png"
    };

    b.model = LoadModel2D(texturesPaths);
    b.model.animData = CreateAnimData();

    b.model.currTexture = &b.model.textures->at(0);

    return b;
}

void DeleteBat(Bat& bat)
{
    UnloadModel2D(bat.model);
}

void Bat::Update()
{
    int frameSpeed = 10;
    int numFrames = 6;

    animCurrFrame = 1;

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

    UpdateAnim(model, facing, frameSpeed, numFrames, animCurrFrame, 1);
}

void Bat::Draw() const
{
    DrawRectangleLines(pos.x, pos.y, 15, 15, GREEN);
    DrawModel2D(model);
}