/* by Wildan R. */

#pragma once

#include "raylib.h"
#include "GetterSetter.h"

namespace raylib 
{
    class Camera2D : public ::Camera2D
    {
    public:
        Camera2D(const ::Camera2D& camera)
        {
            set(camera);
        }

        Camera2D() {}
        Camera2D(Vector2 offset, Vector2 target, float rotation = 0.0f, float zoom = 1.0f) : ::Camera2D{ offset, target, rotation, zoom } {}

        inline Camera2D& BeginMode()
        {
            ::BeginMode2D(*this);
            return *this;
        }

        inline Camera2D& EndMode()
        {
            ::EndMode2D();
            return *this;
        }

        GETTERSETTER(Vector2, Offset, offset)
        GETTERSETTER(Vector2, Target, target)
        GETTERSETTER(float, Rotation, rotation)
        GETTERSETTER(float, Zoom, zoom)

        Camera2D& operator=(const ::Camera2D& camera)
        {
            set(camera);
            return *this;
        }

    private:
        inline void set(const ::Camera2D& camera)
        {
            offset = camera.offset;
            target = camera.target;
            rotation = camera.rotation;
            zoom = camera.zoom;
        }
    };
}