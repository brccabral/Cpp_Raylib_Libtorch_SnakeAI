#pragma once

#include <vector>
#include "Car.h"
#include "Track1.h"

#define CYAN CLITERAL(Color){0, 255, 255, 255} // CYAN
#define NUM_COLORS 9

class RaceTopDown
{
public:

    RaceTopDown(size_t num_cars_);
    ~RaceTopDown();
    void update();
    void draw();
    void apply_action(size_t index, int action);

private:

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE, WHITE};

    std::vector<Track *> tracks{};
    size_t current_track{};
    Camera2D camera{};
    Texture car_texture{};
    size_t num_cars{};
    std::vector<Car> cars{};


    Track1 track1{};
};
