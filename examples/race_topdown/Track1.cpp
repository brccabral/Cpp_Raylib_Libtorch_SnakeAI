#include "Track1.h"

Track1::Track1()
{
    file = "assets/track1.png";
    start = Vector2(1898, 882);
    finish = Vector2(1347, 1732);
    set_distances(Color(110, 110, 110, 255));
}

void Track1::set_car_start(Car *car)
{
    if (car == nullptr)
    {
        return;
    }
    const int x = GetRandomValue(1838, 1959);
    constexpr int y = 882;
    car->set_position(x, y, 270);
}

void Track1::draw(const Camera2D &camera)
{
    BeginMode2D(camera);
    DrawTexture(texture, 0, 0, WHITE);
    EndMode2D();
}
