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

#include <vector>
#include "raylib.h"

struct ObjectTexures : public std::vector<Texture2D>
{
	// Clear the vector of textures
	void Clear();

	// Destructor to clear the vector of textures
	~ObjectTexures();

	// Load a texture from a file
	int LoadTextureFile(const char* texture);
};

inline void ObjectTexures::Clear()
{
	for (const auto& texture : *this)
	{
		UnloadTexture(texture);
	}

	clear();
}

inline ObjectTexures::~ObjectTexures()
{
	Clear();
}

inline int ObjectTexures::LoadTextureFile(const char* texture)
{
	push_back(LoadTexture(texture));
	return int(size() - 1);
}
