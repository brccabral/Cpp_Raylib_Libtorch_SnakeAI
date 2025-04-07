#pragma once
#include <raylib.h>


#define CAR_SCALE 20.0

class Car
{

public:

    Car(Texture *texture_, Color color_);
    void draw(Camera2D &camera);

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
    } car_actions_t;
    // clang-format on
    void apply_action(car_actions_t action);

    typedef enum car_state_t
    {
        CAR_STATE_ALIVE = 0,
        CAR_STATE_DEAD
    } car_state_t;

    Vector2 position;

private:

    Texture *texture;
    // angle in degrees
    float angle{};
    float speed{};
    car_state_t car_state{};
    Color color{};
};
