/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023-2025 Wildan R Wijanarko
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

#include "GameplayScene.h"
#include "rlTileMap/ray_tilemap.h"
#include "rlgl.h"

#include <sstream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <chrono>

static std::atomic<bool> dataLoaded{0};
static std::atomic<int> dataProgress{0};

std::thread loaderThread;
enum class MapState { STATE_WAITING, STATE_LOADING_HOME, STATE_LOADING_WORLD };
MapState state = MapState::STATE_WAITING;

bool showGrid = 0, worldCollision = 1, enteringHouse = 0, onSwitch = 0, isCameraScrollable = 1;

static Rectangle GetRecBottomSide(const Rectangle& rec);
static void DrawGrid(int screenWidth, int screenHeight, int cellSize);
static bool OnTouch(const Player& player, float targetPosX);
static void DrawLoadingScreen();
static void LoadDataThread();

RayTiled::TileMap map;
RayTiled::TileLayer* objectTileLayer = nullptr;

Rectangle houseDoor{ 484.0f, 625.0f, 9, 9 };

Player player;
Horse horse;

Timer mapSwitchTimer;

static int framesCounter = 0;

typedef struct Lightning
{
    Model2D model;
    Rectangle rec;
    Sound sound;
    bool isShown;
    float timer;
};
Lightning lightning{};

Lightning InitLightning()
{
    Lightning lgh{};

    const std::vector<std::string> lightningPaths
    {
        "resources/spritesheet/lightning.png"
    };

    lgh.model = LoadModel2D(lightningPaths);
    lgh.model.animData = CreateAnimData();
    lgh.model.trans.pos = Vector2{710.0f, 480.0f};
    lgh.model.trans.rot = 0.0f;
    lgh.model.trans.scl = 0.45f;
    lgh.rec = Rectangle{700.0f, 555.0f, 200.0f, 130.0f};
    lgh.timer = 10.0f;
    lgh.sound = LoadSound("resources/sounds/thunder_explosion_hit.wav");

    return lgh;
}

void UpdateLightning(Lightning& lightning)
{
    if (lightning.isShown)
    {
        if (lightning.timer > 0.0f)
        {
            lightning.timer -= GetFrameTime();

            if (lightning.timer < 0.0f)
            {
                lightning.timer = 10.0f;
                lightning.model.animData->Reset();
                lightning.model.trans.pos = Vector2{(float)GetRandomValue(610.0f, 760.0f), (float)GetRandomValue(410.0f, 480.0f)};
            }
        }

        if (lightning.timer < 1.0f)
        {
            PlaySound(lightning.sound);
            UpdateAnim(lightning.model, 1.0f, 9.4f, 9, 1, 1);
        }
    }

    lightning.isShown = CheckCollisionRecs(player.rec, lightning.rec);
}

void DrawLightning(const Lightning& lightning)
{
    Rectangle rec = lightning.rec;
    DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, RED);
    DrawModel2D(lightning.model);
}

void DestroyLightning(Lightning& lightning)
{
    UnloadSound(lightning.sound);
    UnloadModel2D(lightning.model);
}

void DrawObjectLayerItem(RayTiled::TileLayer& layer, RayTiled::TileLayer::Drawable& drawable, float startX, float endX)
{
    if (&drawable == &player)
    {
        player.Draw();
    }
    else if (&drawable == &horse)
    {
        horse.Draw();
    }
}

void DrawCollisionLayer(RayTiled::ObjectLayer& layer, Camera2D* camera, Vector2 bounds)
{
    for (auto& object : layer.Objets)
    {
        switch (object->Type)
        {
        case RayTiled::ObjectLayer::ObjectType::Generic:
            DrawRectangleRec(object->Bounds, ColorAlpha(BLUE, 0.25f));
            break;

        case RayTiled::ObjectLayer::ObjectType::Point:
            DrawCircleV(Vector2{ object->Bounds.x, object->Bounds.y }, 8, GRAY);
            break;

        case RayTiled::ObjectLayer::ObjectType::Text:
        {
            auto text = static_cast<RayTiled::ObjectLayer::TextObject*>(object.get());
            DrawText(text->Text.c_str(), object->Bounds.x, object->Bounds.y, text->FontSize, WHITE);
        }
        break;
        }
    }
}

void InitWorldMap()
{
    RayTiled::UnloadTileMap(map, 1);
    RayTiled::LoadTileMap("resources/world.tmx", map);

    auto playerLayer = RayTiled::FindLayer(map, "Objects");
    if (playerLayer && playerLayer->Type == RayTiled::TileLayerType::Tile)
    {
        objectTileLayer = static_cast<RayTiled::TileLayer*>(playerLayer);

        objectTileLayer->CustomDrawalbeFunction = DrawObjectLayerItem;

        objectTileLayer->AddDrawable(&player);
        objectTileLayer->AddDrawable(&horse);
    }

    auto collisionlayer = RayTiled::FindLayer(map, "CollisionObjects");
    if (collisionlayer && collisionlayer->Type == RayTiled::TileLayerType::Object)
    {
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->DrawFunc = DrawCollisionLayer;
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->CheckForCollisions = 1;
    }
}

