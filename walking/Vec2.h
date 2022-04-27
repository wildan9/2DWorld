/* by Wildan R. */

#pragma once

#include "raylib.h"
#include "raymath.h"
#include "GetterSetter.h"

class Vec2 : public Vector2 
{
public:
    Vec2(const Vector2& vec) 
    { 
        set(vec); 
    }

    Vec2(float x, float y) : Vector2{ x, y } {}
    Vec2(float x) : Vector2{ x, 0 } {}
    Vec2() : Vector2{ 0, 0 } {}

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)

    Vec2& operator=(const Vector2& vector2) 
    {
        set(vector2);
        return *this;
    }

    bool operator==(const Vector2& other) 
    {
        return x == other.x && y == other.y;
    }

    Vec2 Add(const Vector2& vector2) const 
    {
        return Vector2Add(*this, vector2);
    }

    Vec2 operator+(const Vector2& vector2) const 
    {
        return Vector2Add(*this, vector2);
    }

    Vec2 Subtract(const Vector2& vector2) const 
    {
        return Vector2Subtract(*this, vector2);
    }

    Vec2 operator-(const Vector2& vector2) const 
    {
        return Vector2Subtract(*this, vector2);
    }

    Vec2 Negate() const 
    {
        return Vector2Negate(*this);
    }

    Vec2 operator-() const 
    {
        return Vector2Negate(*this);
    }

    Vec2 Multiply(const Vector2& vector2) const 
    {
        return Vector2Multiply(*this, vector2);
    }

    Vec2 operator*(const Vector2& vector2) const 
    {
        return Vector2Multiply(*this, vector2);
    }

    Vec2 Scale(const float scale) const 
    {
        return Vector2Scale(*this, scale);
    }

    Vec2 operator*(const float scale) const 
    {
        return Vector2Scale(*this, scale);
    }

    Vec2 Divide(const Vector2& vector2) const 
    {
        return Vector2Divide(*this, vector2);
    }

    Vec2 operator/(const Vector2& vector2) const 
    {
        return Vector2Divide(*this, vector2);
    }

    Vec2 Divide(const float div) const 
    {
        return Vector2{ x / div, y / div };
    }

    Vec2 operator/(const float div) const 
    {
        return Divide(div);
    }

    Vec2& operator+=(const Vector2& vector2) 
    {
        set(Vector2Add(*this, vector2));

        return *this;
    }

    Vec2& operator-=(const Vector2& vector2) 
    {
        set(Vector2Subtract(*this, vector2));

        return *this;
    }

    Vec2& operator*=(const Vector2& vector2) 
    {
        set(Vector2Multiply(*this, vector2));

        return *this;
    }

    Vec2& operator*=(const float scale) 
    {
        set(Vector2Scale(*this, scale));

        return *this;
    }

    Vec2& operator/=(const Vector2& vector2) 
    {
        set(Vector2Divide(*this, vector2));

        return *this;
    }

    Vec2& operator/=(const float div) 
    {
        this->x /= div;
        this->y /= div;

        return *this;
    }

    float Length() const
    {
        return Vector2Length(*this);
    }

    float LengthSqr() const 
    {
        return Vector2LengthSqr(*this);
    }

    Vec2 Normalize() const 
    {
        return Vector2Normalize(*this);
    }

    float DotProduct(const Vector2& vector2) const 
    {
        return Vector2DotProduct(*this, vector2);
    }

    float Angle(const Vector2& vector2) const 
    {
        return Vector2Angle(*this, vector2);
    }

    float Distance(const Vector2& vector2) const 
    {
        return Vector2Distance(*this, vector2);
    }

    Vec2 Lerp(const Vector2& vector2, float amount) const 
    {
        return Vector2Lerp(*this, vector2, amount);
    }

    Vec2 Reflect(const Vector2& normal) const 
    {
        return Vector2Reflect(*this, normal);
    }

    Vec2 Rotate(float degrees) const 
    {
        return Vector2Rotate(*this, degrees);
    }

    Vec2 MoveTowards(const Vector2& target, float max_distance) const 
    {
        return Vector2MoveTowards(*this, target, max_distance);
    }

    static Vec2 Zero() 
    {
        return Vector2Zero();
    }

    static Vec2 One() 
    {
        return Vector2One();
    }

private:
    inline void set(const Vector2& vec) 
    {
        x = vec.x;
        y = vec.y;
    }
};