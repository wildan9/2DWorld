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

#include <cmath>
#include <string>
#include "raylib.h"

// Addition operator for Vector2
inline Vector2 operator +(const Vector2& a, const Vector2& b)
{
	return { a.x + b.x, a.y + b.y };
}

// Subtraction operator for Vector2
inline Vector2 operator -(const Vector2& a, const Vector2& b)
{
	return { a.x - b.x, a.y - b.y };
}

// Addition operator for Vector3
inline Vector3 operator +(const Vector3& a, const Vector3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

// Subtraction operator for Vector3
inline Vector3 operator -(const Vector3& a, const Vector3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

// Convert a Vector2 to a string
inline std::string Vector2ToString(const Vector2& v)
{
	return { "x: " + std::to_string((int)v.x) + "  y: " + std::to_string((int)v.y) };
}

// Calculate the length (magnitude) of a Vector2
inline float Vector2Length(const Vector2& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

// Scale a Vector2 by a given factor
inline Vector2 Vector2Scale(const Vector2& v, float scale)
{
	return { v.x * scale, v.y * scale };
}

// Normalize a Vector2, converting it to a unit vector
inline Vector2 Vector2Normalize(const Vector2& v)
{
	float length = Vector2Length(v);
	
	if (length == 0.0f)
	{
		return { 0.0f, 0.0f }; // Avoid division by zero
	}

	return { v.x / length, v.y / length };
}

// Calculate the Euclidean distance between two Vector2 points
inline float Vector2Distance(const Vector2& p1, const Vector2& p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	return std::sqrt(dx * dx + dy * dy);
}

// Convert a Vector2 to a Vector3 by setting the z component to 0.
inline Vector3 Vector2ToVector3(const Vector2& v)
{
	return { v.x, v.y, 0.0f };
}

// Convert a Vector3 to a Vector2
inline Vector2 Vector3ToVector2(const Vector3& v)
{
	return { v.x, v.y };
}