/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2023 Wildan Wijanarko (@wildan9)
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

#include "2DWorld.h"

// Define an atomic flag to control the collision checking thread
std::atomic<bool> collisionThreadRunning(1);

RLCamera2D* pCamera = nullptr;

std::shared_ptr<Scene> pCurrentScene = nullptr;

bool isDebug = 0, worldCollision = 0;

void CollisionChecking(const std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameObject>>& gameObjects);

bool OnTouch(const Player& player, float targetPosX);
void Merge(std::vector<std::shared_ptr<GameObject>>& vec, int left, int mid, int right);
void MergeSort(std::vector<std::shared_ptr<GameObject>>& vec, int left, int right);

int main()
{
    InitWindow(screenWidth, screenHeight, "2DWorld");

    pCurrentScene = std::make_shared<GameplayScene>();

    //RLTileMap tileMap;

    //Animals animals;

    //auto player = std::make_shared<Player>();

    //auto house1 = std::make_shared<House>();
    //auto house2 = std::make_shared<House>();
    //auto house3 = std::make_shared<House>();
    //auto house4 = std::make_shared<House>();

    //std::vector<std::shared_ptr<GameObject>> gameObjectsVec
    //{ 
    //    // Player
    //    player, 
    //    
    //    // Animals
    //    animals.chicken,
    //    animals.horse,
    //    animals.crocodile,

    //    // House
    //    house1,
    //    house2,
    //    house3,
    //    house4

    //    // TODO: Add NPC?
    //};

    //animals.Start(gameObjectsVec);

    //house2->SetPosition({ 1340.0f, 458.0f, });
    //house3->SetPosition({ 1730.0f, 170.0f, });
    //house4->SetPosition({ 1730.0f, 458.0f, });

    //RLCamera2D camera;

    //pCamera = &camera;

    //Rectangle mapRec = { 10.0f, 10.0f, 61.5f * 61.5f / 2.0f, 61.5f * 61.5f / 2.0f };

    SetTargetFPS(60);

    // Sort the game objects based on their Z-component using merge sort
    // MergeSort(gameObjectsVec, 0, gameObjectsVec.size() - 1);

    // Create a thread for collision checking
    // std::thread collisionThread(CollisionChecking, player, gameObjectsVec);

    pCurrentScene->Start();

    while (!WindowShouldClose())
    {
        pCurrentScene->Update();

        BeginDrawing();
        ClearBackground(WHITE);
        
        pCurrentScene->Draw();

        EndDrawing();
    }

    // Signal the collision checking thread to stop
    //collisionThreadRunning = 0;

    // Wait for the collision checking thread to finish
    ///collisionThread.join();

    CloseWindow();

    return 0;
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

void CollisionChecking(const std::shared_ptr<Player>& player, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    while (collisionThreadRunning)
    {
        if (worldCollision)
        {
            for (auto& gameObject : gameObjects)
            {
                if (gameObject->isInView)
                {
                    gameObject->isOnTriger = 0;
                    if (gameObject->name == "Rhino")
                    {
                        if (CheckCollisionRecs(player->GetRectangle(), gameObject->GetRectangle()) &&
                            IsKeyDown(KEY_ENTER) && Vector2Length(player->GetDirection()) > 0.0f)
                        {
                            gameObject->isOnTriger = 1;
                        }
                    }
                    else if (gameObject->name == "Crocodile")
                    {
                        if (CheckCollisionRecs(player->GetRectangle(), gameObject->GetRectangle()) &&
                            player->IsPunch() && OnTouch(*player, gameObject->GetPosition().x))
                        {
                            gameObject->isOnTriger = 1;
                        }
                    }
                }
            }
        }

        // Sleep for a short duration to control the update rate of collision checking
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
