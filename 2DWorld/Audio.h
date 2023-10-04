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

#pragma once

#include "raylib.h"

#include <thread>
#include <atomic>
#include <stdexcept>

class Audio
{
	template<typename T>
	T LoadAudioData(const char* audio);

public:
	void Update(std::string& bgm, std::atomic<bool>& isEngineShutDown);
	void Load();
	void Free();

private:
	void UnloadBGM();

private:
	Music _bgm = {};
};

template<typename T>
T Audio::LoadAudioData(const char* audio)
{
	if constexpr (std::is_same<T, Music>::value) 
	{
		return LoadMusicStream(audio);
	}
	else if constexpr (std::is_same<T, Sound>::value) 
	{
		return LoadSound(audio);
	}
	else 
	{
		throw std::runtime_error("Unsupported audio data type!");
	}
}

void PlayClickSound();
float GetMasterVolume();
