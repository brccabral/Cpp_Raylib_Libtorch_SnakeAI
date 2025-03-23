#pragma once

#include <vector>
#include <raylib.h>

typedef enum Direction
{
    RIGHT = 1,
    LEFT,
    UP,
    DOWN
} Direction;

class SnakeGameAI
{
    int w = 32;
    int h = 24;
    Direction direction = RIGHT;
    Vector2 head = Vector2(w / 2.0f, h / 2.0f);
    void new_food();
    void place_food();

public:

    SnakeGameAI();
    std::vector<Vector2> snake;
    Vector2 food{};
    int score = 0;
};
