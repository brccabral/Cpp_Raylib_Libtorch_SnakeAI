#pragma once
#include <raylib.h>
#include <vector>
#include "Car.h"


class Track
{
public:

    Track();
    virtual ~Track();

    virtual void draw(const Camera2D &camera) = 0;
    virtual void set_car_start(Car *car) = 0;

protected:

    Texture texture;
    std::vector<int> distances;
    Vector2 start{};
    Vector2 finish{};
    const char *file;

    void set_distances(Color track_color);
};
