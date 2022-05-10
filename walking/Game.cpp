/* by Wildan R. */

#include "Game.h"

Game::Game()
{
	InitWindow(m_window_width, m_window_height, m_window_title.c_str());
	InitAudioDevice();
	SetTargetFPS(60);
}

Game::~Game()
{
	CloseWindow();
	CloseAudioDevice();
}

enum class GameScreen { LOGO = 0, TITLE, GAMEPLAY };

void Game::Run()
{
    GameScreen current_screen{ GameScreen::LOGO };

    int frames_counter{ 0 };

    GameObject game_obj;

    game_obj.animals.SetBatFlyRadius(game_obj.map.GetDreamlandSize().width * game_obj.map.GetMapScale());

    m_camera.target = game_obj.wildan.GetPosition().ToVector2();
    m_camera.offset = Vector2D{ m_window_width / 2.0f,  m_window_height / 2.0f }.ToVector2();
    m_camera.rotation = 0.0f;
    m_camera.zoom = 1.0f;

    while (!WindowShouldClose())
    {
        switch (current_screen)
        {
        case GameScreen::LOGO:
        {
            frames_counter++;

            if (frames_counter > 180) current_screen = GameScreen::TITLE;
            
        } break;
        case GameScreen::TITLE:
        {
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) current_screen = GameScreen::GAMEPLAY;
        } break;
        default: break;
        }

        BeginDrawing();

        ClearBackground(SKYBLUE);

        switch (current_screen)
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
            m_camera.target = game_obj.wildan.GetPosition().ToVector2();
            m_camera.BeginMode();
            game_obj.CheckCollision();
            game_obj.PlayWalkSound();
            game_obj.Draw();
            m_camera.EndMode();

        } break;
        default: break;
        }

        EndDrawing();
    }
}