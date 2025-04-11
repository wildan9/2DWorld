/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2025 Wildan R Wijanarko
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

#pragma once

#include "Model2D.h"
#include "rlTileMap/ray_tilemap.h"

struct Animal
{
    Model2D model;
    Vector2 speed;
    Vector2 pos;
    
    int animCurrFrame;
    float scl;
    float rad;
    float facing;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Draw() const = 0;
};

struct Bat : public Animal
{
    void Start() override;
    void Update() override;
    void Draw() const override;

    ~Bat();
};

struct Horse : public Animal, public RayTiled::TileLayer::Drawable
{
    void Start() override;
    void Update() override;
    void Draw() const override;

    float GetY() override { return pos.y - rad; }

    ~Horse();
};