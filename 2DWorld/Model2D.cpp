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

#include "Model2D.h"
#include "rlgl.h"

Model2D LoadModel2D(const std::vector<std::string>& texturesPath)
{
    Model2D m;
    m.trans = Trans2D();
    m.textures = std::make_unique<ObjectTexures>();

    if (!texturesPath.empty())
    {
        m.textures->push_back(Texture2D());

        for (auto& p : texturesPath)
        {
            if (p == "") continue;           
            m.textures->LoadTextureFile(p.c_str());
        }

        m.currTexture = &m.textures->at(0);
    }

    return m;
}

void UnloadModel2D(Model2D& model)
{
    if (model.textures != nullptr)
    {
        model.textures->Clear();
    }
}

std::unique_ptr<AnimationData> CreateAnimData()
{
    auto a = std::make_unique<AnimationData>();
    a->currFrame = 0;
    a->frameCounter = 0;
    a->recFrame.recData = std::array<Rectangle, 2>();
    a->recFrame.facing = 1.0f;

    return a;
}

void UpdateAnim(Model2D& model, float facing, float frameSpeed, int numFrames, int frame, bool animate)
{
    auto animData = model.animData.get();

    if (animData == nullptr) return;
    
    Trans2D trans = model.trans;
    model.currTexture = &model.textures->at(frame);
    Texture2D texture = *model.currTexture;

    animData->recFrame.recData[0] = Rectangle{
        animData->currFrame*(float)texture.width/numFrames,
        0.0f, facing*(float)texture.width/numFrames,
        (float)texture.height
    };

    animData->recFrame.recData[1] = Rectangle{
        trans.pos.x, trans.pos.y,
        trans.scl*(float)texture.width/numFrames,
        trans.scl*(float)texture.height
    };

    if (!animate) return;

    animData->frameCounter++;
    if (animData->frameCounter >= (GetFPS()/frameSpeed))
    {
        animData->frameCounter = 0;
        animData->currFrame++;
        if (animData->currFrame > numFrames) animData->currFrame = 0;
    }
}

void DrawModel2D(const Model2D& model)
{
    const AnimationData* animData = model.animData.get();

    Vector2 ori;
    Rectangle  src;
    Rectangle  dst;

    if (model.currTexture == nullptr) return;

    Texture2D& tex = *model.currTexture;
    float rot = model.trans.rot;

    if (animData == nullptr)
    {
        ori = Vector2();
        src = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
        dst = {model.trans.pos.x, model.trans.pos.y, (float)tex.width*model.trans.scl, (float)tex.height*model.trans.scl};
    }
    else
    {
        ori = Vector2();
        src = animData->recFrame.recData[0];
        dst = animData->recFrame.recData[1];
    }

    if (tex.id > 0)
    {
        float w = (float)tex.width;
        float h = (float)tex.height;

        Vector2 vertices[4];

        if (rot == 0.0f)
        {
            float x = dst.x - ori.x;
            float y = dst.y - ori.y;

            vertices[0] = {x, y};                    // Top-left
            vertices[1] = {x, y + dst.height};            // Bottom-left
            vertices[2] = {x + dst.width, y + dst.height};    // Bottom-right
            vertices[3] = {x + dst.width, y};            // Top-right
        }
        else
        {
            float sinrot = sinf(rot*DEG2RAD);
            float cosrot = cosf(rot*DEG2RAD);

            float x = dst.x;
            float y = dst.y;

            float dx = -ori.x;
            float dy = -ori.y;

            vertices[0] = {x + dx*cosrot - dy*sinrot, y + dx*sinrot + dy*cosrot}; // Top-left
            vertices[1] = {x + dx*cosrot - (dy + dst.height)*sinrot, y + dx*sinrot + (dy + dst.height)*cosrot}; // Bottom-left
            vertices[2] = {x + (dx + dst.width)*cosrot - (dy + dst.height)*sinrot, y + (dx + dst.width) * sinrot + (dy + dst.height)*cosrot}; // Bottom-right
            vertices[3] = {x + (dx + dst.width)*cosrot - dy*sinrot, y + (dx + dst.width)*sinrot + dy*cosrot}; // Top-right
        }

        rlSetTexture(tex.id);
        rlBegin(RL_QUADS);

        rlColor4ub(255, 255, 255, 255); // White
        rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

        rlTexCoord2f(src.x/w, src.y/h);
        rlVertex2f(vertices[0].x, vertices[0].y);

        rlTexCoord2f(src.x/w, (src.y + src.height)/h);
        rlVertex2f(vertices[1].x, vertices[1].y);

        rlTexCoord2f((src.x + src.width)/w, (src.y + src.height)/h);
        rlVertex2f(vertices[2].x, vertices[2].y);

        rlTexCoord2f((src.x + src.width)/w, src.y/h);
        rlVertex2f(vertices[3].x, vertices[3].y);

        rlEnd();
        rlSetTexture(0);
    }
}

void AnimationData::Reset()
{
    currFrame = 0;
    frameCounter = 0;
    recFrame.recData = std::array<Rectangle, 2>();
    recFrame.facing = 1.0f;
}