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

#pragma once

#include "raylib.h"
#include "raymath.h"

typedef struct Trans2D
{
	Vector2 pos;
	float rot;
	float scl;
};

inline Vector2 Vector2InputDir()
{
	Vector2 dir{ 0 };

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dir.x -= 1.0f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dir.x += 1.0f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dir.y -= 1.0f;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dir.y += 1.0f;

	return Vector2Normalize(dir);
}