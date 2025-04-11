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
{
    laser_distance = 1;
    laser_speed = 1;
}

Vector3 Track1::get_car_start()
{
    const int x = GetRandomValue(1855, 1945);
    constexpr int y = 860;
    constexpr float angle = 270;
    return Vector3(x, y, angle);
}

void Track1::draw(const Camera2D &camera)
{
    DrawTexture(texture, 0, 0, WHITE);

    if (IsKeyDown(KEY_L))
    {
        for (int r = 0; r < get_height(); ++r)
        {
            for (int c = 0; c < get_width(); ++c)
            {
                if (distances(r, c) == laser_distance)
                {
                    DrawRectangle(c, r, 3, 3, LIME);
                }
            }
        }
    }

    if (IsKeyDown(KEY_M))
    {
        draw_distances(camera);
    }
}

void Track1::draw_distances(const Camera2D &camera)
{
    for (auto r = 0; r < GetScreenHeight(); ++r)
    {
        for (auto c = 0; c < GetScreenWidth(); ++c)
        {
            auto world = GetScreenToWorld2D(Vector2(c, r), camera);
            if (world.x < 0 || world.y < 0 || world.x > texture.width - 1 ||
                world.y > texture.height - 1)
            {
                continue;
            }
            if (distances[world] > 0)
            {
                DrawPixel(world.x, world.y, WHITE);
            }
        }
    }
}

void Track1::update_finish_line_distances(int max_distance)
{
    for (auto r = 1733; r < 1753; ++r)
    {
        for (auto c = 1287; c < 1408; ++c)
        {
            distances(r, c) = max_distance;
        }
    }
}

void Track1::update()
{
    laser_distance += laser_speed;
    laser_speed += 0.004;
}
