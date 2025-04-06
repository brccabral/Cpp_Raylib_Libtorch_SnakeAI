#pragma once
#include <raylib.h>


class Car
{

public:

    Car(Texture *texture_, Color color_);
    void draw(Camera2D &camera);

    typedef enum car_state_t
    {
        CAR_STATE_ALIVE = 0,
        CAR_STATE_DEAD
    } car_state_t;

private:

    Texture *texture;
    float x;
    float y;
    float angle{};
    float speed;
    car_state_t car_state;
    Color color;
};
