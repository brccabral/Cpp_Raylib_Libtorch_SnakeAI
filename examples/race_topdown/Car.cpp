#include "Car.h"

#include <cstdio>
#include <raymath.h>


Car::Car(Texture *texture_, Color color_)
{
    texture = texture_;
    color = color_;

    position.x = GetRandomValue(0, 200);
}

void Car::draw(Camera2D &camera)
{
    BeginMode2D(camera);
    DrawTexturePro(
            *texture, Rectangle(0, 0, texture->width, texture->height),
            Rectangle(
                    position.x + texture->width / 2.0 / CAR_SCALE,
                    position.y + texture->height / 2.0 / CAR_SCALE, texture->width / CAR_SCALE,
                    texture->height / CAR_SCALE),
            Vector2(texture->width / 2.0 / CAR_SCALE, texture->height / 2.0 / CAR_SCALE), angle,
            color);
    DrawText(
            TextFormat("x %.0f y %.0f speed %.3f angle %.3f", position.x, position.y, speed, angle),
            position.x, position.y - 20, 10, WHITE);
    DrawPixel(position.x, position.y, RED);
    EndMode2D();
}

void Car::apply_action(car_actions_t action)
{
    if (action & CAR_ACTION_LEFT)
    {
        angle -= 0.3;
    }
    if (action & CAR_ACTION_RIGHT)
    {
        angle += 0.3;
    }
    if (action & CAR_ACTION_ACCELERATE)
    {
        speed += 0.1;
    }
    if (action & CAR_ACTION_BREAK)
    {
        speed -= 0.1;
    }

    speed = Clamp(speed, -0.7, 0.7);

    // TODO drag/boost
    if (speed > 0)
    {
        speed -= 0.01;
    }
    else if (speed < 0)
    {
        speed += 0.01;
    }

    if (std::abs(speed) < 0.001)
    {
        speed = 0;
    }

    Vector2 direction;
    direction.x = cosf(angle * DEG2RAD);
    direction.y = sinf(angle * DEG2RAD);

    position.x += direction.x * speed;
    position.y += direction.y * speed;
}
