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

bool showGrid = 0, worldCollision = 1;

Rectangle GetRecBottomSide(const Rectangle& rec);
void Merge(std::vector<std::shared_ptr<GameObject>>& vec, int left, int mid, int right);
void MergeSort(std::vector<std::shared_ptr<GameObject>>& vec, int left, int right);
//bool OnTouch(const Player& player, float targetPosX);

RayTiled::TileMap map;

RayTiled::UserLayer* testUserLayer = nullptr;

RayTiled::TileLayer* objectTileLayer = nullptr;

Player player;

void DrawObjectLayerItem(RayTiled::TileLayer& layer, RayTiled::TileLayer::Drawable& drawable, float startX, float endX)
{
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

void InitMap()
{
    RayTiled::LoadTileMap("resources/sample_map.tmx", map);

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
        static_cast<RayTiled::ObjectLayer*>(collisionlayer)->CheckForCollisions = true;
    }
}

void GameplayScene::Start()
{
    LoadResources();
    MergeSort(_gameObjectsVec, 0, _gameObjectsVec.size() - 1);
}

void GameplayScene::Update()
{
    // Sort the game objects
    MergeSort(_gameObjectsVec, 0, _gameObjectsVec.size() - 1);

    _mapRec = { 10.0f, 10.0f, 61.5f * 61.5f / 2.0f, 61.5f * 61.5f / 2.0f };

    _camera.Update(player.pos, _mapRec, GetScreenWidth(), GetScreenHeight(), 1);

    // Our debug button
    if (IsKeyPressed(KEY_H))
    {
        showGrid = !showGrid;
    }

    player.Update();
}

void GameplayScene::LoadResources()
{
    InitMap();

    player = CreatePlayer();

    auto house1 = std::make_shared<House>();
    auto house2 = std::make_shared<House>();
    auto house3 = std::make_shared<House>();
    auto house4 = std::make_shared<House>();

    auto ark = std::make_shared<Ark>();

    _gameObjectsVec =
    {
        // Player
        //player.object,

        // House
        house1,
        house2,
        house3,
        house4,

        // Ark
        ark

        // TODO: Add NPC?
    };

    house2->SetPos({ 1340.0f, 458.0f, });
    house3->SetPos({ 1730.0f, 170.0f, });
    house4->SetPos({ 1730.0f, 458.0f, });
}

void GameplayScene::FreeResources()
{
    DeletePlayer(player);
}

void GameplayScene::Draw()
{
    _camera.BeginMode();
        RayTiled::DrawTileMap(map, &_camera);
    _camera.EndMode();

    if (showGrid)
    {
        rlPushMatrix();
        rlTranslatef(0, 25 * 50, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 50);
        rlPopMatrix();
    }

    DrawText(TextFormat("Tiles Drawn: %d", (int)RayTiled::GetTileDrawStats()), 5, 25, 20, WHITE);
}

void Merge(std::vector<std::shared_ptr<GameObject>>& vec, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::shared_ptr<GameObject>> leftVec(n1);
    std::vector<std::shared_ptr<GameObject>> rightVec(n2);

    for (int i = 0; i < n1; i++) leftVec[i] = vec[left + i];
    for (int i = 0; i < n2; i++) rightVec[i] = vec[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (leftVec[i]->GetPos().y <= rightVec[j]->GetPos().y) vec[k++] = leftVec[i++];
        else vec[k++] = rightVec[j++];
    }

    while (i < n1) vec[k++] = leftVec[i++];
    while (j < n2) vec[k++] = rightVec[j++];
}

void MergeSort(std::vector<std::shared_ptr<GameObject>>& vec, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        MergeSort(vec, left, mid);
        MergeSort(vec, mid + 1, right);
        Merge(vec, left, mid, right);
    }
}

//bool OnTouch(const Player& player, float targetPosX)
//{
//    if (player.GetFacing() == 1.0f && player.GetPos().x < targetPosX) return 1;
//    else if (player.GetFacing() == -1.0f && player.GetPos().x > targetPosX) return 1;
//
//    return 0;
//}

void* GameplayScene::CollisionChecking(const std::atomic<bool>& collisionThreadRunning)
{
    while (collisionThreadRunning)
    {
        std::lock_guard<std::mutex> lock{ _collisionMutex };
        std::vector<RayTiled::CollisionRecord> collisions;
        if (GetCollisions(map, math::rl_rec(player.rec), collisions))
        {
            player.pos = player.lastPos;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }

    return nullptr;
}

inline Rectangle GetRecBottomSide(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - fullArea * 0.85f;

    float y = rec.y + (fullArea - bottomArea) / rec.width;

    return { rec.x, y, rec.width, bottomArea / rec.width };
}