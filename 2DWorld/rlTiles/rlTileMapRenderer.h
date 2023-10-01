/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLTiles * Tiled map rendering
*
*   LICENSE: MIT
*
*   Copyright (c) 2022 Jeffery Myers
*   Edited by Wildan Wijanarko in 2023
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

#include "rlTileMap.h"
#include "../rlCamera2D/rlCamera2D.h"

typedef struct RLRenderTile
{
    Rectangle SourceRect = { 0 };
    Vector2 DestinationPos = { 0 };
    bool Rotate = false;
    Texture* SourceTexture = nullptr;
} RLRenderTile;

class RLTileRenderLayer
{
public:
    Vector2 TileSize = { 0,0 };
    std::vector<RLRenderTile> RenderTiles;
};

class RLTileRenderer
{
public:
    RLTileMap& TileMap;

    std::map<int, RLTileRenderLayer> RenderLayers;
    std::map<std::pair<int, int>, std::pair<Texture, RLTileSheet*>> TextureCache;

    RLTileRenderer(RLTileMap& map) : TileMap(map) {}
    ~RLTileRenderer();

    inline void Clear()
    {
        for (const auto& texture : TextureCache)
        {
            UnloadTexture(texture.second.first);
        }

        TextureCache.clear();
        RenderLayers.clear();
    }

    void Setup();
    void Draw(const RLCamera2D& camera);
    void DrawGrid(const Rectangle& playerRec);

protected:
    bool TileInView(RLRenderTile& tile);

    Rectangle CurrentViewRect = { 0,0,0,0 };
};