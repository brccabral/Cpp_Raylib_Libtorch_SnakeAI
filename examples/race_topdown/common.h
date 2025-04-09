#pragma once
#include <cstddef>
#include <raylib.h>
#include <rlgl.h>

inline size_t index_from_location(Vector2 pt, size_t width)
{
    return int(pt.y) * width + int(pt.x);
}

inline void DrawTexturePoly(
        const Texture2D *texture, Vector2 center, const Vector2 *points,
        const Vector2 *texture_coords, int pointCount, Color tint)
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
