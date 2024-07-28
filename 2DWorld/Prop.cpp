/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2024 Wildan R Wijanarko
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

#include "raylib.h"

#include "GameObject.h"

class House final : public GameObject
{
public:
    House()
    {
        Start();
    }

    ~House()
    {
        UnloadModel2D(_model);
    }

protected:
    void Start() override
    {
        _model = LoadModel2D();
        _model.textures->LoadTextureFile("resources/textures/house/house1.png");

        _model.currTexture = &_model.textures->at(0);

        _model.trans.pos = { 1340.0f, 170.0f };

        _model.rec = { _model.trans.pos.x, _model.trans.pos.y, 90.0f, 140.0f };

        name = "House";

        _model.trans.scl = 0.8f;
    }

    void Update() override
    {
        _model.rec = { _model.trans.pos.x + 10.0f, _model.trans.pos.y + 40.0f, 90.0f, 140.0f };
    }
};

class Ark final : public GameObject
{
public:
    Ark()
    {
        Start();
    }

    ~Ark()
    {
        UnloadModel2D(_model);
    }

protected:
    void Start() override
    {
        _model = LoadModel2D();
        _model.textures->LoadTextureFile("resources/textures/ark/ark.png");

        _model.currTexture = &_model.textures->at(0);

        _model.trans.pos = { -10.0f, 900.0f };

        _model.rec = { _model.trans.pos.x, _model.trans.pos.y, 440.0f, 200.0f };

        name = "Ark";

        _model.trans.scl = 0.4f;
    }

    void Update() override
    {
        _model.rec = { _model.trans.pos.x, _model.trans.pos.y + 30.0f, 440.0f, 200.0f };
    }
};