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

#include "Player.h"

static void UpdatePlayerTrans(math::trans2d& trans, math::vec2 pos, float rot, float scl);

static struct
{
    float dt;
    const float ut = 0.084f;

    Sound landStep;

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

Player CreatePlayer()
{
    Player p;
    p.pos = math::vec2{ 300.0f, 300.0f };
    p.rot = 0.0f;
    p.scl = 0.6f;
    p.rad = 10.0f;
    p.facing = 1.0f;
    p.isWalk = 1;
    p.rec = math::rec{ p.pos.x, p.pos.y, 10.0f, 15.0f };

    const std::vector<std::string> texturesPaths
    {
        "resources/textures/character/friendly_man_idle.png",
        "resources/textures/character/friendly_man_punch.png",
        "resources/textures/character/friendly_man_walk.png",
        "resources/textures/character/horse_riding/idle.png",
        "resources/textures/character/horse_riding/walk.png"
    };

    p.model = LoadModel2D(texturesPaths);
    p.model.animator = std::make_unique<Animator>(CreateAnimator());

    p.model.currTexture = &p.model.textures->at(0);

    playerSound.Load();

    return p;
}

void DeletePlayer(Player& player)
{
    UnloadModel2D(player.model);
    playerSound.Unload();
}

void Player::Update()
{
    dir = math::vec2_input_dir();

    lastPos = pos;

    float speed = (IsKeyDown(KEY_SPACE)) ? 2.2f : 1.2f;

    bool isPunch = IsKeyDown(KEY_E);
    bool isOnHorse = 0;

    int frameSpeed = 6;
    int numFrames = 2;

    if (dir.length() != 0)
    {
        isWalk = 1;

        pos = pos - dir.normalize().scale(speed);
        model.textures->at(0) = (isOnHorse) ? model.textures->at(5) : model.textures->at(3);

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
        model.textures->at(0) = (isOnHorse) ? model.textures->at(4) : model.textures->at(1);
    }

    if (!isWalk && isPunch)
    {
        model.textures->at(0) = model.textures->at(2);
        frameSpeed = 16;
        numFrames = 3;
    }

    rec.x = pos.x - 10.0f;
    rec.y = pos.y - 10.0f;

    model.facing = facing;

    math::vec2 playerDrawPos = math::vec2{ pos.x - 15.0f, pos.y - 15.0f };
    UpdatePlayerTrans(model.trans, playerDrawPos, rot, scl);
    UpdateAnim(model, frameSpeed, numFrames, 1);
}

void Player::Draw() const
{
    DrawRectangleLines(rec.x, rec.y, rec.w, rec.h, GREEN);
    DrawCircleLinesV(math::rl_vec(pos), rad, RED);
    DrawModel2D(model);
}

static void UpdatePlayerTrans(math::trans2d& trans, math::vec2 pos, float rot, float scl)
{
    trans.pos = pos;
    trans.rot = rot;
    trans.scl = scl;
}