void InitHouseMap()
{
    RayTiled::UnloadTileMap(map, 1);
    RayTiled::LoadTileMap("resources/house.tmx", map);

    auto playerLayer = RayTiled::FindLayer(map, "Objects");
    if (playerLayer && playerLayer->Type == RayTiled::TileLayerType::Tile)
    {
        objectTileLayer = static_cast<RayTiled::TileLayer*>(playerLayer);

        objectTileLayer->CustomDrawalbeFunction = DrawObjectLayerItem;
        objectTileLayer->AddDrawable(&player);
    }

    auto collisionlayer = RayTiled::FindLayer(map, "CollisionObjects");
    if (collisionlayer && collisionlayer->Type == RayTiled::TileLayerType::Object)
    {
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->DrawFunc = DrawCollisionLayer;
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->CheckForCollisions = 1;
    }
}

void GameplayScene::Start()
{
    LoadResources();
}

void GameplayScene::Update()
{
    mapRec = (enteringHouse) ? Rectangle{10.0f, 10.0f, 28.5f * 28.5f / 2.0f, 24.8f * 24.8f / 2.0f} 
    : Rectangle{10.0f, 10.0f, 51.5f * 51.5f / 2.0f, 51.5f * 51.5f / 2.0f};

    camera.Update(player.pos, mapRec, GetScreenWidth(), GetScreenHeight(), isCameraScrollable);

    // Our debug button
    if (IsKeyPressed(KEY_H))
    {
        showGrid = !showGrid;
    }

    player.Update();
    
    for (auto& bat : bats)
    {
        bat.Update();
    }

    if (!enteringHouse)
    {
        horse.Update();
    }

    if (enteringHouse) houseDoor = Rectangle{100.0f, 300.0f, 9, 9};
    else houseDoor = Rectangle{484.0f, 625.0f, 9, 9};

    CollisionChecking();
    UpdateLightning(lightning);
}

void GameplayScene::LoadResources()
{
    player.Start();

    for (int i = 0; i < 10; i++)
    {
        bats[i].Start();
        bats[i].pos = Vector2{100.0f + i*14 + GetRandomValue(2, 6), 100.0f + i*12 + GetRandomValue(3, 5)};
    }

    horse.Start();
    horse.pos = Vector2{912.0f, 626.0f};
    horse.rad = 1.8f;

    isCameraScrollable = 1;
    InitWorldMap();
    player.pos = Vector2{484.0f, 650.0f};
    camera.zoom = 2.0f;
    enteringHouse = 0;

    lightning = InitLightning();
}

void GameplayScene::FreeResources()
{
    RayTiled::UnloadTileMap(map, 1);

    if (loaderThread.joinable())
    {
        loaderThread.join();
    }

    DestroyLightning(lightning);
}

void GameplayScene::Draw()
{
    camera.BeginMode();
        if (!onSwitch)
        {
            RayTiled::DrawTileMap(map, &camera);
        }

        if (!enteringHouse)
        {   
            for (const auto& bat : bats)
            {
                bat.Draw();
            }
            DrawRectangleLinesEx(mapRec, 12, BLACK);
            DrawLightning(lightning);

        }
    camera.EndMode();

    if (state == MapState::STATE_LOADING_HOME || state == MapState::STATE_LOADING_WORLD)
    {
        DrawLoadingScreen();
    }

    if (showGrid)
    {
        DrawGrid(GetScreenWidth(), GetScreenHeight(), 12);
    }

    std::ostringstream ssCameraZoom;
    ssCameraZoom << "Camera Zoom: " << std::fixed << std::setprecision(1) << camera.GetCameraZoom();

    std::string strPlayerPos{};
    strPlayerPos = strPlayerPos + "X: " + std::to_string((int)player.pos.x) + " Y: " + std::to_string((int)player.pos.y);

    DrawText(strPlayerPos.c_str(), 15, GetScreenHeight() - 30, 24, WHITE);
    DrawText(TextFormat("Tiles Drawn: %d", (int)RayTiled::GetTileDrawStats()), 5, 25, 20, WHITE);
    DrawText(ssCameraZoom.str().c_str(), GetScreenWidth() - 140, GetScreenHeight() - 30, 16, WHITE);
}

static bool OnTouch(const Player& player, float targetPosX)
{
    if (player.facing == 1.0f && player.pos.x < targetPosX) return 1;
    else if (player.facing == -1.0f && player.pos.x > targetPosX) return 1;

    return 0;
}

