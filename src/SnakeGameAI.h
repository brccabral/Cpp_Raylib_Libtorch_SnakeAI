#pragma once

#include <vector>
#include <array>
#include <raylib.h>

#define INPUT_SIZE 15

typedef struct StepResult
{
    int reward = 0;
    bool game_over = false;
} StepResult;

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

    typedef enum action_t
    {
        ACTION_STRAIGHT,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_COUNT
    } action_t;

    std::vector<Vector2> snake;
    Vector2 food{};
    int score = 0;
    Direction direction = RIGHT;

    std::array<int, INPUT_SIZE> get_state();
    StepResult get_step(action_t action);
    void reset();

private:

    struct FieldLoc
    {
        Vector2 p;
        int distance;
    };

    int w = 32;
    int h = 24;
    int frame_iteration = 0;
    Vector2 head = Vector2(w / 2.0f, h / 2.0f);
    Direction clock_wise_direction[4] = {RIGHT, DOWN, LEFT, UP};
    int distance_field[32 * 24]{-1};
    Vector2 p_right = {1, 0};
    Vector2 p_left = {-1, 0};
    Vector2 p_up = {0, -1};
    Vector2 p_down = {0, 1};
    void new_food();
    void place_food();
    bool is_collision(Vector2 pt);
    void move(action_t action);
    void update_field();
    size_t index_from_location(Vector2 pt);
};
