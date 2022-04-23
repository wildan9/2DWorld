/* by Wildan R. */

#pragma once

#include "raylib.h"
#include "raymath.h"
#include "GetterSetter.h"

class Vector2s : public Vector2 
{
public:
    Vector2s(const Vector2& vec) 
    { 
        set(vec); 
    }

    Vector2s(float x, float y) : Vector2{ x, y } {}
    Vector2s(float x) : Vector2{ x, 0 } {}
    Vector2s() : Vector2{ 0, 0 } {}

    GETTERSETTER(float, X, x)
    GETTERSETTER(float, Y, y)

    Vector2s& operator=(const Vector2& vector2) 
    {
        set(vector2);
        return *this;
    }

    bool operator==(const Vector2& other) 
    {
        return x == other.x && y == other.y;
    }

    Vector2s Add(const Vector2& vector2) const 
    {
        return Vector2Add(*this, vector2);
    }

    Vector2s operator+(const Vector2& vector2) const 
    {
        return Vector2Add(*this, vector2);
    }

    Vector2s Subtract(const Vector2& vector2) const 
    {
        return Vector2Subtract(*this, vector2);
    }

    Vector2s operator-(const Vector2& vector2) const 
    {
        return Vector2Subtract(*this, vector2);
    }

    Vector2s Negate() const 
    {
        return Vector2Negate(*this);
    }

    Vector2s operator-() const 
    {
        return Vector2Negate(*this);
    }

    Vector2s Multiply(const Vector2& vector2) const 
    {
        return Vector2Multiply(*this, vector2);
    }

    Vector2s operator*(const Vector2& vector2) const 
    {
        return Vector2Multiply(*this, vector2);
    }

    Vector2s Scale(const float scale) const 
    {
        return Vector2Scale(*this, scale);
    }

    Vector2s operator*(const float scale) const 
    {
        return Vector2Scale(*this, scale);
    }

    Vector2s Divide(const Vector2& vector2) const 
    {
        return Vector2Divide(*this, vector2);
    }

    Vector2s operator/(const Vector2& vector2) const 
    {
        return Vector2Divide(*this, vector2);
    }

    Vector2s Divide(const float div) const 
    {
        return Vector2{ x / div, y / div };
    }

    Vector2s operator/(const float div) const 
    {
        return Divide(div);
    }

    Vector2s& operator+=(const Vector2& vector2) 
    {
        set(Vector2Add(*this, vector2));

        return *this;
    }

    Vector2s& operator-=(const Vector2& vector2) 
    {
        set(Vector2Subtract(*this, vector2));

        return *this;
    }

    Vector2s& operator*=(const Vector2& vector2) 
    {
        set(Vector2Multiply(*this, vector2));

        return *this;
    }

    Vector2s& operator*=(const float scale) 
    {
        set(Vector2Scale(*this, scale));

        return *this;
    }

    Vector2s& operator/=(const Vector2& vector2) 
    {
        set(Vector2Divide(*this, vector2));

        return *this;
    }

    Vector2s& operator/=(const float div) 
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

    Vector2s Normalize() const 
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

    Vector2s Lerp(const Vector2& vector2, float amount) const 
    {
        return Vector2Lerp(*this, vector2, amount);
    }

    Vector2s Reflect(const Vector2& normal) const 
    {
        return Vector2Reflect(*this, normal);
    }

    Vector2s Rotate(float degrees) const 
    {
        return Vector2Rotate(*this, degrees);
    }

    Vector2s MoveTowards(const Vector2& target, float max_distance) const 
    {
        return Vector2MoveTowards(*this, target, max_distance);
    }

    static Vector2s Zero() 
    {
        return Vector2Zero();
    }

    static Vector2s One() 
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