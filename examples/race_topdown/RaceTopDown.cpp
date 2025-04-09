#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include "RaceTopDown.h"
#include "Track.h"


RaceTopDown::RaceTopDown(size_t num_cars_)
{
    camera.zoom = 1.0f;
    tracks.emplace_back(&track1);
    car_texture = LoadTexture("assets/car.png");
    car_dead = LoadTexture("assets/dead.png");
    num_cars = num_cars_;
    cars.reserve(num_cars_);
    for (size_t i = 0; i < num_cars_; i++)
    {
        cars.emplace_back(&car_texture, &car_dead, colors[i % NUM_COLORS], i);
    }
    reset();
};

RaceTopDown::~RaceTopDown()
{
    UnloadTexture(car_texture);
    UnloadTexture(car_dead);
}

void RaceTopDown::reset()
{
    num_dead = 0;
    best_car = 0;
    max_distance = 0;

    for (size_t i = 0; i < num_cars; i++)
    {
        cars[i].reset();
        const auto [x, y, angle] = tracks[current_track]->get_car_start();
        cars[i].set_position(x, y, angle);
    }
}


void RaceTopDown::update()
{
    // Camera follows cars[best_car];
    if (IsKeyDown(KEY_P))
    {
        camera.offset = GetWorldToScreen2D(cars[best_car].position, camera);
        camera.target = cars[best_car].position;

        const float wheel = GetMouseWheelMove();
        if (wheel != 0)
        {
            const float scale = 0.2f * wheel;
            camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
        }

        Vector2 delta = Vector2(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0) -
                        GetWorldToScreen2D(cars[best_car].position, camera);
        delta *= -1.0f / camera.zoom;
        camera.target += delta;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta *= -1.0f / camera.zoom;
        camera.target += delta;
    }

    // Zoom based on mouse wheel
    const float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Get the world point that is under the mouse
        const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        // Set the offset to where the mouse is
        camera.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = mouseWorldPos;

        // Zoom increment
        // Uses log scaling to provide consistent zoom speed
        const float scale = 0.2f * wheel;
        camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
    }
}

void RaceTopDown::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    tracks[current_track]->draw(camera);
    for (auto &car: cars)
    {
        car.draw(camera, best_car);
    }
    EndDrawing();
}

void RaceTopDown::apply_action(size_t index, int action)
{
    Car *car = &cars[index];
    car->apply_action(
            (Car::car_actions_t) action, tracks[current_track]->get_width(),
            tracks[current_track]->get_height());
    car->update(tracks[current_track]->distances, tracks[current_track]->get_width());
    if (car->car_state == Car::CAR_STATE_DEAD)
    {
        ++num_dead;
    }
    if (car->max_distance > max_distance)
    {
        max_distance = car->max_distance;
        best_car = index;
    }
}

bool RaceTopDown::check_end_game() const
{
    if (num_dead >= num_cars)
    {
        return true;
    }
    return false;
}

size_t RaceTopDown::get_state_size()
{
    return NUM_SENSORS + 1; // sensors + speed
}

std::vector<float> RaceTopDown::get_state(size_t index)
{
    std::vector<float> states(get_state_size());

    const Car *car = &cars[index];
    std::ranges::copy(car->sensors_distance, states.begin());
    states[get_state_size() - 1] = car->speed;

    return states;
}

std::array<size_t, 2> RaceTopDown::select_best_cars() const
{
    std::vector<std::pair<double, size_t>> cars_distances;
    cars_distances.reserve(cars.size());
    for (size_t i = 0; i < cars.size(); ++i)
    {
        cars_distances.emplace_back(cars[i].max_distance, i);
    }
    std::ranges::sort(
            cars_distances, [](const auto &a, const auto &b) { return a.first > b.first; });
    return {cars_distances[0].second, cars_distances[1].second};
}
