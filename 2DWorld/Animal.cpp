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

    frameCol = 5;
    frameRow = 3;
    selectedRow = 1;
    frameFacing = 1.0f;
    frameScale = 1.0f;
    frameSpeed = 8.0f;

    sprite = std::make_unique<Sprite>(pos, "resources/Spritesheet/bat_sprite_sheet.png", frameCol, frameRow, frameFacing);
}

void Bat::Update()
{
    float frameSpeed = 10.0f;
    int numFrames = 6;

    const float flyRadius = 1200.0f;

    if (pos.x >= flyRadius || pos.x <= 0)
    {
        speed.x *= -1.0f;
        frameFacing *= -1.0f;
    }
    if (pos.y >= flyRadius || pos.y <= 0)
    {
        speed.y *= -1.0f;
    }

    pos = pos + speed;

    sprite->Update(Vector2{ pos.x - rad - 5, pos.y - rad - 5 }, frameScale, frameSpeed, selectedRow, frameFacing, 5, 0);
}

void Bat::Draw() const
{
    DrawRectangleLines(pos.x, pos.y, 15, 15, GREEN);
    sprite->Draw();
    
}

void Horse::Start()
{
    pos = pos;

    frameCol = 13;
    frameRow = 1;
    selectedRow = 0;
    frameFacing = 1.0f;
    frameScale = 1.0f;
    frameSpeed = 8.0f;

    sprite = std::make_unique<Sprite>(pos, "resources/textures/animals/horse/idle.png", frameCol, frameRow, frameFacing);
}

void Horse::Update()
{
    sprite->Update(Vector2{ pos.x - rad - 5, pos.y - rad - 5 }, frameScale, frameSpeed, selectedRow, frameFacing, 13, 0);
}

void Horse::Draw() const
{
    DrawRectangleLines(pos.x, pos.y, 15, 15, GREEN);
    sprite->Draw();
}

void Frog::Start()
{
    frameCol = 8;
    frameRow = 3;
    selectedRow = 0;
    frameFacing = -1.0f;
    frameScale = 0.4f;
    frameSpeed = 8.0f;

    pos = Vector2{517.0f, 390.0f};

    sprite = std::make_unique<Sprite>(pos, "resources/Spritesheet/frog_sprite_sheet.png", frameCol, frameRow, frameFacing);
    rad = 6.0f;

    rec = Rectangle{pos.x - 60, pos.y - 60, 140.0f, 140.0f};

    sound = LoadSound("resources/sounds/frog-croaking-sound-effect-322956.wav");
}

void Frog::Update(const Rectangle& playerRec)
{
    if (CheckCollisionRecs(rec, playerRec) && !IsSoundPlaying(sound))
    {
        PlaySound(sound);
    }

    sprite->Update(Vector2{pos.x - rad - 5, pos.y - rad - 5}, frameScale, frameSpeed, selectedRow, frameFacing, 6, 0);
}

void Frog::Draw() const
{
    DrawCircleLinesV(pos, rad, GREEN);
    DrawRectangleLinesEx(rec, 0.4f, RED);
    sprite->Draw();
}

Frog::~Frog()
{
    UnloadSound(sound);
}
