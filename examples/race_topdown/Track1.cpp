#include "common.h"
#include "Track1.h"


Track1::Track1()
{
    file = "assets/track1.png";
    start = Vector2(1898, 882);
    finish = Vector2(1347, 1732);
    set_distances(Color(110, 110, 110, 255));
}

void Track1::reset()
{}


Vector3 Track1::get_car_start()
{
    const int x = GetRandomValue(1838, 1959);
    constexpr int y = 867;
    constexpr float angle = 270;
    return Vector3(x, y, angle);
}

void Track1::draw(const Camera2D &camera)
{
    BeginMode2D(camera);
    DrawTexture(texture, 0, 0, WHITE);

    if (IsKeyDown(KEY_M))
    {
        draw_distances(camera);
    }

    EndMode2D();
}

void Track1::draw_distances(const Camera2D &camera)
{
    for (auto r = 0; r < GetScreenHeight(); ++r)
    {
        for (auto c = 0; c < GetScreenWidth(); ++c)
        {
            auto world = GetScreenToWorld2D(Vector2(c, r), camera);
            if (world.x < 0 || world.y < 0 || world.x > texture.width || world.y > texture.height)
            {
                continue;
            }
            if (distances[(int) world.y][(int) world.x] > 0)
            {
                DrawPixel(world.x, world.y, WHITE);
            }
        }
    }
}
