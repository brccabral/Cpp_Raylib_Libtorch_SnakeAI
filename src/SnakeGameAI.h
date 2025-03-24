#pragma once

#include <vector>
#include <array>
#include <raylib.h>

#define INPUT_SIZE 11

class SnakeGameAI
{
public:

    SnakeGameAI();

    typedef enum Direction
    {
        RIGHT = 1,
        LEFT,
        UP,
        DOWN
    } Direction;

    typedef enum action
    {
        ACTION_STRAIGHT,
        ACTION_LEFT,
        ACTION_RIGHT
    } action;

    std::vector<Vector2> snake;
    Vector2 food{};
    int score = 0;
    Direction direction = RIGHT;

    std::array<int, INPUT_SIZE> get_state();

private:

    int w = 32;
    int h = 24;
    Vector2 head = Vector2(w / 2.0f, h / 2.0f);
    void new_food();
    void place_food();
    bool is_collision(Vector2 pt);
};
