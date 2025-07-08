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

#include <iomanip>
#include <sstream>

enum class MapState {STATE_WAITING, STATE_LOADING_HOME, STATE_LOADING_WORLD};
MapState state = MapState::STATE_WAITING;

bool isShowGrid = 0, enteringHouse = 0, onSwitch = 0, isCameraScrollable = 1, isDrawRectangles = 1;
unsigned currentDrawFire = 0;

std::array<Rectangle, 4> fishingRecs {};

static void DrawGrid(int screenWidth, int screenHeight, int cellSize);
static bool OnTouch(const Player& player, float targetPosX);

RayTiled::TileMap map;
RayTiled::TileLayer* objectTileLayer = nullptr;

Rectangle houseDoor {484.0f, 625.0f, 9, 9};

Rectangle cameraRec {};

Player player;
Horse horse;
Frog frog;

Timer mapSwitchTimer;

typedef struct Lightning
{
    Vector2 pos;
    Rectangle rec;
    std::unique_ptr<Sprite> sprite;

    int frameCol;
    int frameRow;
    int selectedRow;

    float frameFacing;
    float frameScale;
    float frameSpeed;

    Sound sound;
    bool isShown;
    bool isDrawn;
    float timer;
    int numFrame;
};
Lightning lightning{};

struct Fire : public RayTiled::TileLayer::Drawable
{
    std::unique_ptr<Sprite> sprite;
    float rad;
    Timer timer;
    float colorTimer;
    bool moveNext;
    bool isDrawn;
    int selectedRow;
    Rectangle rec;

    float GetY() override { return rec.y - rad; }
};
std::array<Fire, 2> fires {};

Fire InitRedFire()
{
    Fire fire{};
    fire.rad = 10.0f;
    fire.moveNext = 0;
    fire.selectedRow = 0;
    fire.colorTimer = 5.0f;
    fire.rec = Rectangle{616.5f, 292.5f, 24, 24};
    StartTimer(fire.timer, 0.7f);
    fire.sprite = std::make_unique<Sprite>(Vector2{619.0f, 299.0f}, "resources/Spritesheet/fire4_64.png", 10, 6, 1.0f);

    return fire;
}

Fire InitBlueFire()
{
    Fire fire{};
    fire.rad = 10.0f;
    fire.moveNext = 0;
    fire.selectedRow = 0;
    fire.colorTimer = 5.0f;
    fire.rec = Rectangle{616.5f, 292.5f, 24, 24};
    StartTimer(fire.timer, 0.7f);
    fire.sprite = std::make_unique<Sprite>(Vector2{619.0f, 299.0f}, "resources/Spritesheet/fire7_64.png", 10, 6, 1.0f);

    return fire;
}

void UpdateFire(Fire& fire)
{
    if (CheckCollisionRecs(player.rec, fire.rec) && IsKeyDown(KEY_E))
    {
        UpdateTimer(fire.timer);
    }

    if (IsTimerDone(fire.timer))
    {
        fire.isDrawn = !fire.isDrawn;
        StartTimer(fire.timer, 0.7f);

        currentDrawFire = 0;
    }

    if (fire.isDrawn || currentDrawFire)
    {
        if (fire.sprite != nullptr)
        {
            fire.sprite->Update(Vector2{fire.rec.x - fire.rad - ((currentDrawFire) ? 7 : 3), fire.rec.y + ((currentDrawFire) ? 0 : 4) - fire.rad - 10}, ((currentDrawFire) ? 0.5f : 0.38f), 25.0f, fire.selectedRow, 1.0f, 10, 1);
        }
    }

    if (CheckCollisionRecs(cameraRec, Rectangle{fire.rec.x - 20, fire.rec.y - 20, 50.f, 50.0f}) && fire.isDrawn)
    {
        if (fire.sprite->GetCurrentFrame() >= 9 && !fire.moveNext)
        {
            fire.selectedRow = fire.selectedRow + 1;
            fire.moveNext = 1;
        }
        else if (fire.sprite->GetCurrentFrame() < 9 && fire.selectedRow == 6)
        {
            fire.selectedRow = 0;
            fire.moveNext = 0;
        }
        else if (fire.sprite->GetCurrentFrame() < 9)
        {
            fire.moveNext = 0;
        }

        if (fire.colorTimer >= 0.0f)
        {
            fire.colorTimer -= GetFrameTime();

            if (fire.colorTimer <= 0.0f)
            {
                fire.colorTimer = 5.0f;
                currentDrawFire = 1;
            }
        }
    }
    else if (!fire.isDrawn && currentDrawFire)
    {
        fire.isDrawn = !fire.isDrawn;
        StartTimer(fire.timer, 0.7f);
    }
}

