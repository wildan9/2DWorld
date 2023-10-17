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

bool showGrid = 0, worldCollision = 1, createBats = 0;
float batsCleaner = 20.0f;

Rectangle GetRecBottomSide(const Rectangle& rec);
std::vector<std::shared_ptr<Bat>> CreateBatsVec(unsigned n);
bool IsSorted(const std::vector<std::shared_ptr<GameObject>>& vec);
void Merge(std::vector<std::shared_ptr<GameObject>>& vec, int left, int mid, int right);
void MergeSort(std::vector<std::shared_ptr<GameObject>>& vec, int left, int right);
bool OnTouch(const Player& player, float targetPosX);

void GameplayScene::Start()
{
    LoadResources();
    _rendererMap->Setup();

    MergeSort(_gameObjectsVec, 0, _gameObjectsVec.size() - 1);
}

void GameplayScene::Update()
{
    Rectangle mapRec = { 10.0f, 10.0f, 61.5f * 61.5f / 2.0f, 61.5f * 61.5f / 2.0f };

    _camera.Update(_player->GetPosition(), mapRec, GetScreenWidth(), GetScreenHeight(), 1);
    _animals->Update(_player->GetSpeed(), _player->GetDirection(), _player->GetFacing());

    // Update the game objects
    for (const auto& gameObject : _gameObjectsVec)
    {
        gameObject->isInView = 0;
        if (CheckCollisionRecs(_camera.GetRectangle(), gameObject->GetRectangle()))
        {
            gameObject->Update();
            gameObject->isInView = 1;
        }
        // Updated each frame
        else if (gameObject->name == "Crocodile" || gameObject->name == "Chicken")
        {
            gameObject->Update();
        }
    }

    if (!IsSorted(_gameObjectsVec))
    {
        MergeSort(_gameObjectsVec, 0, _gameObjectsVec.size() - 1);
    }
    
    if (_batsLifetime >= 0)
    {
        _batsLifetime -= GetFrameTime();
    }
 
    if (!_batsVec.empty())
    {
        for (auto& bat : _batsVec)
        {
            bat->isInView = 0;
            if (CheckCollisionRecs(_camera.GetRectangle(), bat->GetRectangle()))
            {
                bat->isInView = 1;
            }
            bat->Update();

            if (_batsLifetime < 0 && !bat->IsDead() && !bat->healed)
            {
                bat->Death();
            }
            if (_batsLifetime < 0 && bat->IsDead())
            {
                if (bat->isOnTriger == 1)
                {
                    bat->Heal();
                }
            }
        }
    }

    if (createBats == 1)
    {
        _batsVec = CreateBatsVec(5);
    }

    if (batsCleaner >= 0 && _batsLifetime < 0.0f)
    {
        batsCleaner -= GetFrameTime();
    }

    if (!_batsVec.empty() && batsCleaner < 0)
    {
        _batsVec.clear();

        batsCleaner   = 20.0f;
        _batsLifetime = 20.0f;
    }

    // Our debug button
    if (IsKeyPressed(KEY_H))
    {
        showGrid = !showGrid;
    }
}

