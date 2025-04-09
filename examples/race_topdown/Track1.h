#pragma once

#include "Track.h"

class Track1 : public Track
{
public:

    Track1();

    void draw(const Camera2D &camera) override;
    Vector3 get_car_start() override;
    void reset() override;

private:

    void draw_distances(const Camera2D &camera);
};
