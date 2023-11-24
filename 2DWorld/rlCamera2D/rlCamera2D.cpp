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

#include "RLCamera2D.h"

Rectangle GetRecX1(const Rectangle& rec);
Rectangle GetRecX2(const Rectangle& rec);
Rectangle GetRecY1(const Rectangle& rec);
Rectangle GetRecY2(const Rectangle& rec);

Vector2 RLCamera2D::GetMouseDirection()
{
    Vector2 dir = {};
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, *this);

    if (CheckCollisionPointRec(worldMousePos, GetRecY1(_rectangle)))
    {
        dir.x += _cameraSpeed;
    }
    if (CheckCollisionPointRec(worldMousePos, GetRecY2(_rectangle)))
    {
        dir.x -= _cameraSpeed;
    }
    if (CheckCollisionPointRec(worldMousePos, GetRecX1(_rectangle)))
    {
        dir.y += _cameraSpeed;
    }
    if (CheckCollisionPointRec(worldMousePos, GetRecX2(_rectangle)))
    {
        dir.y -= _cameraSpeed;
    }

    return dir;
}

void RLCamera2D::Update(const Vector2& playerPos, const Rectangle& mapRec, int screenWidth, int screenHeight, bool scrollable)
{
    offset = { screenWidth / 2.0f, screenHeight / 2.0f };

    _freeMode = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);

    if (_freeMode) target = target + GetMouseDirection();
    else target = playerPos;

    const Rectangle& rec = mapRec;
    
    float minX = rec.width, minY = rec.height, maxX = 0, maxY = 0;

    minX = std::fminf(rec.x, minX);
    maxX = std::fmaxf(rec.x + rec.width, maxX);
    minY = std::fminf(rec.y, minY);
    maxY = std::fmaxf(rec.y + rec.height, maxY);

    Vector2 max = GetWorldToScreen2D({ maxX, maxY }, *this);
    Vector2 min = GetWorldToScreen2D({ minX, minY }, *this);

    if (max.x < screenWidth) offset.x = screenWidth - (max.x - screenWidth / 2);
    if (max.y < screenHeight) offset.y = screenHeight - (max.y - screenHeight / 2);
    if (min.x > 0) offset.x = screenWidth / 2 - min.x;
    if (min.y > 0) offset.y = screenHeight / 2 - min.y;

    if (scrollable)
    {
        if ((GetMouseWheelMove() > 0.0f) && zoom < 2.0f) zoom += 0.1f;
        if ((GetMouseWheelMove() < 0.0f) && zoom > 1.0f) zoom -= 0.1f;
    }

    _rectangle.x = target.x - offset.x / zoom;
    _rectangle.y = target.y - offset.y / zoom;

    _rectangle.width  = screenWidth / zoom;
    _rectangle.height = screenHeight / zoom;
}

inline Rectangle GetRecX1(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - (fullArea * 0.9f);

    float y = rec.y + (fullArea - bottomArea) / rec.width;

    return { rec.x, y, rec.width, bottomArea / rec.width };
}

inline Rectangle GetRecX2(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float bottomArea = fullArea - (fullArea * 0.9f);

    return { rec.x, rec.y, rec.width, bottomArea / rec.width };
}

inline Rectangle GetRecY1(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float rightArea = fullArea - (fullArea * 0.9f);

    float x = rec.x + (fullArea - rightArea) / rec.height;

    return { x, rec.y, rightArea / rec.height, rec.height };
}

inline Rectangle GetRecY2(const Rectangle& rec)
{
    float fullArea = rec.width * rec.height;
    float leftArea = fullArea * 0.1f;

    float x = rec.x;
    float width = leftArea / rec.height;

    return { x, rec.y, width, rec.height };
}