void GameplayScene::LoadResources()
{
    _player  = std::make_shared<Player>();
    _animals = std::make_shared<Animals>();

    auto house1 = std::make_shared<House>();
    auto house2 = std::make_shared<House>();
    auto house3 = std::make_shared<House>();
    auto house4 = std::make_shared<House>();

    auto ark = std::make_shared<Ark>();

    _gameObjectsVec =
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
        house4,

        // Ark
        ark

        // TODO: Add NPC?
    };

    RLReadTileMap("resources/maps/empire/wildan_empire.tmx", _tileMap);

    for (std::map<int, RLTileSheet>::iterator itr = _tileMap.Sheets.begin(); itr != _tileMap.Sheets.end(); itr++)
    {
        itr->second.SheetSource = "resources/" + itr->second.SheetSource;
    }

    _rendererMap = std::make_unique<RLTileRenderer>(_tileMap);

    _animals->Start(_gameObjectsVec);

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

    if (!_batsVec.empty())
    {
        if (IsBatDead())
        {
            for (const auto& bat : _batsVec)
            {
                if (bat->isInView)
                {
                    bat->Draw();
                }
            }
        }
    }

    // Draw the game objects in sorted order
    for (const auto& gameObject : _gameObjectsVec)
    {
        if (gameObject->isInView)
        {
            gameObject->Draw();

            if (showGrid)
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

    if (!_batsVec.empty())
    {
        if (!IsBatDead())
        {
            for (const auto& bat : _batsVec)
            {
                if (bat->isInView)
                {
                    bat->Draw();
                }
            }
        }
    }

    _camera.EndMode();
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
        if (leftVec[i]->GetZ() <= rightVec[j]->GetZ()) vec[k++] = leftVec[i++];
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

bool OnTouch(const Player& player, float targetPosX)
{
    if (player.GetFacing() == 1.0f && player.GetPosition().x < targetPosX) return 1;
    else if (player.GetFacing() == -1.0f && player.GetPosition().x > targetPosX) return 1;

    return 0;
}

void* GameplayScene::CollisionChecking(const std::atomic<bool>& collisionThreadRunning)
{
    while (collisionThreadRunning)
    {
        if (worldCollision)
        {
            // Lock the mutex to protect shared data?
            std::lock_guard<std::mutex> lock(_collisionMutex);

            for (auto& gameObject : _gameObjectsVec)
            {
                if (gameObject->isInView)
                {
                    gameObject->isOnTriger = 0;
                    if (gameObject->name == "Rhino")
                    {
                        if (CheckCollisionRecs(_player->GetRectangle(), gameObject->GetRectangle()) &&
                            IsKeyDown(KEY_ENTER) && Vector2Length(_player->GetDirection()) > 0.0f)
                        {
                            gameObject->isOnTriger = 1;
                        }
                        else if (CheckCollisionRecs(_player->GetRectangle(), GetRecBottomSide(gameObject->GetRectangle())))
                        {
                            _player->Stop();
                        }
                    }
                    else if (gameObject->name == "Crocodile")
                    {
                        if (CheckCollisionRecs(_player->GetRectangle(), gameObject->GetRectangle()) &&
                            _player->IsPunch() && OnTouch(*_player, gameObject->GetPosition().x))
                        {
                            gameObject->isOnTriger = 1;
                        }
                    }
                    else if (gameObject->name == "House")
                    {
                        if (CheckCollisionRecs(_player->GetRectangle(), GetRecBottomSide(gameObject->GetRectangle())))
                        {
                            gameObject->isOnTriger = 1;
                        }
                        const Rectangle houseBounds =
                        {
                            gameObject->GetRectangle().x,
                            gameObject->GetRectangle().y,
                            GetRecBottomSide(gameObject->GetRectangle()).width,
                            gameObject->GetRectangle().height - GetRecBottomSide(gameObject->GetRectangle()).height,
                        };
                        if (CheckCollisionRecs(_player->GetRectangle(), houseBounds))
                        {
                            _player->Stop();
                        }
                    }
                    else if (gameObject->name == "Ark")
                    {
                        if (CheckCollisionRecs(_player->GetRectangle(), GetRecBottomSide(gameObject->GetRectangle())))
                        {
                            gameObject->isOnTriger = 1;
                        }
                        const Rectangle arkBounds =
                        {
                            gameObject->GetRectangle().x,
                            gameObject->GetRectangle().y,
                            GetRecBottomSide(gameObject->GetRectangle()).width - 10.0f,
                            gameObject->GetRectangle().height - GetRecBottomSide(gameObject->GetRectangle()).height,
                        };
                        if (CheckCollisionRecs(_player->GetRectangle(), arkBounds))
                        {
                            _player->Stop();
                        }
                        
                        createBats = 0;
                        if (_batsVec.empty())
                        {
                            if (CheckCollisionRecs(_player->GetRectangle(), gameObject->GetRectangle()) && _player->IsPunch())
                            {
                                createBats = 1;
                            }
                        }
                    }
                }
            }

            for (auto& bat : _batsVec)
            {
                if (bat->isInView)
                {
                    bat->isOnTriger = 0;
                    if (CheckCollisionRecs(_player->GetRectangle(), bat->GetRectangle()) && IsKeyDown(KEY_ENTER))
                    {
                        bat->isOnTriger = 1;
                    }
                }
            }
        }

        // Sleep for a short duration to control the update rate of collision checking
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return nullptr;
}

bool IsSorted(const std::vector<std::shared_ptr<GameObject>>& vec)
{
    for (int i = 1; i < vec.size(); i++)
    {
        if (vec[i]->GetZ() < vec[i - 1]->GetZ()) 
        {
            return 0;
        }
    }

    return 1;
}

inline Rectangle GetRecBottomSide(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - (fullArea * 0.85f);

    float y = rec.y + ((fullArea - bottomArea) / rec.width);

    return { rec.x, y, rec.width, bottomArea / rec.width };
}

std::vector<std::shared_ptr<Bat>> CreateBatsVec(unsigned n)
{
    std::vector<std::shared_ptr<Bat>> batsVec;

    for (unsigned i = 0; i < n; i++)
    {
        batsVec.push_back(std::make_shared<Bat>());
    }

    return batsVec;
}