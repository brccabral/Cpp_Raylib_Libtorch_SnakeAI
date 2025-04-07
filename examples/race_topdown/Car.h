#pragma once
#include <raylib.h>


#define CAR_SCALE 20.0

class Car
{

public:

    Car(Texture *texture_, Color color_);
    void draw(Camera2D &camera);


    typedef enum car_actions_t
    {
        CAR_ACTION_NONE = 0,
        CAR_ACTION_LEFT,
        CAR_ACTION_RIGHT,
        CAR_ACTION_ACCELERATE,
        CAR_ACTION_BREAK,
    } car_actions_t;
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
