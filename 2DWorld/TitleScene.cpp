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

#include "TitleScene.h"

void TitleScene::Start()
{
	LoadResources();
}

void TitleScene::Update()
{

}

void TitleScene::LoadResources()
{
	_earthTexture = LoadTexture("resources/textures/screens_bg/earth.png");
}

void TitleScene::FreeResources()
{
	UnloadTexture(_earthTexture);
}

void TitleScene::Draw()
{
	DrawTextureV(_earthTexture, {}, WHITE);
	DrawText("Walking", 15, 20, 40, GREEN);
	DrawCenteredText(164, "Welcome to 2DWorld!", 24, GREEN);
	DrawCenteredText(220, "PRESS ENTER to Walking!", 19, GREEN);
}