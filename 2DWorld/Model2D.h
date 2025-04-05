/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2024-2025 Wildan R Wijanarko
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

#include "MathLib.h"
#include "ObjectTexures.h"

#include <array>
#include <string>
#include <memory>

struct RecFrame
{
    float facing;
    std::array<Rectangle, 2> recData;
};

struct AnimationData
{
    int currFrame;
    int frameCounter;
    RecFrame recFrame;
};

struct Model2D
{
    Trans2D trans;
    Texture2D* currTexture;
    std::unique_ptr<AnimationData> animData;
    std::unique_ptr<ObjectTexures> textures;
};

std::unique_ptr<AnimationData> CreateAnimData();
Model2D LoadModel2D(const std::vector<std::string>& texturesPath = {});
void UpdateAnim(Model2D& model, float facing, int frameSpeed, int numFrames, int frame, bool animate);
void UnloadModel2D(Model2D& model);
void DrawModel2D(const Model2D& model);