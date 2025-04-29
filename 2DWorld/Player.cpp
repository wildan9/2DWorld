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

static void UpdatePlayerTrans(Trans2D& trans, Vector2 pos, float rot, float scl);

static struct
{
    float dt = 0.0f;
    const float ut = 0.084f;

    Sound landStep{};

    inline void Load()
    {
        landStep = LoadSound("resources/sounds/land_step.wav");
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
    rot = 0.0f;
    scl = 0.6f;
    rad = 10.0f;
    facing = 1.0f;
    isWalk = 1;
    rec = Rectangle{ pos.x, pos.y, 10.0f, 15.0f };

    const std::vector<std::string> texturesPaths
    {
        "resources/textures/character/friendly_man_idle.png",
        "resources/textures/character/friendly_man_punch.png",
        "resources/textures/character/friendly_man_walk.png",
        "resources/textures/character/horse_riding/idle.png",
        "resources/textures/character/horse_riding/walk.png"
    };

    model = LoadModel2D(texturesPaths);
    model.animData = CreateAnimData();

    playerSound.Load();
}

Player::~Player()
{
    UnloadModel2D(model);
    playerSound.Unload();
}

void Player::Update()
{
    dir = Vector2InputDir();

    lastPos = pos;

    float speed = (IsKeyDown(KEY_SPACE)) ? 2.2f : 1.2f;

    bool isPunch = IsKeyDown(KEY_E);
    bool isOnHorse = 0;

    float frameSpeed = 6.0f;
    int numFrames = 2;

    if (Vector2Length(dir) != 0)
    {
        isWalk = 1;

        pos = pos - Vector2Scale(dir, speed);
        animCurrTexture = (isOnHorse) ? 5 : 3;

        if (dir.x < 0.0f) facing = 1.0f;
        if (dir.x > 0.0f) facing = -1.0f;

        numFrames = 6;
        frameSpeed = 12;

        float st = (speed > 2.0f) ? 0.33f : 0.23f;
        playerSound.Play("land", st);
    }
    else
    {
        isWalk = 0;
        animCurrTexture = (isOnHorse) ? 4 : 1;
    }

    if (!isWalk && isPunch)
    {
        animCurrTexture = 2;
        frameSpeed = 16;
        numFrames = 3;
    }

    rec.x = pos.x - 10.0f;
    rec.y = pos.y - 10.0f;

    Vector2 playerDrawPos = Vector2{ pos.x - 15.0f, pos.y - 15.0f };
    UpdatePlayerTrans(model.trans, playerDrawPos, rot, scl);
    UpdateAnim(model, facing, frameSpeed, numFrames, animCurrTexture, 1);
}

void Player::Draw() const
{
    DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GREEN);
    DrawCircleLinesV(pos, rad, RED);
    DrawModel2D(model);
}

static void UpdatePlayerTrans(Trans2D& trans, Vector2 pos, float rot, float scl)
{
    trans.pos = pos;
    trans.rot = rot;
    trans.scl = scl;
}