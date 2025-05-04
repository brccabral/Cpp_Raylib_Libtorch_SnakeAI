#include <algorithm>
#include <cmath>
#include "Car.h"


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

    reset();
}

void Car::draw_debug() const
{
    DrawText(
            TextFormat(
                    "x %.0f y %.0f speed %.3f angle %.3f max_distance %d advance_timeout "
                    "%.0f",
                    position.x, position.y, speed, angle, max_distance, advance_timeout),
            position.x - 100, position.y - 20, 10, WHITE);
    for (auto i = 0; i < NUM_SENSORS; ++i)
    {
        const double sensor_angle = angle - 90 + i * 180.0 / NUM_SENSORS;
        const Vector2 step =
                Vector2(std::cos(sensor_angle * DEG2RAD), std::sin(sensor_angle * DEG2RAD)) *
                sensors_distance[i];
        const Vector2 sensor_position = position + step;
        DrawLineV(position, sensor_position, BLUE);
    }
    DrawCircle(position.x, position.y, 30, Color(88, 88, 88, 80));
}

void Car::draw() const
{
    DrawTexturePoly(texture, position, shape, texture_coords, 4, color);
}

void Car::apply_action(car_actions_t action, int track_width, int track_height)
{
    float delta_angle = 0;
    if (action & CAR_ACTION_LEFT)
    {
        delta_angle = -5.0;
    }
    if (action & CAR_ACTION_RIGHT)
    {
        delta_angle = 5.0;
    }
    if (action & CAR_ACTION_ACCELERATE)
    {
        speed += 0.2;
    }
    if (action & CAR_ACTION_BREAK)
    {
        speed -= 0.2;
    }

    speed = std::max(speed, -4.0f);

    // TODO drag/boost
    if (speed > 0)
    {
        speed -= 0.1;
    }
    else if (speed < 0)
    {
        speed += 0.1;
    }

    if (std::abs(speed) < 0.001)
    {
        speed = 0;
    }

    rotate(delta_angle);

    Vector2 direction;
    direction.x = cosf(angle * DEG2RAD) * speed;
    direction.y = sinf(angle * DEG2RAD) * speed;

    translate(direction);

    if (position.x < 0 || position.x > track_width || position.y < 0 || position.y > track_height)
    {
        translate(direction * -1);
    }
}

void Car::set_position(int x, int y, float angle_)
{
    const Vector2 direction = Vector2(x, y) - position;
    rotate(angle_ - angle);
    translate(direction);
}

void Car::rotate(float delta_angle)
{
    const float rad = delta_angle * DEG2RAD;
    for (auto &corner: shape)
    {
        corner = Vector2Rotate(corner - position, rad) + position;
    }
    angle += delta_angle;
}

void Car::translate(Vector2 movement)
{
    position += movement;
    for (auto &corner: shape)
    {
        corner += movement;
    }
}

void Car::update_sensors(const Distances &distances)
{
    for (int i = 0; i < NUM_SENSORS; ++i)
    {
        auto sensor_position = Vector2i(position.x, position.y);
        const double sensor_angle = angle - 90 + i * 180.0 / NUM_SENSORS;

        const auto sensor_direction =
                Vector2(std::cos(sensor_angle * DEG2RAD), std::sin(sensor_angle * DEG2RAD));

        Vector2 delta;
        delta.x = sensor_direction.x == 0 ? 1e30f : std::abs(1 / sensor_direction.x);
        delta.y = sensor_direction.y == 0 ? 1e30f : std::abs(1 / sensor_direction.y);

        Vector2 step;
        Vector2 inner_depth;
        if (sensor_direction.x < 0)
        {
            step.x = -1;
            inner_depth.x = (position.x - sensor_position.x) * delta.x;
        }
        else
        {
            step.x = 1;
            inner_depth.x = (sensor_position.x + 1.0 - position.x) * delta.x;
        }
        if (sensor_direction.y < 0)
        {
            step.y = -1;
            inner_depth.y = (position.y - sensor_position.y) * delta.y;
        }
        else
        {
            step.y = 1;
            inner_depth.y = (sensor_position.y + 1.0 - position.y) * delta.y;
        }

        while (true)
        {
            if (distances(sensor_position.y, sensor_position.x) == 0)
            {
                sensors_distance[i] =
                        Vector2Distance(position, Vector2(sensor_position.x, sensor_position.y));
                break;
            }

            if (inner_depth.x < inner_depth.y)
            {
                sensor_position.x += step.x;
                inner_depth.x += delta.x;
            }
            else
            {
                sensor_position.y += step.y;
                inner_depth.y += delta.y;
            }
        }
    }
}

void Car::update(const Distances &distances, int laser_distance)
{
    advance_timeout -= 0.1;
    if (distances[position] > max_distance)
    {
        max_distance = distances[position];
        advance_timeout = ADVANCE_TIMEOUT;
    }
    update_sensors(distances);
#if !MANUAL
    if (advance_timeout < 0.0 || check_collision(distances) ||
        distances[position] <= laser_distance)
    {
        car_state = CAR_STATE_DEAD;
        texture = dead_texture;
    }
#endif
}

bool Car::check_collision(const Distances &distances)
{
    for (auto i = 0; i < 4; ++i)
    {
        if (distances[shape[i]] == 0.0)
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
}

size_t Car::get_action_count()
{
    return 4;
}
