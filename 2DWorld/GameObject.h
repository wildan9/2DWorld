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
#include "Texture2DVec.h"

#include <string>
#include <cassert>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	int id = 0;
	std::string name = "";
	bool isInView    = 0;
	bool isOnTrigger = 0;

public:
	inline Vector2 GetPosition() const
	{ 
		return { _position.x, _position.y }; 
	}

	inline float GetZ() const
	{
		return _position.z;
	}
	
	inline void SetPosition(Vector2 position) 
	{ 
		_position.x = position.x; 
		_position.y = position.y; 
	}

	inline Rectangle GetRectangle() const
	{
		return _rectangle;
	}

	inline float GetFacing() const
	{
		return _facing;
	}

	inline void SetFacing(float facing)
	{
		_facing = facing;
	}

	inline void Draw() const
	{
		assert(_pCurrentTexture != nullptr);

		DrawTexturePro(*_pCurrentTexture, _source, _dest, { 0.0f, 0.0f }, _rotation, WHITE);
	}

public:
	virtual void Start()  = 0;
	virtual void Update() = 0;

protected:
	void Animate(float frameSpeed, float numFrames, const bool animate = 1);
	
	inline int GetCurrentFrame()
	{
		return _currentFrame;
	}

protected:
	Rectangle _source = { 0, 0, 0, 0 };
	Rectangle _dest   = { 0, 0, 0, 0 };
	
	Rectangle _rectangle = { 0, 0, 0, 0 };
	Vector3	  _position  = { 0.0f, 0.0f, 0.0f }; // Need to use Z-component for layout position
	
	Texture2DVec _textures = {};
	Texture2D*   _pCurrentTexture = nullptr;

	float _rotation = 0.0f;
	float _facing	= 1.0f;
	float _scale    = 1.0f;

private:
	int _currentFrame = 0;
	int _frameCounter = 0;
};