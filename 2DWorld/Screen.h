/* by Wildan R. */

#pragma once

#include <array>
#include "Math2D.h"

class Screen
{
public:
	virtual void Draw() = 0;
};

void DrawHelixNebula();
void DrawEarth();
void DrawGalaxy();
void DrawDusk();

void SetActiveScreen(Screen* screen);
void DrawScreen();

void InitScreenTexture();
void UnloadScreenTexture();

void DrawCenteredText(int y, const char* text, int fontSize, Color color);