void GameplayScene::CollisionChecking()
{
    std::vector<RayTiled::CollisionRecord> collisions;
    if (GetCollisions(map, player.rec, collisions))
    {
        player.pos = player.lastPos;
    }

    switch (state)
    {
        case MapState::STATE_WAITING:
        {
            if (IsKeyPressed(KEY_B) && !enteringHouse)
            {
                dataLoaded.store(0, std::memory_order_relaxed);
                dataProgress.store(0, std::memory_order_relaxed);

                loaderThread = std::thread(LoadDataThread);
                TraceLog(LOG_INFO, "Loading thread initialized successfully");

                state = MapState::STATE_LOADING_HOME;
                onSwitch = 1;
            }
            else if (IsKeyPressed(KEY_B) && enteringHouse)
            {
                dataLoaded.store(0, std::memory_order_relaxed);
                dataProgress.store(0, std::memory_order_relaxed);

                loaderThread = std::thread(LoadDataThread);
                TraceLog(LOG_INFO, "Loading thread initialized successfully");

                state = MapState::STATE_LOADING_WORLD;
                onSwitch = 1;
            }
        } break;

        case MapState::STATE_LOADING_HOME:
        {
            RayTiled::UnloadTileMap(map, 1);

            framesCounter++;
            if (dataLoaded.load(std::memory_order_relaxed))
            {
                framesCounter = 0;
                if (loaderThread.joinable())
                {
                    loaderThread.join();
                    TraceLog(LOG_INFO, "Loading thread terminated successfully");
                }

                dataLoaded.store(0, std::memory_order_relaxed);
                dataProgress.store(0, std::memory_order_relaxed);

                isCameraScrollable = 0;
                InitHouseMap();
                player.pos = Vector2{ 200.0f, 200.0f };
                camera.zoom = 2.0f;
                enteringHouse = 1;

                SetCurrBGM("harp");

                state = MapState::STATE_WAITING;
                onSwitch = 0;
            }
        } break;

        case MapState::STATE_LOADING_WORLD:
        {
            RayTiled::UnloadTileMap(map, 1);

            framesCounter++;
            if (dataLoaded.load(std::memory_order_relaxed))
            {
                framesCounter = 0;
                if (loaderThread.joinable())
                {
                    loaderThread.join();
                    TraceLog(LOG_INFO, "Loading thread terminated successfully");
                }

                dataLoaded.store(0, std::memory_order_relaxed);
                dataProgress.store(0, std::memory_order_relaxed);

                isCameraScrollable = 1;
                InitWorldMap();
                player.pos = Vector2{ 484.0f, 650.0f };
                camera.zoom = 2.0f;
                enteringHouse = 0;

                SetCurrBGM("bird");

                state = MapState::STATE_WAITING;
                onSwitch = 0;
            }
        } break;
    }
}

static Rectangle GetRecBottomSide(const Rectangle& rec)
{
    float fullArea = rec.width*rec.height;
    float bottomArea = fullArea - fullArea*0.85f;

    float y = rec.y + (fullArea - bottomArea)/rec.width;

    return { rec.x, y, rec.width, bottomArea/rec.width };
}

static void DrawLoadingScreen()
{
    DrawRectangle(0, 0, 512, 512, DARKBLUE); // Draw the background

    static const char* text = "Loading...";  // Static text for the loading screen
    static float alpha = 1.0f;               // Initial alpha transparency
    static bool fadeOut = 1;                 // Direction of fading
    static float blinkSpeed = 1.0f;          // Speed of blinking (lower value = faster blink)

    Vector2 textPos {(float)GetScreenWidth()/2.0f - 80.0f, (float)GetScreenHeight() - 300.0f}; // Centered position

    // Update alpha transparency
    if (fadeOut) alpha -= blinkSpeed*GetFrameTime();
    else alpha += blinkSpeed*GetFrameTime();

    // Clamp the alpha between 0 and 1
    if (alpha <= 0.0f) { alpha = 0.0f; fadeOut = 0; }
    else if (alpha >= 1.0f) { alpha = 1.0f; fadeOut = 1; }

    // Draw the blinking loading text
    DrawTextEx(GetFontDefault(), text, textPos, 40.0f, 2, Fade(BLACK, alpha));
}

static void DrawGrid(int screenWidth, int screenHeight, int cellSize)
{
    cellSize = cellSize*4;

    // Draw vertical lines
    for (int x = 0; x <= screenWidth; x += cellSize)
    {
        DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
    }

    // Draw horizontal lines
    for (int y = 0; y <= screenHeight; y += cellSize)
    {
        DrawLine(0, y, screenWidth, y, LIGHTGRAY);
    }
}

static void LoadDataThread()
{
    auto start = std::chrono::steady_clock::now();

    while (1)
    {
        auto now = std::chrono::steady_clock::now();
        int elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        if (elapsedMs >= 5000) break;

        dataProgress.store(elapsedMs/10, std::memory_order_relaxed);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    dataLoaded.store(1, std::memory_order_relaxed);
}