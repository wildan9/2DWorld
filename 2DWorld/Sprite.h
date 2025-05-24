/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2025 Wildan R Wijanarko
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

class Sprite
{
private:
    Vector2 position;
    Rectangle frameRec;
    Texture2D spriteSheet;

    int frameWidth;
    int frameHeight;
    int currentFrame;
    int frameCounter;
    float frameSpeed;
    float frameScale;
    float frameFacing;

    int frameColumns;
    int frameRows;

public:
    Sprite(Vector2 position_, const char* spriteSheetPath_, int frameColumns_, int frameRows_, float frameFacing_)
    {
        spriteSheet = LoadTexture(spriteSheetPath_);
        position = position_;

        frameColumns = frameColumns_;
        frameRows = frameRows_;

        frameWidth = spriteSheet.width/frameColumns_;
        frameHeight = spriteSheet.height/frameRows_;

        frameRec = {
            0, 0,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)
        };

        currentFrame = 0;
        frameCounter = 0;
        frameSpeed = 8.0f;
        frameScale = 2.0f;
        frameFacing = frameFacing_;
    }

    ~Sprite()
    {
        UnloadTexture(spriteSheet);
    }

    void Reset(float frameScale_ = 1.0f)
    {
        currentFrame = 0;
        frameCounter = 0;
        frameSpeed = 8.0f;
        frameScale = frameScale_;
    }

    int GetCurrentFrame() const
    {
        return currentFrame;
    }

    void Update(Vector2 position_, float frameScale_, float frameSpeed_, int selectedRow_, float frameFacing_, int totalFrames_)
    {
        const int framesPerRow = spriteSheet.width/frameWidth*totalFrames_/frameColumns;

        frameScale = frameScale_;
        frameSpeed = frameSpeed_;
        frameFacing = frameFacing_;
        position = position_;

        frameCounter++;
        if (frameCounter >= (GetFPS()/frameSpeed))
        {
            currentFrame++;
            if (currentFrame >= framesPerRow) currentFrame = 0;

            const int row = selectedRow_;
            const int col = currentFrame;

            frameRec.x = col*frameWidth;
            frameRec.y = row*frameHeight;

            frameCounter = 0;
        }
    }

    void Draw() const
    {
        const Rectangle source{
            frameRec.x, frameRec.y,
            frameRec.width*frameFacing,
            frameRec.height
        };

        DrawTexturePro(
            spriteSheet,
            source,
            Rectangle{
            position.x, position.y,
                frameRec.width*frameScale,
                frameRec.height*frameScale
            },
            Vector2{0, 0}, 0.0f,
            WHITE
        );
    }
};