#pragma once
#include "common.h"
#include <raylib.h>


#define NUM_SENSORS 17
#define ADVANCE_TIMEOUT (100.0)

class Car
{

public:

    Car(Texture *alive_texture_, Texture *dead_texture_, Color color_, size_t index_);
    void draw(const Camera2D &camera, size_t best_index) const;
    void update(const Distances &distances);
    void reset();

    // clang-format off
    typedef enum car_actions_t
    {
        CAR_ACTION_NONE = 0,
        CAR_ACTION_LEFT =             0b0001,
        CAR_ACTION_RIGHT =            0b0010,
        CAR_ACTION_ACCELERATE =       0b0100,
        CAR_ACTION_ACCELERATE_LEFT =  0b0101,
        CAR_ACTION_ACCELERATE_RIGHT = 0b0110,
        CAR_ACTION_BREAK =            0b1000,
        CAR_ACTION_BREAK_LEFT =       0b1001,
        CAR_ACTION_BREAK_RIGHT =      0b1010,
        CAR_ACTION_COUNT,
    } car_actions_t;
    // clang-format on
    void apply_action(car_actions_t action, int track_width, int track_height);
    void set_position(int x, int y, float angle_);

    typedef enum car_state_t
    {
        CAR_STATE_ALIVE = 0,
        CAR_STATE_DEAD
    } car_state_t;

    Vector2 position{};
    // angle in degrees
    float angle{};
    float sensors_distance[NUM_SENSORS]{};
    car_state_t car_state{};
    float speed{};
    int max_distance{};

private:

    Texture *texture{};
    Texture *alive_texture{};
    Texture *dead_texture{};
    Color color{};
    Vector2 shape[5]{};
    Vector2 texture_coords[5]{};
    float advance_timeout{};
    size_t index;

    void rotate(float delta_angle);
    void translate(Vector2 movement);
    void update_sensors(const Distances &distances);
    bool check_collision(const Distances &distances);
};
