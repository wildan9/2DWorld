/* by Wildan R. */

#include "Animation.h"

BaseAnimation::BaseAnimation()
{

}

void BaseAnimation::Animate(Vector2D& position, Texture2D& texture, const float& deltaTime, const float& size, const float& row, float facing, float timer, bool animate, float rotation)
{
    if (animate)
    {
        _timer += deltaTime;
        if (_timer >= _updateTime)
        {
            _frame++;
            _timer = timer;
            if (_frame > row) _frame = 1;
        }
    }

    Rectangle source{
        _frame * (float)texture.width / row,
        0.0f, facing * (float)texture.width / row,
        (float)texture.height
    };
    Rectangle dest{
        position.x, position.y,
        size * (float)texture.width / row,
        size * (float)texture.height
    };
    DrawTexturePro(texture, source, dest, Vector2D{}, rotation, WHITE);
}