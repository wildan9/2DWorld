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

bool showGrid = 0, worldCollision = 1, enteringHouse = 0, onSwitch = 0, isCameraScrollable = 1;

static Rectangle GetRecBottomSide(const Rectangle& rec);
static void DrawGrid(int screenWidth, int screenHeight, int cellSize);
static bool OnTouch(const Player& player, float targetPosX);
static void DrawLoadingScreen();

RayTiled::TileMap map;

RayTiled::UserLayer* testUserLayer = nullptr;

RayTiled::TileLayer* objectTileLayer = nullptr;

Rectangle houseDoor{ 484.0f, 625.0f, 9, 9 };

Player player;

Timer mapSwitchTimer;

void DrawObjectLayerItem(RayTiled::TileLayer& layer, RayTiled::TileLayer::Drawable& drawable, float startX, float endX)
{
    DrawRectangleLines(houseDoor.x, houseDoor.y, houseDoor.width, houseDoor.height, RED);
    player.Draw();
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

    testUserLayer = RayTiled::InsertTileMapLayer<RayTiled::UserLayer>(map, map.Layers.back()->LayerId);

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

void InitHouseMap()
{
    RayTiled::UnloadTileMap(map, 1);
    RayTiled::LoadTileMap("resources/house.tmx", map);

    testUserLayer = RayTiled::InsertTileMapLayer<RayTiled::UserLayer>(map, map.Layers.back()->LayerId);

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
    mapRec = (enteringHouse) ? Rectangle{ 10.0f, 10.0f, 28.5f * 28.5f / 2.0f, 24.8f * 24.8f / 2.0f } 
    : Rectangle{ 10.0f, 10.0f, 51.5f * 51.5f / 2.0f, 51.5f * 51.5f / 2.0f };

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

    if (enteringHouse) houseDoor = Rectangle{ 100.0f, 300.0f, 9, 9 };
    else houseDoor = Rectangle{ 484.0f, 625.0f, 9, 9 };

    CollisionChecking();
}

void GameplayScene::LoadResources()
{
    InitWorldMap();

    player.Start();

    for (int i = 0; i < 10; i++)
    {
        bats[i] = CreateBat(Vector2{ 100.0f + i * 14 + GetRandomValue(2, 6), 100.0f + i * 12 + GetRandomValue(3, 5) });
    }
}

void GameplayScene::FreeResources()
{
    for (auto& bat : bats)
    {
        DeleteBat(bat);
    }

    RayTiled::UnloadTileMap(map, 1);
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
        }
    camera.EndMode();

    if (onSwitch)
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

    if (CheckCollisionRecs(houseDoor, player.rec) && !enteringHouse && !onSwitch)
    {
        onSwitch = 1;
        isCameraScrollable = 0;
        InitHouseMap();
        player.pos = Vector2{ 200.0f, 200.0f };
        camera.zoom = 2.0f;
        enteringHouse = 1;

        SetCurrBGM("harp");

        StartTimer(mapSwitchTimer, 2.5f);
    }

    UpdateTimer(mapSwitchTimer);

    if (onSwitch && IsTimerDone(mapSwitchTimer))
    {
        onSwitch = 0;
    }

    if (CheckCollisionRecs(houseDoor, player.rec) && enteringHouse && !onSwitch)
    {
        onSwitch = 1;
        isCameraScrollable = 1;
        InitWorldMap();
        player.pos = Vector2{ 484.0f, 650.0f };
        camera.zoom = 2.0f;
        enteringHouse = 0;

        SetCurrBGM("bird");

        StartTimer(mapSwitchTimer, 2.5f);
    }
}

static Rectangle GetRecBottomSide(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - fullArea * 0.85f;

    float y = rec.y + (fullArea - bottomArea) / rec.width;

    return { rec.x, y, rec.width, bottomArea / rec.width };
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
    else alpha += blinkSpeed * GetFrameTime();

    // Clamp the alpha between 0 and 1
    if (alpha <= 0.0f) { alpha = 0.0f; fadeOut = 0; }
    else if (alpha >= 1.0f) { alpha = 1.0f; fadeOut = 1; }

    // Draw the blinking loading text
    DrawTextEx(GetFontDefault(), text, textPos, 40.0f, 2, Fade(BLACK, alpha));
}

static void DrawGrid(int screenWidth, int screenHeight, int cellSize)
{
    cellSize = cellSize * 4;

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