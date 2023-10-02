/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023 Wildan Wijanarko (@wildan9)
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

bool isDebug = 0;

void GameplayScene::Start()
{
    LoadResources();
    _rendererMap->Setup();
}

void GameplayScene::Update()
{
    Rectangle mapRec = { 10.0f, 10.0f, 61.5f * 61.5f / 2.0f, 61.5f * 61.5f / 2.0f };

    _camera.Update(_player->GetPosition(), mapRec, GetScreenWidth(), GetScreenHeight(), 1);
    _animals->Update(_player->GetSpeed(), _player->GetDirection(), _player->GetFacing());

    // Update the game objects
    for (const auto& gameObject : gameObjectsVec)
    {
        gameObject->isInView = 0;
        if (CheckCollisionRecs(_camera.GetRectangle(), gameObject->GetRectangle()))
        {
            gameObject->Update();
            gameObject->isInView = 1;
        }
        // Updated each frame
        else if (gameObject->name == "Crocodile" || gameObject->name == "Bat" || gameObject->name == "Chicken")
        {
            gameObject->Update();
        }
    }

    // Our debug button
    if (IsKeyPressed(KEY_H))
    {
        isDebug = !isDebug;
    }
}

void GameplayScene::LoadResources()
{
    _animals = std::make_shared<Animals>();

    _player = std::make_shared<Player>();

    auto house1 = std::make_shared<House>();
    auto house2 = std::make_shared<House>();
    auto house3 = std::make_shared<House>();
    auto house4 = std::make_shared<House>();

    gameObjectsVec =
    {
        // Player
        _player,

        // Animals
        _animals->chicken,
        _animals->horse,
        _animals->crocodile,

        // House
        house1,
        house2,
        house3,
        house4

        // TODO: Add NPC?
    };

    RLReadTileMap("resources/maps/empire/wildan_empire.tmx", _tileMap);

    for (std::map<int, RLTileSheet>::iterator itr = _tileMap.Sheets.begin(); itr != _tileMap.Sheets.end(); itr++)
    {
        itr->second.SheetSource = "resources/" + itr->second.SheetSource;
    }

    _rendererMap = std::make_unique< RLTileRenderer>(_tileMap);

    _animals->Start(gameObjectsVec);

    house2->SetPosition({ 1340.0f, 458.0f, });
    house3->SetPosition({ 1730.0f, 170.0f, });
    house4->SetPosition({ 1730.0f, 458.0f, });
}

void GameplayScene::FreeResources()
{

}

void GameplayScene::Draw()
{
    _camera.BeginMode();
    _rendererMap->Draw(_camera);

    // Draw the game objects in sorted order
    for (const auto& gameObject : gameObjectsVec)
    {
        if (gameObject->isInView)
        {
            gameObject->Draw();

            if (isDebug)
            {
                _rendererMap->DrawGrid(_player->GetRectangle());
                DrawRectangleLinesEx(gameObject->GetRectangle(), 1.2f, GREEN);

                if (gameObject->name != "")
                {
                    DrawText(
                        gameObject->name.c_str(),
                        (int)gameObject->GetPosition().x,
                        (int)gameObject->GetPosition().y,
                        12, WHITE
                    );
                }
                DrawText(
                    Vector2ToString(gameObject->GetPosition()).c_str(),
                    (int)gameObject->GetPosition().x,
                    (int)gameObject->GetPosition().y + 12,
                    14, WHITE
                );

                DrawRectangleLinesEx(_camera.GetRectangle(), 2.2f, RED);
            }
        }
    }

    _camera.EndMode();

    DrawFPS(0, 0);
}