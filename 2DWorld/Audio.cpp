/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023-2025 Wildan R Wijanarko
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

#include "Audio.h"

#include <string>

enum class BGMStates
{
	IDLE = 0,
	LOAD_BIRD,
	LOAD_HARP

};
BGMStates bgmState{};

struct Volume
{
	bool  muted   = 0;
	float master  = 0.6f;
	float current = 0.6f;
};
Volume volume{};

Sound clickSound{};
Timer volumeBarTimer{};

char* currBGM;

void Audio::LoadResources()
{
	bgm = LoadAudioData<Music>("resources/sounds/birds-isaiah658.ogg");
	clickSound = LoadAudioData<Sound>("resources/sounds/menu_selection_click.wav");
}

void Audio::Update()
{
	switch (bgmState)
	{
	case BGMStates::LOAD_BIRD:
	{
		UnloadBGM();
		bgm = LoadAudioData<Music>("resources/sounds/birds-isaiah658.ogg");
		bgm.looping = 1;
		currBGM = "";
		PlayMusicStream(bgm);
	} break;
	case BGMStates::LOAD_HARP:
	{
		UnloadBGM();
		bgm = LoadAudioData<Music>("resources/sounds/harp.ogg");
		bgm.looping = 1;
		currBGM = "";
		PlayMusicStream(bgm);
	} break;
	default:
		break;
	}

	if (currBGM == "") bgmState = BGMStates::IDLE;
	else if (currBGM == "harp") bgmState = BGMStates::LOAD_HARP;
	else if (currBGM == "bird") bgmState = BGMStates::LOAD_BIRD;

	UpdateMusicStream(bgm);

	if (IsKeyPressed(KEY_L) && volume.master < 1.0f && !volume.muted)
	{
		volume.current += 0.1f;
		volume.master += 0.1f;
	}

	if (IsKeyPressed(KEY_K) && volume.master > 0.0f && !volume.muted)
	{
		volume.current -= 0.1f;
		volume.master -= 0.1f;
	}

	if (IsKeyPressed(KEY_M)) volume.muted = !volume.muted;
	if (IsKeyPressed(KEY_L) && volume.muted || IsKeyPressed(KEY_K) && volume.muted)
	{
		volume.muted = !volume.muted;
		volume.master = volume.current;
	}
	if (volume.muted && IsKeyPressed(KEY_M) || volume.muted)  volume.master = 0.0f;
	if (!volume.muted && IsKeyPressed(KEY_M) || !volume.muted) volume.master = volume.current;

	if (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_K) || IsKeyPressed(KEY_M))
	{
		std::string strVolume{"Master Volume: "};

		strVolume = strVolume + std::to_string(volume.master);

		TraceLog(LOG_INFO, strVolume.c_str());
	}

	SetMasterVolume(volume.master);
}

void Audio::FreeResources()
{
	UnloadBGM();
	UnloadSound(clickSound);
}

void Audio::UnloadBGM()
{
	StopMusicStream(bgm);

	bgm.frameCount = 0;

	UnloadMusicStream(bgm);
}

void PlayClickSound()
{
	PlaySound(clickSound);
}

void DrawVolumeBar()
{
	const char* strVolumeArr[11]
	{
		"Muted", "Volume: #", "Volume: # #", "Volume: # # #",
		"Volume: # # # #", "Volume: # # # # #", "Volume: # # # # # #",
		"Volume: # # # # # # #", "Volume: # # # # # # # #",
		"Volume: # # # # # # # # #", "Volume: # # # # # # # # # #"
	};

	const Color volumeColors[]{ RED, GREEN, LIME, DARKGREEN };

	const unsigned volume = GetMasterVolume() * 10;

	if (volume < 0.1f || !IsTimerDone(volumeBarTimer))
	{
		UpdateTimer(volumeBarTimer);

		int volumeColor = 1;

		if (volume < 1) volumeColor = 0; 
		else if (volume >= 1 && volume < 3) volumeColor = 1; 
		else if (volume >= 3 && volume < 7) volumeColor = 2; 
		else if (volume >= 7) volumeColor = 3;

		const char* strVolume = strVolumeArr[volume];

		DrawText(strVolume, GetScreenWidth() - 320, 10, 21, volumeColors[volumeColor]);
	}

	if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_L) || IsKeyPressed(KEY_K))
	{
		StartTimer(volumeBarTimer, 4.0f);
	}
}

void SetCurrBGM(char* bgm)
{
	currBGM = bgm;
}