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

#pragma once

#include "raylib.h"
#include "VectorMath.h"

class RLCamera2D : public ::Camera2D
{
    Vector2 GetMouseDirection();

public:
    RLCamera2D() : ::Camera2D{ offset = { 0.0f, 0.0f }, target = { 0.0f, 0.0f }, rotation = 0.0f, zoom = 1.0f } {}
    
    void Update(const Vector2& playerPos, const Rectangle& mapRec, int screenWidth, int screenHeight, bool scrollable);

    inline ::Camera2D& BeginMode()
    {
        ::BeginMode2D(*this);
        return (*this);
    }

    inline ::Camera2D& EndMode()
    {
        ::EndMode2D();
        return (*this);
    }

    inline Rectangle GetRectangle() const
    {
        return _rectangle;
    }

private:
    Rectangle _rectangle = { 0, 0, 0, 0 };
    float _cameraSpeed = 5.5f;
    bool _freeMode = 0;
};