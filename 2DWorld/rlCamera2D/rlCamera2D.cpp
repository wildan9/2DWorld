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

#include "RLCamera2D.h"

math::rec GetRecX1(const math::rec& r);
math::rec GetRecX2(const math::rec& r);
math::rec GetRecY1(const math::rec& r);
math::rec GetRecY2(const math::rec& r);

math::vec2 RLCamera2D::GetDir() const
{
    math::vec2 dir{};
    math::vec2 worldMousePos(math::to_vec2(GetScreenToWorld2D(GetMousePosition(), *this)));

    if (GetRecY1(_rec).check_collision(worldMousePos))
    {
        dir.x += _cameraSpeed;
    }
    if (GetRecY2(_rec).check_collision(worldMousePos))
    {
        dir.x -= _cameraSpeed;
    }
    if (GetRecX1(_rec).check_collision(worldMousePos))
    {
        dir.y += _cameraSpeed;
    }
    if (GetRecX2(_rec).check_collision(worldMousePos))
    {
        dir.y -= _cameraSpeed;
    }

    return dir;
}

void RLCamera2D::Update(const math::vec2& playerPos, const math::rec& rec, int screenWidth, int screenHeight, bool scrollable)
{
    offset = { screenWidth / 2.0f, screenHeight / 2.0f };

    _freeMode = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);

    const math::vec2 cameraTarget = (_freeMode) ? math::to_vec2(target) + GetDir() : playerPos;

    target = math::rl_vec(cameraTarget);
    
    float minX = rec.w, minY = rec.h, maxX = 0, maxY = 0;

    minX = std::fminf(rec.x, minX);
    maxX = std::fmaxf(rec.x + rec.w, maxX);
    minY = std::fminf(rec.y, minY);
    maxY = std::fmaxf(rec.y + rec.h, maxY);

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

    _rec.x = target.x - offset.x / zoom;
    _rec.y = target.y - offset.y / zoom;

    _rec.w  = screenWidth / zoom;
    _rec.h = screenHeight / zoom;
}

inline math::rec GetRecX1(const math::rec& r)
{
    float fullArea = r.w * r.h;
    float bottomArea = fullArea - (fullArea * 0.9f);

    float y = r.y + (fullArea - bottomArea) / r.w;

    return { r.x, y, r.w, bottomArea / r.w };
}

inline math::rec GetRecX2(const math::rec& r)
{
    float fullArea = r.w * r.h;
    float bottomArea = fullArea - (fullArea * 0.9f);

    return { r.x, r.y, r.w, bottomArea / r.w };
}

inline math::rec GetRecY1(const math::rec& r)
{
    float fullArea = r.w * r.h;
    float rightArea = fullArea - (fullArea * 0.9f);

    float x = r.x + (fullArea - rightArea) / r.h;

    return { x, r.y, rightArea / r.h, r.h };
}

inline math::rec GetRecY2(const math::rec& r)
{
    float fullArea = r.w * r.h;
    float leftArea = fullArea * 0.1f;

    float x = r.x;
    float width = leftArea / r.h;

    return { x, r.y, width, r.h };
}