/* by Wildan R. */

#pragma once

#include "Math2D.h"

class BaseAnimation
{
public:
    virtual ~BaseAnimation() = default;

protected:
    void Animate(const Vector2D& position, const Texture2D& texture, const float deltaTime, const float size, 
        const float row, float facing = 1.0f, float timer = 0.0f, bool animate = 1, float rotation = 0.0f);

private:
    float _timer{};
    int _frame{};
};