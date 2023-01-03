/* by Wildan R. */

#include "Screen.h"

Screen* activeScreen = nullptr;

static const char* strTextureFile[4]
{
	"textures/screens_bg/helix_nebula.png",
	"textures/screens_bg/earth.png",
	"textures/screens_bg/galaxy.png",
	"textures/screens_bg/dusk.png"
};

std::array<Texture2D, 4> screenTextures{};

void InitScreenTexture()
{
	for (auto i = 0; i < 4; i++)
	{
		screenTextures.at(i) = LoadTexture(strTextureFile[i]);
	}
}

void DrawHelixNebula()
{
	DrawTextureV(screenTextures.at(0), {}, WHITE);
}

void DrawEarth()
{
	DrawTextureV(screenTextures.at(1), {}, WHITE);
}

void DrawGalaxy()
{
	DrawTextureV(screenTextures.at(2), {}, WHITE);
}

void DrawDusk()
{
	DrawTextureV(screenTextures.at(3), {}, WHITE);
}

void UnloadScreenTexture()
{
	for (const auto& texture : screenTextures)
	{
		UnloadTexture(texture);
	}
}

void SetActiveScreen(Screen* screen)
{
	activeScreen = screen;
}

void DrawScreen()
{
	if (activeScreen != nullptr)
	{
		activeScreen->Draw();
	}
}

void DrawCenteredText(int y, const char* text, int fontSize, Color color)
{
	int textWidth(MeasureText(text, fontSize));
	DrawText(text, GetScreenWidth() / 2 - textWidth / 2, y - fontSize / 2, fontSize, color);
}