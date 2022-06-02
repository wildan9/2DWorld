/* by Wildan R. */

#pragma once

#include "Player.h"
#include "Map.h"
#include "Animal.cpp"
#include "Prop.cpp"

struct GameObject
{
    Player wildan{ { 40.0f, 140.0f } };
    Map map;
    Prop prop;
    Animals animals;
    void PlayWalkSound();
    void CheckCollision();
    void Draw(float deltaTime);
};

inline void GameObject::PlayWalkSound()
{
    if (wildan.GetPosition().x < -15.0f || wildan.GetPosition().y < 0.0f ||
        wildan.GetPosition().x > ((float)map.GetDreamlandSize().width - 15.0f) * map.GetMapScale() &&
        wildan.GetPosition().x < (float)map.GetDreamlandSize().width + 1500.0f ||
        wildan.GetPosition().y > (float)map.GetDreamlandSize().height * map.GetMapScale()) wildan.OnWater();
    else wildan.OnLand();
}

inline void GameObject::CheckCollision()
{
    if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) && 
        wildan.IsPunch() && (wildan.GetFacing() == 1.0f && animals.crocodile.GetFacing() >= -1.0f &&
        wildan.GetPosition().x < animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
    else if (CheckCollisionRecs(wildan.GetCollision(), animals.crocodile.GetCollision()) &&
        wildan.IsPunch() && (wildan.GetFacing() == -1.0f && animals.crocodile.GetFacing() >= -1.0f &&
        wildan.GetPosition().x > animals.crocodile.GetPosition().x)) animals.crocodile.Hurt();
    else animals.crocodile.Walk();

    for (auto& rhino : animals.rhinos) if (CheckCollisionRecs(wildan.GetCollision(), rhino.GetCollision())) wildan.Stop();

    if (CheckCollisionRecs(wildan.GetCollision(), map.GetMapLine1()) ||
        CheckCollisionRecs(wildan.GetCollision(), map.GetMapLine2())) wildan.Stop();

    for (auto& invisibleFence : invisibleFences)
    {
        if (CheckCollisionRecs(wildan.GetCollision(), 
            Rectangle{ invisibleFence.x, invisibleFence.y, invisibleFence.width, invisibleFence.height })) wildan.Stop();
    }

    if (CheckCollisionRecs(wildan.GetCollision(), prop.naturalObj.GetBigStone1Coll())) wildan.SetPosition(Vector2D{ 3000.0f, 300.0f });

    if (CheckCollisionRecs(wildan.GetCollision(), prop.naturalObj.GetBigStone2Coll())) wildan.SetPosition(Vector2D{ 40.0f, 140.0f });
}

inline void GameObject::Draw(float deltaTime)
{
    map.Draw();
    wildan.Draw();
    prop.Draw();
    animals.Draw(deltaTime);
}