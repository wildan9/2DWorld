/* by Wildan R. */

#pragma once

#include "FileUtils.h"

class BaseAnimation
{
public:
    BaseAnimation();
    virtual ~BaseAnimation() = default;

protected:
    void Animate(Vector2D& position, Texture2D& texture, const float& deltaTime, const float& size, const float& row, float facing = 1.0f, float timer = 0.0f, bool animate = 1, float rotation = 0.0f);

private:
    const float _updateTime{ 0.0834f };
    float _timer{};
    int _frame{};
};