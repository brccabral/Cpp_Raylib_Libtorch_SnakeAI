#pragma once
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <rlgl.h>

inline size_t index_from_location(Vector2 pt, size_t width)
{
    return int(pt.y) * width + int(pt.x);
}

inline void DrawTexturePoly(
        const Texture2D *texture, const Vector2 &center, const Vector2 *points,
        const Vector2 *texture_coords, int pointCount, const Color &tint)
{
    rlSetTexture(texture->id);
    rlBegin(RL_TRIANGLES);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    for (int i = 0; i < pointCount - 1; i++)
    {
        rlTexCoord2f(0.5f, 0.5f);
        rlVertex2f(center.x, center.y);

        rlTexCoord2f(texture_coords[i].x, texture_coords[i].y);
        rlVertex2f(points[i].x, points[i].y);

        rlTexCoord2f(texture_coords[i + 1].x, texture_coords[i + 1].y);
        rlVertex2f(points[i + 1].x, points[i + 1].y);
    }
    rlEnd();
    rlSetTexture(0);
}

inline Vector2 operator*(const Vector2 &lhs, float rhs)
{
    return Vector2(lhs.x * rhs, lhs.y * rhs);
}

inline Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline void operator+=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

inline Vector2 operator/(const Vector2 &lhs, float rhs)
{
    return Vector2(lhs.x / rhs, lhs.y / rhs);
}

inline float Vector2Distance(const Vector2 &lhs, const Vector2 &rhs)
{
    return sqrtf((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}

inline Vector2 Vector2Rotate(const Vector2 &v, float angle)
{
    Vector2 result = {0};

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres - v.y * sinres;
    result.y = v.x * sinres + v.y * cosres;

    return result;
}

inline bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
