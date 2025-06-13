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

#include "Player.h"

static struct
{
    float dt = 0.0f;
    const float ut = 0.084f;

    Sound landStep{};

    inline void Load()
    {
        landStep = LoadSound("resources/sounds/grass_walk.wav");
    }

    inline void Unload()
    {
        UnloadSound(landStep);
    }

    inline void Play(const char* sound, float t)
    {
        if (sound == "land")
        {
            dt += GetFrameTime() * t;
            if (dt >= ut)
            {
                dt = 0.0f;
                PlaySound(landStep);
            }
        }
    }

} playerSound;

void Player::Start()
{
    pos = Vector2{ 300.0f, 300.0f };
    rad = 10.0f;
    facing = 1.0f;
    isWalk = 1;
    rec = Rectangle{ pos.x, pos.y, 10.0f, 15.0f };

    playerSound.Load();

    frameCol = 9;
    frameRow = 8;
    selectedRow = 1;
    frameFacing = 1.0f;
    frameScale = 0.4f;
    frameSpeed = 8.0f;
    totalRows = 2;
    rad = 10.0f;

    pos = Vector2{ 517.0f, 390.0f };

    sprite = std::make_unique<Sprite>(pos, "resources/Spritesheet/player.png", frameCol, frameRow, frameFacing);
}

Player::~Player()
{
    playerSound.Unload();
}

static inline Vector2 Vector2InputDir()
{
    Vector2 dir{ 0 };

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dir.x -= 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dir.x += 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dir.y -= 1.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dir.y += 1.0f;

    return Vector2Normalize(dir);
}

void Player::Update()
{
    sprite->Update(Vector2{ pos.x - rad - 15, pos.y - rad - 5 }, frameScale, frameSpeed, selectedRow, frameFacing, totalRows, 0);

    dir = Vector2InputDir();

    lastPos = pos;
    frameFacing = facing*-1.0f;

    float speed = (IsKeyDown(KEY_SPACE)) ? 2.2f : 1.2f;

    bool isConjuring = IsKeyDown(KEY_E);
    bool isOnHorse = 0;

    if (Vector2Length(dir) != 0)
    {
        isWalk = 1;

        pos = pos - Vector2Scale(dir, speed);

        if (dir.x < 0.0f) facing = 1.0f;
        if (dir.x > 0.0f) facing = -1.0f;

        totalRows = 8;
        selectedRow = (IsKeyDown(KEY_SPACE)) ? 6 : 0;
        frameSpeed = 12.0f;

        float st = (speed > 2.0f) ? 0.28f : 0.18f;
        playerSound.Play("land", st);
    }
    else
    {
        isWalk = 0;
        totalRows = 2;
        frameSpeed = 4.0f;
        selectedRow = 1;
    }

    if (!isWalk && isConjuring)
    {
        totalRows = 7;
        frameSpeed = 9.0f;
        selectedRow = 2;
    }

    rec.x = pos.x - 10.0f;
    rec.y = pos.y - 10.0f;

    Vector2 playerDrawPos = Vector2{ pos.x - 15.0f, pos.y - 15.0f };
}

void Player::Draw() const
{
    DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GREEN);
    DrawCircleLinesV(pos, rad, RED);
    sprite->Draw();
}