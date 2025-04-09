#include "Car.h"
#include "common.h"
#include <raymath.h>


Car::Car(Texture *alive_texture_, Texture *dead_texture_, Color color_, size_t index_)
{
    alive_texture = alive_texture_;
    dead_texture = dead_texture_;
    texture = alive_texture_;
    color = color_;
    index = index_;

    texture_coords[0] = Vector2(0.0, 0.0);
    texture_coords[1] = Vector2(0.0, 1.0);
    texture_coords[2] = Vector2(1.0, 1.0);
    texture_coords[3] = Vector2(1.0, 0.0);
    texture_coords[4] = Vector2(0.0, 0.0);

    reset();
}

void Car::draw(const Camera2D &camera, size_t best_index) const
{
    BeginMode2D(camera);
    if (index == best_index)
    {
        DrawCircle(position.x, position.y, 30, Color(88, 88, 88, 80));
    }
    DrawTexturePoly(texture, position, shape, texture_coords, 5, color);

    if (IsKeyDown(KEY_P))
    {
        DrawText(
                TextFormat(
                        "x %.0f y %.0f speed %.3f angle %.3f max_distance %d advance_timeout %.0f",
                        position.x, position.y, speed, angle, max_distance, advance_timeout),
                position.x - 100, position.y - 20, 10, WHITE);
        for (auto i = 0; i < NUM_SENSORS; ++i)
        {
            const double sensor_angle = angle - 90 + i * 180.0 / NUM_SENSORS;
            const Vector2 step = Vector2(cos(sensor_angle * DEG2RAD), sin(sensor_angle * DEG2RAD)) *
                                 sensors_distance[i];
            const Vector2 sensor_position = position + step;
            DrawLineV(position, sensor_position, BLUE);
        }
    }

    EndMode2D();
}

void Car::apply_action(car_actions_t action, int track_width, int track_height)
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

    speed = Clamp(speed, -1.0, 1.0);

    // TODO drag/boost
    if (speed > 0)
    {
        speed -= 0.005;
    }
    else if (speed < 0)
    {
        speed += 0.005;
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

    if (position.x < 0 || position.x > track_width)
    {
        translate(direction * -1);
    }
    if (position.y < 0 || position.y > track_height)
    {
        translate(direction * -1);
    }
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

void Car::update_sensors(const std::vector<int> &distances, int track_width)
{
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        Vector2 sensor_position = position;
        const double sensor_angle = angle - 90 + i * 180.0 / NUM_SENSORS;
        const auto step = Vector2(cos(sensor_angle * DEG2RAD), sin(sensor_angle * DEG2RAD));
        while (true)
        {
            if (distances[index_from_location(sensor_position, track_width)] == 0)
            {
                sensors_distance[i] = Vector2Distance(position, sensor_position);
                break;
            }
            sensor_position += step;
        }
    }
}

void Car::update(const std::vector<int> &distances, int track_width)
{
    advance_timeout -= 0.1;
    if (distances[index_from_location(position, track_width)] > max_distance)
    {
        max_distance = distances[index_from_location(position, track_width)];
        advance_timeout = ADVANCE_TIMEOUT;
    }
    update_sensors(distances, track_width);
    if (advance_timeout < 0.0 || check_collision(distances, track_width))
    {
        car_state = CAR_STATE_DEAD;
        texture = dead_texture;
    }
}

bool Car::check_collision(const std::vector<int> &distances, int track_width)
{
    for (auto i = 0; i < 4; ++i)
    {
        if (distances[index_from_location(shape[i], track_width)] == 0.0)
        {
            return true;
        }
    }
    return false;
}

void Car::reset()
{
    car_state = CAR_STATE_ALIVE;
    speed = 0;
    angle = 0;
    advance_timeout = ADVANCE_TIMEOUT;
    texture = alive_texture;
    max_distance = 0;

    position = Vector2(0, 0);

    constexpr float car_scale = 20.0f;
    const auto center = Vector2(texture->width / 2.0, texture->height / 2.0);

    shape[0] = (Vector2(0, 0) - center) / car_scale;
    shape[1] = (Vector2(0, texture->height) - center) / car_scale;
    shape[2] = (Vector2(texture->width, texture->height) - center) / car_scale;
    shape[3] = (Vector2(texture->width, 0) - center) / car_scale;
    shape[4] = (Vector2(0, 0) - center) / car_scale;
}
