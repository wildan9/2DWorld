/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2024 Wildan R Wijanarko
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

#include "Animator.h"

Animator CreateAnimator()
{
    Animator a;
    a.currFrame = 0;
    a.frameCounter = 0;
    a.recData = std::array<math::rec, 2>();

    return a;
}

void UpdateAnim(Animator* animator, Texture2D texture, math::trans2d trans, float facing, int frameSpeed, int numFrames, bool animate)
{
    if (animator == nullptr) return;

    auto* pAnim = animator;

    pAnim->recData[0] = math::rec{
        pAnim->currFrame * (float)texture.width / numFrames,
        0.0f, facing * (float)texture.width / numFrames,
        (float)texture.height
    };

    pAnim->recData[1] = math::rec{
        trans.pos.x, trans.pos.y,
        trans.scl * (float)texture.width/numFrames,
        trans.scl * (float)texture.height
    };

    if (!animate) return;

    pAnim->frameCounter++;
    if (pAnim->frameCounter >= (GetFPS()/frameSpeed))
    {
        pAnim->frameCounter = 0;
        pAnim->currFrame++;
        if (pAnim->currFrame > numFrames) pAnim->currFrame = 0;
    }
}