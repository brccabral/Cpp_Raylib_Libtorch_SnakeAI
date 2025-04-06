#pragma once
#include <vector>
#include "Track.h"

class RaceTopDown
{
public:

    RaceTopDown();
    void update();
    void draw();

private:

    std::vector<Track> tracks{};
    size_t current_track{};
    Camera2D camera{};
};
