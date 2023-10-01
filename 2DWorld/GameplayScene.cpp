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

#include "GameplayScene.h"

void GameplayScene::Start()
{
    RLReadTileMap("resources/maps/empire/wildan_empire.tmx", _tileMap);

    for (std::map<int, RLTileSheet>::iterator itr = _tileMap.Sheets.begin(); itr != _tileMap.Sheets.end(); itr++)
    {
        itr->second.SheetSource = "resources/" + itr->second.SheetSource;
    }

    _rendererMap = std::make_unique< RLTileRenderer>(_tileMap);
    _rendererMap->Setup();
}

void GameplayScene::Update()
{
    Rectangle mapRec = { 10.0f, 10.0f, 61.5f * 61.5f / 2.0f, 61.5f * 61.5f / 2.0f };

    _camera.Update({}, mapRec, GetScreenWidth(), GetScreenHeight(), 1);
}

void GameplayScene::LoadResources()
{

}

void GameplayScene::FreeResources()
{

}

void GameplayScene::Draw()
{
    _camera.BeginMode();
    _rendererMap->Draw(_camera);
    _camera.EndMode();

    DrawFPS(0, 0);
}