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

#include "2DWorld.h"

enum class WorldStates
{
    TITLE = 0,
    GAMEPLAY
};
WorldStates worldState;

void Engine::Start()
{
    InitWindow(screenWidth, screenHeight, "2DWorld");
    InitAudioDevice();
    SetTargetFPS(60);
    Image icon = LoadImage("resources/icon/earth_400_400.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&icon, BLACK, BLANK);
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetActiveScene(std::make_shared<TitleScene>());

    _audio.LoadResources();

    _threads.push_back(std::thread(&Audio::Update, _audio, std::ref(_currentBGM), std::ref(_isEngineShutDown)));
}

void Engine::Update()
{
    switch (worldState)
    {
    case WorldStates::TITLE:
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            _gameplayScene = std::make_shared<GameplayScene>();

            SetActiveScene(_gameplayScene);

            worldState = WorldStates::GAMEPLAY;

            _collisionThreadRunning = 1;

            _threads.push_back(std::thread(&GameplayScene::CollisionChecking, _gameplayScene, std::ref(_collisionThreadRunning)));
        }

        GetCurrentScene()->Update();

    } break;
    case WorldStates::GAMEPLAY:
    {
        GetCurrentScene()->Update();
    } break;
    default:
        break;
    }
}

void Engine::Render()
{
    BeginDrawing();
    ClearBackground(WHITE);

    if (GetCurrentScene() != nullptr)
    {
        GetCurrentScene()->Draw();
    }

    DrawVolumeBar();

    EndDrawing();
}

void Engine::ShutDown()
{
    _isEngineShutDown = 1;

    // Signal the collision checking thread to stop
    _collisionThreadRunning = 0;

    for (auto& thread : _threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    _threads.clear();

    _audio.FreeResources();

    CloseAudioDevice();
    CloseWindow();
}

int main()
{
    Engine engine;
    engine.Start();

    while (!WindowShouldClose())
    {
        engine.Update();
        engine.Render();
    }

    engine.ShutDown();

    return 0;
}
