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

struct GameObject
{
    Player wildan{ Vector2D{ 40.0f, 140.0f } };
    Prop prop;
    Map map;
    Animals animals;

    void PlayWalkSound()
    {
        if (wildan.GetPosition().x < -15.0f || wildan.GetPosition().y < 0.0f ||
            wildan.GetPosition().x > ((float)map.GetDreamlandSize().width - 15.0f) * map.GetMapScale() &&
            wildan.GetPosition().x < (float)map.GetDreamlandSize().width + 1500.0f ||
            wildan.GetPosition().y > (float)map.GetDreamlandSize().height * map.GetMapScale()) wildan.OnWater();
        else wildan.OnLand();
    }

    void CheckCollision()
    {
        if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) &&
            wildan.IsPunch() && (wildan.GetFacing() == 1.0f && animals.crocodile.GetFacing() >= -1.0f &&
            wildan.GetPosition().x < animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
        else if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) &&
            wildan.IsPunch() && (wildan.GetFacing() == -1.0f && animals.crocodile.GetFacing() >= -1.0f &&
            wildan.GetPosition().x > animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
        else animals.crocodile.Walk();

        for (auto& rhino : animals.rhinos) if (CheckCollisionRecs(wildan.GetCollision(), rhino.GetRectColl())) wildan.Stop();

        if (CheckCollisionRecs(wildan.GetCollision(), prop.invisible_fence.GetFenceRectangle1()) ||
            CheckCollisionRecs(wildan.GetCollision(), prop.invisible_fence.GetFenceRectangle2())) wildan.Stop();

        if (CheckCollisionRecs(wildan.GetCollision(), prop.invisible_fence.GetTreeRectangle1()) ||
            CheckCollisionRecs(wildan.GetCollision(), prop.invisible_fence.GetTreeRectangle2())) wildan.Stop();

        if (CheckCollisionRecs(wildan.GetCollision(), prop.natural_obj.GetBigStone1Coll())) wildan.SetPosition(Vector2D{ 3000.0f, 300.0f });

        if (CheckCollisionRecs(wildan.GetCollision(), prop.natural_obj.GetBigStone2Coll())) wildan.SetPosition(Vector2D{ 40.0f, 140.0f });
    }

    void Draw()
    {
        map.Draw();
        wildan.Draw();
        prop.Draw();
        animals.Draw(GetFrameTime());
    }
};

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