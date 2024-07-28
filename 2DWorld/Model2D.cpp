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

#include "Model2D.h"
#include "rlgl.h"

Model2D LoadModel2D(const std::vector<std::string>& texturesPath)
{
    Model2D model{};
    model.facing = 1.0f;
    model.rec = math::rec();
    model.trans = math::trans2d();
    model.textures = std::make_unique<ObjectTexures>();

    if (!texturesPath.empty())
    {
        model.textures->push_back(Texture2D());

        for (auto& p : texturesPath)
        {
            if (p == "") continue;           
            model.textures->LoadTextureFile(p.c_str());
        }
    }

    return model;
}

void UnloadModel2D(Model2D& model)
{
    if (model.textures != nullptr)
    {
        model.textures->Clear();
    }
}

void UpdateAnim(Model2D& model, int frameSpeed, int numFrames, bool animate)
{
    if (model.animator == nullptr) return;

    auto* pAnim = model.animator.get();
    auto* pTex  = model.currTexture;

    pAnim->recData[0] = math::rec{
        pAnim->currFrame * (float)pTex->width/numFrames,
        0.0f, model.facing * (float)pTex->width/numFrames,
        (float)pTex->height
    };

    pAnim->recData[1] = math::rec{
        model.trans.pos.x, model.trans.pos.y,
        model.trans.scl * (float)pTex->width/numFrames,
        model.trans.scl * (float)pTex->height
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

void DrawModel2D(const Model2D& model)
{
    const Animator* animator = model.animator.get();

    math::vec2 ori{};
    math::rec  src{};
    math::rec  dst{};

    if (model.currTexture == nullptr) return;

    Texture2D& tex = *model.currTexture;
    float rot = model.trans.rot;

    if (animator == nullptr)
    {
        ori = math::vec2();
        src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
        dst = { model.trans.pos.x, model.trans.pos.y, (float)tex.width * model.trans.scl, (float)tex.height * model.trans.scl };
    }
    else
    {
        ori = math::vec2();
        src = animator->recData[0];
        dst = animator->recData[1];
    }

    if (tex.id > 0)
    {
        float w = (float)tex.width;
        float h = (float)tex.height;

        math::vec2 vertices[4];

        if (rot == 0.0f)
        {
            float x = dst.x - ori.x;
            float y = dst.y - ori.y;

            vertices[0] = { x, y };                    // Top-left
            vertices[1] = { x, y + dst.h };            // Bottom-left
            vertices[2] = { x + dst.w, y + dst.h };    // Bottom-right
            vertices[3] = { x + dst.w, y };            // Top-right
        }
        else
        {
            float sinrot = std::sinf(rot * DEG2RAD);
            float cosrot = std::cosf(rot * DEG2RAD);

            float x = dst.x;
            float y = dst.y;

            float dx = -ori.x;
            float dy = -ori.y;

            vertices[0] = { x + dx * cosrot - dy * sinrot, y + dx * sinrot + dy * cosrot }; // Top-left
            vertices[1] = { x + dx * cosrot - (dy + dst.h) * sinrot, y + dx * sinrot + (dy + dst.h) * cosrot }; // Bottom-left
            vertices[2] = { x + (dx + dst.w) * cosrot - (dy + dst.h) * sinrot, y + (dx + dst.w) * sinrot + (dy + dst.h) * cosrot }; // Bottom-right
            vertices[3] = { x + (dx + dst.w) * cosrot - dy * sinrot, y + (dx + dst.w) * sinrot + dy * cosrot }; // Top-right
        }

        rlSetTexture(tex.id);
        rlBegin(RL_QUADS);

        rlColor4ub(255, 255, 255, 255); // White
        rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

        rlTexCoord2f(src.x/w, src.y/h);
        rlVertex2f(vertices[0].x, vertices[0].y);

        rlTexCoord2f(src.x/w, (src.y + src.h)/h);
        rlVertex2f(vertices[1].x, vertices[1].y);

        rlTexCoord2f((src.x + src.w)/w, (src.y + src.h)/h);
        rlVertex2f(vertices[2].x, vertices[2].y);

        rlTexCoord2f((src.x + src.w)/w, src.y/h);
        rlVertex2f(vertices[3].x, vertices[3].y);

        rlEnd();
        rlSetTexture(0);
    }
}