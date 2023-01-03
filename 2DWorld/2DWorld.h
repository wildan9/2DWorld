/* by Wildan R. */

#pragma once

#include <ctime>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "Screen.h"
#include "Math2D.h"
#include "Map.h"
#include "Player.h"
#include "Prop.cpp"
#include "Animal.cpp"

#define RAYGUI_IMPLEMENTATION
#include "extras/raygui.h"

struct Audio
{
    bool  muted         = 0;
    float masterVolume  = 0.2f;
    float currentVolume = 0.2f;
};

class Timer
{
public:
    void Start(float lifeTime);
    void Update();
    bool Done() const;

private:
    float _lifeTime = 0.0f;
};

struct StaticObject
{
    Map     map;
    Prop    prop;
    Player  player;
    Animals animals;
};