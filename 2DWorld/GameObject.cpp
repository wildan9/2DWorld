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

#include "GameObject.h"

GameObject::GameObject() = default;

GameObject::~GameObject() = default;

void GameObject::Animate(float frameSpeed, float numFrames, const bool animate)
{
    assert(_pCurrentTexture != nullptr);

    _source = {
        _currentFrame * (float)_pCurrentTexture->width / numFrames,
        0.0f, _facing * (float)_pCurrentTexture->width / numFrames,
        (float)_pCurrentTexture->height
    };

    _dest = {
        _position.x, _position.y,
        _scale * (float)_pCurrentTexture->width / numFrames,
        _scale * (float)_pCurrentTexture->height
    };

    if (animate)
    {
        _frameCounter++;
        if (_frameCounter >= (GetFPS() / frameSpeed))
        {
            _frameCounter = 0;
            _currentFrame++;
            if (_currentFrame > numFrames) _currentFrame = 0;
        }
    }
}