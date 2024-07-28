/**********************************************************************************************
*
*   LICENSE: MIT
*
*   Copyright (c) 2023-2024 Wildan R Wijanarko
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

#include "Model2D.h"

#include <memory>
#include <cassert>

class GameObject
{
protected:
	Model2D _model;

public:
	GameObject() : _model{} {}
	virtual ~GameObject() = default;
	virtual void Start()  = 0;
	virtual void Update() = 0;
	void Draw() const;

	int id = 0;
	std::string name = "";
	bool isInView    = 0;
	bool isOnTrigger = 0;

	inline math::rec  GetRec() const { return _model.rec; }
	inline math::vec2 GetPos() const { return _model.trans.pos; }
	inline float      GetFacing() const { return _model.facing; }

	inline void SetFacing(float facing) { _model.facing = facing; }
	inline void SetRec(const math::rec& rec) { _model.rec = rec; }
	inline void SetPos(const math::vec2& pos) { _model.trans.pos = pos; }
};