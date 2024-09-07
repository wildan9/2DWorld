/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023-2024 Wildan R Wijanarko
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

bool showGrid = 0, worldCollision = 1, enteringHouse = 0, onSwitch = 0, isCameraScrollable = 1;

static Rectangle GetRecBottomSide(const Rectangle& rec);
static bool OnTouch(const Player& player, float targetPosX);

RayTiled::TileMap map;

RayTiled::UserLayer* testUserLayer = nullptr;

RayTiled::TileLayer* objectTileLayer = nullptr;

math::rec houseDoor{ 484.0f, 625.0f, 9, 9 };

Player player;

Timer mapSwitchTimer;

void DrawObjectLayerItem(RayTiled::TileLayer& layer, RayTiled::TileLayer::Drawable& drawable, float startX, float endX)
{
    DrawRectangleLines(houseDoor.x, houseDoor.y, houseDoor.w, houseDoor.h, RED);
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
    _mapRec = (enteringHouse) ? math::rec{ 10.0f, 10.0f, 28.5f * 28.5f / 2.0f, 24.8f * 24.8f / 2.0f } 
    : math::rec{ 10.0f, 10.0f, 51.5f * 51.5f / 2.0f, 51.5f * 51.5f / 2.0f };

    _camera.Update(player.pos, _mapRec, GetScreenWidth(), GetScreenHeight(), isCameraScrollable);

    // Our debug button
    if (IsKeyPressed(KEY_H))
    {
        showGrid = !showGrid;
    }

    player.Update();
    
    for (auto& bat : _bats)
    {
        bat.Update();
    }

    if (enteringHouse) houseDoor = math::rec{ 100.0f, 300.0f, 9, 9 };
    else houseDoor = math::rec{ 484.0f, 625.0f, 9, 9 };

    CollisionChecking();
}

void GameplayScene::LoadResources()
{
    InitWorldMap();

    player = CreatePlayer();

    for (int i = 0; i < 10; i++)
    {
        _bats[i] = CreateBat(math::vec2{ 100.0f + i * 14, 100.0f + i * 12 });
    }
}

void GameplayScene::FreeResources()
{
    for (auto& bat : _bats)
    {
        DeleteBat(bat);
    }
    
    DeletePlayer(player);
    RayTiled::UnloadTileMap(map, 1);
}

void GameplayScene::Draw()
{
    _camera.BeginMode();
        if (!onSwitch)
        {
            RayTiled::DrawTileMap(map, &_camera);
        }

        if (!enteringHouse)
        {
            for (const auto& bat : _bats)
            {
                bat.Draw();
            }
            DrawRectangleLinesEx(math::rl_rec(_mapRec), 12, BLACK);
        }
    _camera.EndMode();

    if (showGrid)
    {
        rlPushMatrix();
        rlTranslatef(0, 25 * 50, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 50);
        rlPopMatrix();
    }

    std::string strPlayerPos{};
    strPlayerPos = strPlayerPos + "X: " + std::to_string((int)player.pos.x) + " Y: " + std::to_string((int)player.pos.y);
    DrawText(strPlayerPos.c_str(), 15, GetScreenHeight() - 30, 24, WHITE);
    DrawText(TextFormat("Tiles Drawn: %d", (int)RayTiled::GetTileDrawStats()), 5, 25, 20, WHITE);
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
    if (GetCollisions(map, math::rl_rec(player.rec), collisions))
    {
        player.pos = player.lastPos;
    }

    if (houseDoor.check_collision(player.rec) && !enteringHouse && !onSwitch)
    {
        onSwitch = 1;
        isCameraScrollable = 0;
        InitHouseMap();
        player.pos = math::vec2{ 200.0f, 200.0f };
        _camera.zoom = 2.0f;
        enteringHouse = 1;

        SetCurrBGM("harp");

        StartTimer(mapSwitchTimer, 0.5f);
    }

    UpdateTimer(mapSwitchTimer);

    if (onSwitch && IsTimerDone(mapSwitchTimer))
    {
        onSwitch = 0;
    }

    if (houseDoor.check_collision(player.rec) && enteringHouse && !onSwitch)
    {
        onSwitch = 1;
        isCameraScrollable = 1;
        InitWorldMap();
        player.pos = math::vec2{ 484.0f, 650.0f };
        _camera.zoom = 2.0f;
        enteringHouse = 0;

        SetCurrBGM("bird");

        StartTimer(mapSwitchTimer, 0.5f);
    }
}

static Rectangle GetRecBottomSide(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - fullArea * 0.85f;

    float y = rec.y + (fullArea - bottomArea) / rec.width;

    return { rec.x, y, rec.width, bottomArea / rec.width };
}