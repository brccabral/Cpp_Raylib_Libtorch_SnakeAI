#include <cassert>
#include <cmath>
#include "common.h"
#include <rlgl.h>

void draw_triangle(
        const Vector2 &center, const Vector2 &p1, const Vector2 &p2, const Vector2 &t1,
        const Vector2 &t2)
{
    rlTexCoord2f(0.5f, 0.5f);
    rlVertex2f(center.x, center.y);

    rlTexCoord2f(t1.x, t1.y);
    rlVertex2f(p1.x, p1.y);

    rlTexCoord2f(t2.x, t2.y);
    rlVertex2f(p2.x, p2.y);
}

void DrawTexturePoly(
        const Texture2D *texture, const Vector2 &center, const Vector2 *points,
        const Vector2 *texture_coords, int pointCount, const Color &tint)
{
    rlSetTexture(texture->id);
    rlBegin(RL_TRIANGLES);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    for (int i = 0; i < pointCount - 1; i++)
    {
        draw_triangle(center, points[i], points[i + 1], texture_coords[i], texture_coords[i + 1]);
    }
    draw_triangle(
            center, points[pointCount - 1], points[0], texture_coords[pointCount - 1],
            texture_coords[0]);
    rlEnd();
    rlSetTexture(0);
}

Vector2 operator*(const Vector2 &lhs, float rhs)
{
    return Vector2(lhs.x * rhs, lhs.y * rhs);
}

void operator*=(Vector2 &lhs, float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
}

Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

void operator+=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

Vector2 operator/(const Vector2 &lhs, float rhs)
{
    return Vector2(lhs.x / rhs, lhs.y / rhs);
}

float Vector2Distance(const Vector2 &lhs, const Vector2 &rhs)
{
    return sqrtf((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}

Vector2 Vector2Rotate(const Vector2 &v, float angle)
{
    Vector2 result = {0};

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres - v.y * sinres;
    result.y = v.x * sinres + v.y * cosres;

    return result;
}

bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void Distances::init(int width_, int height_)
{
    width = width_;
    height = height_;
    values = new int[width * height];
}

Distances::~Distances()
{
    delete[] values;
}


int Distances::operator[](Vector2 pt) const
{
    assert(pt.x < width);
    assert(pt.y < height);
    return values[int(pt.y) * width + int(pt.x)];
}

int &Distances::operator[](Vector2 pt)
{
    assert(pt.x < width);
    assert(pt.y < height);
    return values[int(pt.y) * width + int(pt.x)];
}

int Distances::operator()(int y, int x) const
{
    assert(x < width);
    assert(y < height);
    return values[y * width + x];
}

int &Distances::operator()(int y, int x)
{
    assert(x < width);
    assert(y < height);
    return values[y * width + x];
}
