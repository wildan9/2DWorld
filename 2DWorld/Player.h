/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2022-2023 Wildan Wijanarko (@wildan9)
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

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	inline void Stop() 
	{ 
		_position.x = _lastPosition.x;
		_position.y = _lastPosition.y;
	}

	inline void OnHorse(bool isOnHorse) 
	{ 
		_isOnHorse = isOnHorse; 
	}

	inline bool IsPunch() const 
	{ 
		return (IsKeyDown(KEY_E) && !_isWalk); 
	}
	
	inline bool IsOnHorse() const 
	{ 
		return _isOnHorse; 
	}
	
	inline bool IsInvisible() const 
	{ 
		return IsKeyDown(KEY_LEFT_SHIFT); 
	}
	
	inline float GetStamina() const 
	{ 
		return (_isDragonInside) ? 9.0f : _stamina; 
	}

	inline void SetStamina(bool isDragonInside)
	{
		_stamina = 6.0f;
		_isDragonInside = isDragonInside;
	}

public:
	void OnLand();
	void OnWater();
	void Start()  override;
	void Update() override;
	float GetSpeed() const;
	Vector2 GetDirection() const;

private:
	int FrameSpeed() const;
	int NumFrames()  const;

private:
	bool _isWalk, _isDragonInside, _isOnHorse;
	Sound _landStep, _waterStep, _horseStep;
	const float _updateTime = 0.084f;
	float _timer, _stamina;
	Vector2 _lastPosition;
};