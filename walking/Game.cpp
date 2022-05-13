/* by Wildan R. */

#include "Game.h"

Game::Game()
{
	InitWindow(_windowWidth, _windowHeight, _windowTitle.c_str());
	InitAudioDevice();
}

Game::~Game()
{
	CloseWindow();
	CloseAudioDevice();
}

enum class GameScreen { LOGO = 0, TITLE, GAMEPLAY };

void Game::Run()
{
    SetTargetFPS(60);

    GameScreen currentScreen{ GameScreen::LOGO };

    int framesCounter{ 0 };

    GameObject gameObj;

    gameObj.animals.SetBatFlyRadius(gameObj.map.GetDreamlandSize().width * gameObj.map.GetMapScale());

    _camera.target = gameObj.wildan.GetPosition();
    _camera.offset = Vector2D{ _windowWidth / 2.0f,  _windowHeight / 2.0f };
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case GameScreen::LOGO:
        {
            framesCounter++;

            if (framesCounter > 180) currentScreen = GameScreen::TITLE;
            
        } break;
        case GameScreen::TITLE:
        {
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) currentScreen = GameScreen::GAMEPLAY;
        } break;
        default: break;
        }

        BeginDrawing();

        ClearBackground(SKYBLUE);

        switch (currentScreen)
        {
        case GameScreen::LOGO:
        {
            DrawText("Walking", 120, 220, 80, BLACK);

        } break;
        case GameScreen::TITLE:
        {
            DrawText("Walking", 15, 20, 40, BLACK);
            DrawText("PRESS ENTER or TAP to Walking!", 80, 220, 20, BLACK);

        } break;
        case GameScreen::GAMEPLAY:
        {
            _camera.target = gameObj.wildan.GetPosition();
            _camera.BeginMode();
            gameObj.CheckCollision();
            gameObj.PlayWalkSound();
            gameObj.Draw(GetFrameTime());
            DrawText(std::to_string((int)gameObj.wildan.GetPosition().x).append(" ").append(std::to_string((int)gameObj.wildan.GetPosition().y)).c_str(), 
                (int)GetScreenToWorld2D(Vector2D{}, _camera).x + 20, (int)GetScreenToWorld2D(Vector2D{}, _camera).y + 450, 30, BLACK);
            _camera.EndMode();

        } break;
        default: break;
        }

        EndDrawing();
    }
}