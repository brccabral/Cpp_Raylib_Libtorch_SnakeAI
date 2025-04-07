#pragma once

#include "Track.h"

class Track1 : public Track
{
public:

    Track1();

    void draw(const Camera2D &camera) override;
    void set_car_start(Car *car) override;
};
