/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023-2024 Wildan R Wijanarko
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

#include <cmath>
#include <string>

namespace math
{
	struct vec2;

	inline vec2 to_vec2(const Vector2& vec);
	inline Vector2 rl_vec(const vec2& vec);

	struct vec2
	{
		vec2() : x{ 0.0f }, y{ 0.0f } {}
		vec2(float x) : x{ x }, y{ 0.0f } {}
		vec2(float x, float y) : x{ x }, y{ y } {}

		inline std::string to_string() const
		{
			return { "x: " + std::to_string((int)x) + "  y: " + std::to_string((int)y) };
		}

		inline vec2 scale(float scale) const
		{
			return { x * scale, y * scale };
		}

		inline vec2 normalize() const
		{
			float len = length();
			if (len != 0.0f)
			{
				return { x / len, y / len };
			}
			else
			{
				return vec2();
			}
		}

		inline float length() const
		{
			return std::sqrt(x * x + y * y);
		}
		
		float x;
		float y;
	};

	inline vec2 operator +(const vec2& a, const vec2& b)
	{
		return { a.x + b.x, a.y + b.y };
	}

	inline vec2 operator -(const vec2& a, const vec2& b)
	{
		return { a.x - b.x, a.y - b.y };
	}

	inline vec2 to_vec2(const Vector2& vec)
	{
		return { vec.x, vec.y };
	}

	inline Vector2 rl_vec(const vec2& vec)
	{
		return { vec.x, vec.y };
	}

	struct trans2d
	{
		trans2d() : pos{ vec2() }, scl{ 0.0f }, rot{ 0.0f } {}
		trans2d(vec2 pos, float scl, float rot) : pos{ pos }, scl{ scl }, rot{ rot } {}

		vec2 pos;
		float rot;
		float scl;
	};

	struct rec
	{
		float x;
		float y;
		float w;
		float h;

		inline bool check_collision(const rec& r) const
		{
			return ((x < (r.x + r.w) && (x + w) > r.x) && (y < (r.y + r.h) && (y + h) > r.y));
		}

		inline bool check_collision(const vec2& p) const
		{
			return ((p.x >= x) && (p.x < (x + w)) && (p.y >= y) && (p.y < (y + h)));
		}
	};

	inline Rectangle rl_rec(const rec& r)
	{
		return { r.x, r.y, r.w, r.h };
	}

	inline vec2 vec2_input_dir()
	{
		vec2 dir{ 0 };

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dir.x -= 1.0f;
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dir.x += 1.0f;
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dir.y -= 1.0f;
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dir.y += 1.0f;

		return dir;
	}
}