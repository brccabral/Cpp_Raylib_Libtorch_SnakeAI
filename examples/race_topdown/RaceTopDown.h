#pragma once

#include <vector>
#include <array>
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
    bool check_end_game() const;
    void reset();
    static size_t get_state_size();
    std::vector<float> get_state(size_t index);
    std::array<size_t, 2> select_best_cars() const;

    std::vector<Car> cars{};
    size_t num_dead{};
    size_t best_car{};
    int max_distance{};

private:

    Color colors[NUM_COLORS] = {GRAY, YELLOW, GREEN, RED, BLUE, CYAN, ORANGE, PURPLE, WHITE};

    std::vector<Track *> tracks{};
    size_t current_track{};
    Camera2D camera{};
    Texture car_texture{};
    Texture car_dead{};
    size_t num_cars{};


    Track1 track1{};
};
