/* by Wildan R. */

#pragma once

#include "raylib.h"

class BaseAnimation
{
public:
    BaseAnimation();
    virtual ~BaseAnimation() = default;

protected:
    void Animate(Vector2& position, Texture2D& texture, const float& delta_time, const float& size, const float& row, float facing = 1.0f, bool animate = 1);
    void Animate(Vector2& position, Texture2D& texture, const float& delta_time, const float& size, const float& row, float facing, float timer = 0.0f, bool animate = 1);

private:
    const float m_update_time{ 0.0834f };
    float m_timer{};
    int m_frame{};
};