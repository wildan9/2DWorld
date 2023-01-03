/* by Wildan R. */

#pragma once

#include <cmath>
#include <string>
#include "raylib.h"

struct Vector2D : public Vector2
{
    Vector2D(const Vector2& vec) : Vector2{ vec.x, vec.y } {}
    Vector2D(float x, float y) : Vector2{ x, y } {}
    Vector2D(float x) : Vector2{ x, 0.0f } {}
    Vector2D() : Vector2{ 0.0f, 0.0f } {}
    float Length() const;
    float DotProduct(const Vector2D& vec) const;
    Vector2D Add(const Vector2D& vec) const;
    Vector2D Add(const Vector2& vec) const;
    Vector2D Subtract(const Vector2D& vec) const;
    Vector2D Subtract(const Vector2& vec) const;
    Vector2D Scale(float scale) const;
    Vector2D Normalize() const;
    Vector2D Rotate(float angle) const;
    std::string ToString() const;
    int ConcatXY() const;
};

inline float Vector2D::Length() const
{
    return { sqrtf((x * x) + (y * y)) };
}

inline float Vector2D::DotProduct(const Vector2D& vec) const
{
    return { x * vec.x + y * vec.y };
}

inline Vector2D Vector2D::Add(const Vector2D& vec) const
{
    return { x + vec.x, y + vec.y };
}

inline Vector2D Vector2D::Add(const Vector2& vec) const
{
    return { x + vec.x, y + vec.y };
}

inline Vector2D Vector2D::Subtract(const Vector2D& vec) const
{
    return { x - vec.x, y - vec.y };
}

inline Vector2D Vector2D::Subtract(const Vector2& vec) const
{
    return { x - vec.x, y - vec.y };
}

inline Vector2D Vector2D::Scale(float scale) const
{
    return { x * scale, y * scale };
}

inline Vector2D Vector2D::Normalize() const
{
    Vector2D result{};

    float length{ sqrtf((x * x) + (y * y)) };

    if (length > 0)
    {
        float ilength = 1.0f / length;
        result.x = x * ilength;
        result.y = y * ilength;
    }

    return result;
}

inline Vector2D Vector2D::Rotate(float angle) const
{
    Vector2D result{};

    float cosres{ cosf(angle) };
    float sinres{ sinf(angle) };

    result.x = x * cosres - y * sinres;
    result.y = x * sinres + y * cosres;

    return result;
}

inline std::string Vector2D::ToString() const
{
    return { "x: " + std::to_string((int)x) + "  y: " + std::to_string((int)y) };
}

inline int Vector2D::ConcatXY() const
{
    return stoi(std::to_string((int)x) + std::to_string((int)y));
}