void DrawFire(const Fire& fire)
{
    DrawCircleLines(fire.rec.x, fire.rec.y, fire.rad, RED);
    
    if (fire.sprite != nullptr && fire.isDrawn)
    {
        fire.sprite->Draw();
    }
}

Lightning InitLightning()
{
    Lightning lgh{};

    lgh.pos = Vector2{710.0f, 480.0f};
    lgh.rec = Rectangle{700.0f, 555.0f, 200.0f, 130.0f};

    lgh.frameCol = 16;
    lgh.frameRow = 4;
    lgh.selectedRow = 3;
    lgh.frameFacing = 1.0f;
    lgh.frameScale = 1.0f;
    lgh.frameSpeed = 8.0f;

    lgh.timer = 10.0f;
    lgh.sound = LoadSound("resources/sounds/thunder_explosion_hit.wav");
    lgh.sprite = std::make_unique<Sprite>(lgh.pos, "resources/Spritesheet/new_lightning.png", lgh.frameCol, lgh.frameRow, lgh.frameFacing);

    return lgh;
}

void UpdateLightning(Lightning& lightning)
{
    if (lightning.isShown)
    {
        lightning.isDrawn = 0;
        if (lightning.timer > 0.0f)
        {
            lightning.timer -= GetFrameTime();

            if (lightning.timer < 0.0f)
            {
                lightning.timer = 10.0f;
                lightning.selectedRow = GetRandomValue(0, 3);
                lightning.sprite->Reset();
                lightning.pos = Vector2{static_cast<float>(GetRandomValue(610.0f, 760.0f)), static_cast<float>(GetRandomValue(410.0f, 480.0f))};
                            
                if (lightning.selectedRow != 1)
                {
                    lightning.numFrame = 9;
                }
                else
                {
                    lightning.numFrame = 16;
                }
            }
        }

        if (lightning.timer < 1.0f)
        {
            lightning.isDrawn = 1;
            PlaySound(lightning.sound);
            lightning.sprite->Update(lightning.pos, lightning.frameScale, lightning.frameSpeed, lightning.selectedRow, lightning.frameFacing, lightning.numFrame, 0);
        }
    }

    lightning.isShown = CheckCollisionRecs(player.rec, lightning.rec);
}

void DrawLightning(const Lightning& lightning)
{
    if (lightning.isDrawn)
    {
        lightning.sprite->Draw();
    }
}

void DestroyLightning(Lightning& lightning)
{
    UnloadSound(lightning.sound);
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
    else if (&drawable == &frog)
    {
        frog.Draw();
    }
    else if (&drawable == &fires[currentDrawFire])
    {
        DrawFire(fires[currentDrawFire]);
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
            DrawCircleV(Vector2{object->Bounds.x, object->Bounds.y}, 8, GRAY);
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
        objectTileLayer->AddDrawable(&frog);
        objectTileLayer->AddDrawable(&fires[0]);
        objectTileLayer->AddDrawable(&fires[1]);
    }

    auto collisionlayer = RayTiled::FindLayer(map, "CollisionObjects");
    if (collisionlayer && collisionlayer->Type == RayTiled::TileLayerType::Object)
    {
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->DrawFunc = DrawCollisionLayer;
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->CheckForCollisions = 1;
    }

    fishingRecs[0] = Rectangle{505.f, 369.0f, 30.0f, 145.0f};
    fishingRecs[1] = Rectangle{680.f, 335.0f, 20.0f, 130.0f};
    fishingRecs[2] = Rectangle{637.f, 456.0f, 20.0f, 40.0f};
    fishingRecs[3] = Rectangle{580.f, 335.0f, 20.0f, 30.0f};
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
    mapRec = (enteringHouse) ? Rectangle{10.0f, 10.0f, 28.5f*28.5f/2.0f, 24.8f*24.8f/2.0f} 
    : Rectangle{10.0f, 10.0f, 51.5f*51.5f/2.0f, 51.5f*51.5f/2.0f};

    cameraRec = camera.GetRec();

    if (onSwitch)
    {
        camera.target = Vector2Zero();
    }
    else
    {
        camera.Update(player.pos, mapRec, GetScreenWidth(), GetScreenHeight(), isCameraScrollable);
    }

    if (IsKeyPressed(KEY_H))
    {
        isShowGrid = !isShowGrid;
    }

    if (IsKeyPressed(KEY_J))
    {
        isDrawRectangles = !isDrawRectangles;
    }

    player.Update();
    
    for (auto& bat : bats)
    {
        bat.Update();
    }

    if (!enteringHouse)
    {
        horse.Update();
        frog.Update(player.rec);
    }

    if (enteringHouse) houseDoor = Rectangle{100.0f, 300.0f, 9, 9};
    else houseDoor = Rectangle{484.0f, 625.0f, 9, 9};

    CollisionChecking();
    UpdateLightning(lightning);
    UpdateFire(fires[currentDrawFire]);
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

    frog.Start();

    isCameraScrollable = 1;
    InitWorldMap();
    player.pos = Vector2{484.0f, 650.0f};
    camera.zoom = 2.0f;
    enteringHouse = 0;

    lightning = InitLightning();
    
    fires[0] = InitBlueFire();
    fires[1] = InitRedFire();
}

