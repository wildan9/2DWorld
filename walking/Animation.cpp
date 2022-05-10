/* by Wildan R. */

#include "Animation.h"

BaseAnimation::BaseAnimation()
{

}

void BaseAnimation::Animate(Vector2D& position, Texture2D& texture, const float& delta_time, const float& size, const float& row, float facing, bool animate)
{
    if (animate)
    {
        m_timer += delta_time;
        if (m_timer >= m_update_time)
        {
            m_frame++;
            m_timer = 0.0f;
            if (m_frame > row)  m_frame = 1;
        }
    }

    Rectangle source{
        m_frame * (float)texture.width / row,
        0.0f,  facing * (float)texture.width / row,
        (float)texture.height
    };
    Rectangle dest{
        position.x, position.y,
        size * (float)texture.width / row,
        size * (float)texture.height
    };
    DrawTexturePro(texture, source, dest, Vector2D{}.ToVector2(), 0.0f, WHITE);
}

void BaseAnimation::Animate(Vector2D& position, Texture2D& texture, const float& delta_time, const float& size, const float& row, float facing, float timer, bool animate)
{
    if (animate)
    {
        m_timer += delta_time;
        if (m_timer >= m_update_time)
        {
            m_frame++;
            m_timer = timer;
            if (m_frame > row)  m_frame = 1;
        }
    }

    Rectangle source{
        m_frame * (float)texture.width / row,
        0.0f, facing * (float)texture.width / row,
        (float)texture.height
    };
    Rectangle dest{
        position.x, position.y,
        size * (float)texture.width / row,
        size * (float)texture.height
    };
    DrawTexturePro(texture, source, dest, Vector2D{}.ToVector2(), 0.0f, WHITE);
}