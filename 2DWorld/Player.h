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

#include "Sprite.h"
#include "rlTileMap/ray_tilemap.h"

class Player : public RayTiled::TileLayer::Drawable
{
private:
    std::unique_ptr<Sprite> sprite;
    bool isWalk;
    float rad;

    int frameCol;
    int frameRow;
    int selectedRow;
    int totalRows;
    float frameFacing;
    float frameScale;
    float frameSpeed;
 
public:
    Rectangle rec;
    Vector2 pos;
    Vector2 lastPos;
    Vector2 dir;
    float facing;

    void Start();
    void Update();
    void Draw() const;
    
    float GetY() override { return pos.y - rad; }

    ~Player();
};