void GameplayScene::FreeResources()
{
    RayTiled::UnloadTileMap(map, 1);
    DestroyLightning(lightning);
}

void GameplayScene::DrawHUD()
{
    if (isShowGrid)
    {
        DrawGrid(GetScreenWidth(), GetScreenHeight(), 15);
    }

    std::ostringstream ssCameraZoom;
    ssCameraZoom << "Camera Zoom: " << std::fixed << std::setprecision(1) << camera.GetCameraZoom();

    std::string strPlayerPos{};
    strPlayerPos = strPlayerPos + "X: " + std::to_string(static_cast<int>(player.pos.x)) + " Y: " + std::to_string(static_cast<int>(player.pos.y));

    DrawText(strPlayerPos.c_str(), 15, GetScreenHeight() - 30, 24, WHITE);
    DrawText(TextFormat("Tiles Drawn: %d", (int)RayTiled::GetTileDrawStats()), 5, 25, 20, WHITE);
    DrawText(ssCameraZoom.str().c_str(), GetScreenWidth() - 140, GetScreenHeight() - 30, 16, WHITE);
}

void DrawRecs()
{
    for (const auto& rec : fishingRecs)
    {
        DrawRectangleLinesEx(rec, 1.2f, RED);
    }
    DrawRectangleLinesEx(frog.rec, 0.8f, RED);
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

            DrawLightning(lightning);

            if (isDrawRectangles)
            {
                const Rectangle& rec = lightning.rec;
                DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, RED);
                DrawRectangleLinesEx(mapRec, 12, BLACK);
                DrawRecs();
            }
        }
    camera.EndMode();

    DrawHUD();
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
            if (CheckCollisionRecs(player.rec, houseDoor) && !enteringHouse)
            {
                state = MapState::STATE_LOADING_HOME;
                onSwitch = 1;
            }
            else if (CheckCollisionRecs(player.rec, houseDoor) && enteringHouse)
            {
                state = MapState::STATE_LOADING_WORLD;
                onSwitch = 1;
            }
        } break;

        case MapState::STATE_LOADING_HOME:
        {
            RayTiled::UnloadTileMap(map, 1);

            player.pos = Vector2{200.0f, 200.0f};
            isCameraScrollable = 0;
            InitHouseMap();
            camera.zoom = 2.0f;
            enteringHouse = 1;

            SetCurrBGM("harp");

            state = MapState::STATE_WAITING;
            onSwitch = 0;
        } break;

        case MapState::STATE_LOADING_WORLD:
        {
            RayTiled::UnloadTileMap(map, 1);

            player.pos = Vector2{484.0f, 650.0f};
            isCameraScrollable = 1;
            InitWorldMap();
            camera.zoom = 2.0f;
            enteringHouse = 0;

            SetCurrBGM("bird");

            state = MapState::STATE_WAITING;
            onSwitch = 0;
        } break;
    }
}

static void DrawGrid(int screenWidth, int screenHeight, int cellSize)
{
    cellSize = cellSize*3;

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
