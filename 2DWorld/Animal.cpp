/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2023 Wildan Wijanarko (@wildan9)
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

#include "Animal.h"

bool isBatDeath = 0;

void Bat::Update()
{
	if (_isDeath) _pCurrentTexture = &_textures[1];
	else _pCurrentTexture = &_textures[0];

	_rectangle = { _position.x, _position.y, 28.0f, 28.0f };

	_position.x += _speed.x;
	_position.y += _speed.y;

	if (_isDeath && _fallTimer >= 0) _fallTimer -= GetFrameTime();

	if (_fallTimer < 0)
	{
		if (GetCurrentFrame() == _numFrames - 1)
		{
			_animate = 0;
		}
	}

	isBatDeath = _isDeath;

	if (_position.x >= _flyRadius || _position.x <= 0)
	{
		_speed.x *= -1.0f;
		_facing *= -1.0f;
	}
	if (_position.y >= _flyRadius || _position.y <= 0) _speed.y *= -1.0f;

	Animate(_frameSpeed, _numFrames, 2.0f, _animate);
}

bool IsBatDeath()
{
	return isBatDeath;
}