#include "Car.h"

#include <cstdio>
#include <raymath.h>
#include <rlgl.h>

void DrawTexturePoly(
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


Car::Car(Texture *texture_, Color color_)
{
    texture = texture_;
    color = color_;
    position = Vector2(0, 0);

    constexpr float car_scale = 20.0f;
    const auto center = Vector2(texture->width / 2.0, texture->height / 2.0);

    shape[0] = (Vector2(0, 0) - center) / car_scale;
    shape[1] = (Vector2(0, texture->height) - center) / car_scale;
    shape[2] = (Vector2(texture->width, texture->height) - center) / car_scale;
    shape[3] = (Vector2(texture->width, 0) - center) / car_scale;
    shape[4] = (Vector2(0, 0) - center) / car_scale;

    texture_coords[0] = Vector2(0.0, 0.0);
    texture_coords[1] = Vector2(0.0, 1.0);
    texture_coords[2] = Vector2(1.0, 1.0);
    texture_coords[3] = Vector2(1.0, 0.0);
    texture_coords[4] = Vector2(0.0, 0.0);
}

void Car::draw(const Camera2D &camera) const
{
    BeginMode2D(camera);
    DrawTexturePoly(texture, position, shape, texture_coords, 5, color);
    DrawText(
            TextFormat("x %.0f y %.0f speed %.3f angle %.3f", position.x, position.y, speed, angle),
            position.x, position.y - 20, 10, WHITE);
    DrawPixel(position.x, position.y, RED);
    EndMode2D();
}

void Car::apply_action(car_actions_t action)
{
    float delta_angle = 0;
    if (action & CAR_ACTION_LEFT)
    {
        delta_angle = -0.3;
    }
    if (action & CAR_ACTION_RIGHT)
    {
        delta_angle = 0.3;
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

    rotate(delta_angle);
    angle += delta_angle;

    Vector2 direction;
    direction.x = cosf(angle * DEG2RAD) * speed;
    direction.y = sinf(angle * DEG2RAD) * speed;

    translate(direction);
}

void Car::set_position(int x, int y, float angle_)
{
    const Vector2 direction = Vector2(x, y) - position;
    rotate(angle_ - angle);
    translate(direction);
    angle = angle_;
}

void Car::rotate(float delta_angle)
{
    const float rad = delta_angle * DEG2RAD;
    for (auto &corner: shape)
    {
        corner = Vector2Rotate(corner - position, rad) + position;
    }
}

void Car::translate(Vector2 movement)
{
    position += movement;
    for (auto &corner: shape)
    {
        corner += movement;
    }
}
