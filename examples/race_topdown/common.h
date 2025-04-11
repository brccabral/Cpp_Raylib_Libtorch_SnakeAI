#pragma once
#include <raylib.h>

#define MANUAL 0

void DrawTexturePoly(
        const Texture2D *texture, const Vector2 &center, const Vector2 *points,
        const Vector2 *texture_coords, int pointCount, const Color &tint);

Vector2 operator*(const Vector2 &lhs, float rhs);
void operator*=(Vector2 &lhs, float rhs);
Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs);
Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs);
void operator+=(Vector2 &lhs, const Vector2 &rhs);
Vector2 operator/(const Vector2 &lhs, float rhs);
float Vector2Distance(const Vector2 &lhs, const Vector2 &rhs);
Vector2 Vector2Rotate(const Vector2 &v, float angle);
bool operator==(const Vector2 &lhs, const Vector2 &rhs);

class Distances
{
public:

    Distances() = default;
    Distances(int width_, int height_);
    int operator[](Vector2 pt) const;
    int &operator[](Vector2 pt);
    int operator()(int y, int x) const;
    int &operator()(int y, int x);

    int *values{};
    int width{};
    int height{};
};

typedef struct Vector2i
{
    int x;
    int y;
} Vector2i;
